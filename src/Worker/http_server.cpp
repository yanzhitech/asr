#include "http_server.hpp"
#include "worker.hpp"
#include "worker_loader.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <poll.h>
#include "output_package.h"
#include "glog/logging.h"

static const char* DEFAULT_EMPTY_RESULT = "<DOCUMENT></DOCUMENT>";

Http_Server::Http_Server() {
    _epoll_fd = INVALID_FD;
    _pipe_read = INVALID_FD;
    _pipe_write = INVALID_FD;
    _socket_server_listen = INVALID_FD;
    _stop_task = 0;
    _epoll_ready_event_num = 0;
    _now_connections = 0;

    pthread_mutex_init(&_epoll_mutex, NULL);
    pthread_mutex_init(&_map_mutex,NULL);
    pthread_mutex_init(&_connections_mutex,NULL);
}

Http_Server::~Http_Server() {
    ::close(_epoll_fd);
    ::close(_pipe_read);
    ::close(_pipe_write);
    ::close(_socket_server_listen);
    pthread_mutex_destroy(&_epoll_mutex);
    pthread_mutex_destroy(&_map_mutex);
    pthread_mutex_destroy(&_connections_mutex);

    for (std::map<int, SocketHandle*>::const_iterator it = m_fd_set.begin(); it != m_fd_set.end(); ++it) {
        ::close(it->second->get_fd());
        delete it->second;
    }
}

int Http_Server::register_worker_loader(Worker_Loader* workerloader) {
    m_workerloader = workerloader;
    return 0;
}

int Http_Server::open(size_t thread_num, size_t stack_size, int port) {
    if ((_epoll_fd = epoll_create(MAX_FD_NUM)) == -1) {
        fprintf(stderr,"error: epoll create fail!!\n");
        return -1;
    }
    if (create_pipe()) {
        fprintf(stderr,"error: create pipe fail!! \n");
        return -1;
    }
    if (create_listen(_socket_server_listen, port)) {
        fprintf(stderr,"error: create listen fail !! port: %d\n", port);
        return -1;
    }

    add_input_fd(_pipe_read);
    add_input_fd(_socket_server_listen);

    return task_base::open(thread_num, stack_size);
}

int Http_Server::stop() {
    _stop_task = 1;
    write(_pipe_write, &_stop_task, 1);
    join();
    return 0;
}

int Http_Server::svc() {
    int fd,new_fd;

    while (!_stop_task) {
        pthread_mutex_lock(&_epoll_mutex);
        if (_stop_task) {
            pthread_mutex_unlock(&_epoll_mutex);
            break;
        }

        if (_epoll_ready_event_num <= 0) {
            _epoll_ready_event_num = epoll_wait(_epoll_fd, _epoll_ready_event, MAX_EPOLL_EVENT_NUM, -1);
        }
        if (_epoll_ready_event_num-- < 0) {
            pthread_mutex_unlock(&_epoll_mutex);
            if (errno == EINTR) {
                continue;    
            }
            else {
                break;
            }
        }

        fd = _epoll_ready_event[_epoll_ready_event_num].data.fd;
        if (fd == _socket_server_listen) {
            while ((new_fd = accept(fd, NULL, NULL)) >= 0) {
                pthread_mutex_lock(&_connections_mutex);                
                _now_connections++;                            
                pthread_mutex_unlock(&_connections_mutex);
                set_socket(new_fd, O_NONBLOCK);
                create_handle(new_fd,LISTEN_PORT_1);
                add_input_fd(new_fd);
                LOG(INFO) << "[new connection on socket(" << new_fd << ")]";
            }
            pthread_mutex_unlock(&_epoll_mutex);
            continue;
        }
        if (fd == _pipe_read) {
            pthread_mutex_unlock(&_epoll_mutex);
            continue;
        }

        del_input_fd(fd);
        pthread_mutex_unlock(&_epoll_mutex);
        LOG(INFO) << "before new worker"; 
        recv_request(fd);
    }

    return 0;
}

int Http_Server::create_pipe()
{
    int options;
    int pipe_fd[2];

    if (pipe(pipe_fd)) {
        return -1;
    }

    _pipe_read = pipe_fd[0];
    _pipe_write = pipe_fd[1];

    for (int i=0; i<2; i++) {
        if ((options = fcntl(pipe_fd[i], F_GETFL)) == -1) {
            return -1;
        }
        if (fcntl(pipe_fd[i], F_SETFL, options | O_NONBLOCK) == -1) {
            return -1;
        }
    }

    return 0;
}

int Http_Server::create_listen(int &socket_fd, unsigned short port) {
    sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
    if (set_socket(socket_fd, O_NONBLOCK)) {
        return -1;
    }
    if (::bind(socket_fd, (const sockaddr*)&addr, sizeof addr)) {
        return -1;
    }
    if (listen(socket_fd, MAX_FD_NUM)) {
        return -1;
    }

    return 0;
}

int Http_Server::set_socket(int fd, int flag) {
    int options;
    options = SOCKET_SND_BUF_SIZE;
    setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &options, sizeof(int));
    options = SOCKET_RCV_BUF_SIZE;
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &options, sizeof(int));
    options = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(int));
    options = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, options | flag);
    int on = 1;
    int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));
    return ret;
}

int Http_Server::add_input_fd(int fd) {
    epoll_event event;
    bzero(&event, sizeof(epoll_event));
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &event);

    return 0;
}

int Http_Server::del_input_fd(int fd) {
    epoll_event event;
    bzero(&event, sizeof(epoll_event));
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &event);

    return 0;
}

int Http_Server::create_handle(int fd, int flag) {
    SocketHandle * newhdle = new SocketHandle;
    newhdle->set_flag(flag);
    newhdle->set_fd(fd);
    insert_handle(fd,newhdle);

    return 0;
}

int Http_Server::insert_handle(int fd,SocketHandle * hdle) {
    pthread_mutex_lock(&_map_mutex);
    std::pair< std::map<int, SocketHandle*>::iterator , bool> ret = 
        m_fd_set.insert(std::map<unsigned int, SocketHandle*>::value_type(fd, hdle));
    pthread_mutex_unlock(&_map_mutex);
    if(!ret.second) {
        LOG(ERROR) << "<btlswkxt> insert map error!!";
        return -1;
    }

    return 0;
}

SocketHandle * Http_Server::get_handle(int fd) {    
    pthread_mutex_lock(&_map_mutex);
    std::map<int, SocketHandle *>::iterator it = m_fd_set.find(fd);
    if (it != m_fd_set.end())  {
        it->second->acquire();
        pthread_mutex_unlock(&_map_mutex);
        return it->second;
    }
    pthread_mutex_unlock(&_map_mutex);
    LOG(ERROR) << "<btlswkxt> do not get handle !!";
    return NULL;
}

int Http_Server::release_handle(SocketHandle* hdle) {
    pthread_mutex_lock(&_map_mutex);
    if(hdle->release() == 0 && (hdle->get_flag() & ERROR_SOCKET)) {
        int fd = hdle->get_fd();
        del_input_fd(fd);
        ::close(fd);
        delete hdle;
        m_fd_set.erase(fd);
        pthread_mutex_lock(&_connections_mutex);
        _now_connections--;
        pthread_mutex_unlock(&_connections_mutex);
        LOG(ERROR) << "close connection " << fd;
    }    
    pthread_mutex_unlock(&_map_mutex);

    return 0;
}

int Http_Server::recv_request(int fd) {
    SocketHandle * hdle = get_handle(fd);    assert(hdle);
    Worker * worker = new Worker; assert(worker);
    timeval timeout = { 0, RECEIVE_TIMEOUT * 1000 };
    std::string httpheader;
    int length;

    if ( read_http_header_timeout(hdle, httpheader, &timeout) == 0 ) {
        if(httpheader.length() >0 && GetInfo(httpheader,length) == 0) {

            int buf_len = readn_timeout(hdle, worker->request,length,&timeout);
            LOG(INFO) << "[readn_timeout][buffer]" << worker->request;
            LOG(INFO) << "[readn_timeout][buffer len]" << buf_len;
            if(buf_len == length ) {
                worker->handle = hdle;
                gettimeofday(&worker->receive_time, NULL);
                m_workerloader->submit_worker(worker);
                add_input_fd(fd);
                return 0;
            }
            else {
                LOG(ERROR) << "[readn_timeout]" << httpheader;
            }
        }
        else {
            LOG(ERROR) << "[Package Invalid]" << httpheader;
            delete worker;
            worker=NULL;
            add_input_fd(fd);
            return 0;
        }
    }

    LOG(ERROR) << "[disconnect error socket]" <<  fd;
    hdle->set_flag(hdle->get_flag() | ERROR_SOCKET);
    release_handle(hdle);
    delete worker;
    worker=NULL;
    return 0;
}

double difftime(timeval t1,timeval t2) {
    double sec = t1.tv_sec - t2.tv_sec;
    double usec = t1.tv_usec - t2.tv_usec;
    return sec+usec/1000000;
}

int Http_Server::retrieve_worker(Worker * worker) {
    int ret = 0;
    if(worker == NULL) {
        return -1;
    }
    ///////////////////////////////
    Output_Package::toJson(worker);
    //////////////////////////////
    if(worker->result_len > 0) {
        if((ret = send_result(worker->handle,worker->result, worker->result_len))<0) {
            LOG(ERROR) << "send result error! ret:" << ret << ", request: " << worker->request;
        }
    }
    else {
        strcpy(worker->result,DEFAULT_EMPTY_RESULT);
        worker->result_len=strlen(worker->result);
        if((ret = send_result(worker->handle,worker->result, worker->result_len))<0) {
            LOG(ERROR) << "send result error! ret:" << ret << ", request:" << worker->request;      
        }
    }
    release_handle(worker->handle);

    // timeval t1 = worker->receive_time; 
    // timeval t2 = worker->loadpage_time; 
    // timeval t3;
    // gettimeofday(&t3, NULL);
    gettimeofday(&worker->store->t_stat[END_TIME],NULL);


    double wait_time	= difftime(worker->store->t_stat[BEGIN_TIME],worker->store->t_stat[CREATE_TIME]);	
    double prepare_time	= difftime(worker->store->t_stat[QUERY_TIME],worker->store->t_stat[BEGIN_TIME]);	
    double query_time	= difftime(worker->store->t_stat[BACK_TIME],worker->store->t_stat[QUERY_TIME]);
    double post_time	= difftime(worker->store->t_stat[END_TIME],worker->store->t_stat[BACK_TIME]);
    double total_time	= difftime(worker->store->t_stat[END_TIME],worker->store->t_stat[CREATE_TIME]);

    LOG(INFO) << "wait_time = " << wait_time;
    LOG(INFO) << "prepare_time = " << prepare_time;
    LOG(INFO) << "query_time = " << query_time;
    LOG(INFO) << "post_time = " << post_time;
    LOG(INFO) << "total_time = " << total_time;

    delete worker;
    worker=NULL;
    return ret;
}

int Http_Server::send_result(SocketHandle *hdle,const void * buf, int buf_len) {
    int ret = 0;
    std::string log;
    log.assign((char*)buf, buf_len);
    LOG(INFO) << "log-result:" <<  log;
    timeval timeout = { 0, REPLY_TIMEOUT * 1000 };

    std::string httpheader;
    if(generate_http_header(httpheader,buf_len) <=0 ) {
        return -1;
    }
    hdle->lock();
    if(writen_timeout(hdle->get_fd(),httpheader.c_str(),httpheader.length(),&timeout) !=(int)httpheader.length()) {
        ret = -2;
    }
    else if(writen_timeout(hdle->get_fd(), buf, buf_len, &timeout) != (int)buf_len) {
        ret = -3;    
    }
    hdle->unlock();    

    return ret;
}

int Http_Server::GetInfo(std::string header, int& length) {
    LOG(INFO) << "GetInfo header:" << header;
    char uri[2048];
    unsigned int http_version[2];
    const char * buf = header.find("POST") + header.c_str();
    LOG(INFO) << "GetInfo buf:" << buf;
    if (sscanf(buf, "POST /%2040s HTTP/%u.%u", uri, &(http_version[0]),&(http_version[1])) != 3) {
        LOG(ERROR) << "get post error!!";
        return -1;
    }

    buf = header.c_str() + header.find("Content-Length:");
    if(sscanf(buf,"Content-Length:%d",&length) != 1) {
        LOG(ERROR) << "get content length error!!";
        return -1;
    }

    if(strcasecmp(uri, "request") != 0) {
        LOG(ERROR) << "get source error!! search hub request?";
        return -1;
    }
    return 0;
}

int Http_Server::read_http_header_timeout(SocketHandle * hdle, std::string &http, timeval *timeout) {
    char buf[READ_BUF_SIZE + 1];
    int n,pos;    
    std::string & _buffer = hdle->get_read_buf();
    int fd = hdle->get_fd();
    LOG(INFO) << "http buffer: " << _buffer; 
    while(1) {
        if ((pos = _buffer.find("\r\n\r\n")) >= 0) {
            http = _buffer.substr(0, pos);
            _buffer = _buffer.substr(pos + 4);
            return 0;
        }
        if((n = read_data(fd,buf,READ_BUF_SIZE,timeout)) <= 0) {
            return -1;
        }
        buf[n] = '\0';
        _buffer += buf;
    }
    return -1;
}

int Http_Server::readn_timeout(SocketHandle * hdle, std::string &content, int buf_len, timeval *timeout) {
    char buf[READ_BUF_SIZE+1];
    int n,len;
    std::string & _buffer = hdle->get_read_buf();
    int left = buf_len - _buffer.length();
    int fd = hdle->get_fd();
    LOG(INFO) << "[readn_timeout][in]" << buf_len;
    LOG(INFO) << "[readn_timeout][buffer]" << _buffer;
    if(left<0) {
        content = _buffer.substr(0,buf_len);
        _buffer = _buffer.substr(buf_len);
        return buf_len;
    }
    content = _buffer;
    _buffer = "";

    while (left > 0) {
        len = left>READ_BUF_SIZE?READ_BUF_SIZE:left;
        if ((n = read_data(fd, buf, len,timeout)) <= 0) {
            return buf_len - left;
        }

        buf[n] = '\0';
        content += buf;
        left -= n;
    }

    return buf_len;
}

int Http_Server::read_data(int fd, void * buf, int buf_len, timeval * timeout) {
    int timeout_in_milliseconds = (NULL ==  timeout) ? -1 : 
        timeout->tv_sec * 1000 + timeout->tv_usec / 1000;
    struct pollfd fds[1] = {{fd, POLLIN, 0}};
    if (poll(fds, 1, timeout_in_milliseconds) <= 0) {
        return -1;
    }

    return read(fd, buf, buf_len);
}

int Http_Server::generate_http_header(std::string& res,int len) {
    char buf[HTTP_HEADER_MAX_LENGTH];

    //generate header
    int ret = snprintf(buf, HTTP_HEADER_MAX_LENGTH, "HTTP/1.1 %d %s\r\n",200,"OK");
    res = buf;

    // 生成响应报文时间戳
    time_t t;
    struct tm tmp_time;
    time(&t);
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%a, %e %b %Y %H:%M:%S %Z", gmtime_r(&t, &tmp_time));
    ret = snprintf(buf,HTTP_HEADER_MAX_LENGTH,"Date:%s\r\n",timestr);
    res += buf;

    //add content length
    ret = snprintf(buf,HTTP_HEADER_MAX_LENGTH,"Content-Length:%d\r\n",len);
    res += buf;

    //add content type
    ret = snprintf(buf,HTTP_HEADER_MAX_LENGTH,"Content-Type: %s; charset=%s\r\n","text/xml","utf-8");
    res += buf;

    snprintf(buf,HTTP_HEADER_MAX_LENGTH,"\r\n");
    res += buf;
    return res.length();
}

int Http_Server::writen_timeout(int fd, const void *buf, int buf_len, timeval *timeout) {
    int left = buf_len;
    int n;
    int timeout_in_milliseconds = (NULL ==  timeout) ? -1 : 
        timeout->tv_sec * 1000 + timeout->tv_usec / 1000;
    struct pollfd fds[1] = {{fd, POLLOUT, 0}};

    while (left > 0) {
        if (poll(fds, 1, timeout_in_milliseconds) <= 0) {
            return -1;
        }
        if ((n = write(fd, buf, left)) <= 0) {
            return buf_len - left;
        }

        buf = (char *)buf + n;
        left -= n;
    }
    return buf_len;
}

