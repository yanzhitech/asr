
#ifndef QO_HTTP_SERVER_HPP
#define QO_HTTP_SERVER_HPP
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include "task_base.hpp"
#include "socket_handle.hpp"
#include <sys/time.h>
#include <sys/epoll.h>
#include <map>
#include <string>
// #include "cache/mem_cache.hpp"
#include "glog/logging.h"
#define LISTEN_PORT_1               0x00000001U
#define ERROR_SOCKET                0x00000010U

#define MAX_EPOLL_EVENT_NUM         256
#define MAX_FD_NUM                  1024
#define RECEIVE_TIMEOUT             50
#define REPLY_TIMEOUT               50
#define SOCKET_SND_BUF_SIZE         (1024*1024)
#define SOCKET_RCV_BUF_SIZE         (1024*1024)
#define READ_BUF_SIZE       512
#define HTTP_HEADER_MAX_LENGTH  1024

#define WASTE_TIME_US_INTERVAL(start, end)\
	({\
	 (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);\
	 })
class Worker_Loader; 
class Worker;

class Http_Server : public task_base
{
public:
    Http_Server();
    virtual ~Http_Server();
    int register_worker_loader(Worker_Loader *pageloader);
    // void register_cache(Mem_Cache* cache);

    int open(size_t thread_num, size_t stack_size, int port);
    virtual int stop();
    virtual int svc();

    int retrieve_worker(Worker * worker);

protected:
    int create_pipe();
    int create_listen(int &socket_fd, unsigned short port);
    int set_socket(int fd, int flag);
    int add_input_fd(int fd);
    int del_input_fd(int fd);
    int create_handle(int fd, int flag);
    int insert_handle(int fd,SocketHandle * hdle);
    SocketHandle *get_handle(int fd);
    int release_handle(SocketHandle* hdle);
    int recv_request(int fd);
    int GetInfo(std::string,int &);
    int read_http_header_timeout(SocketHandle *hdle, std::string &http, timeval *timeout);
    int readn_timeout(SocketHandle * hdle, std::string &content, int buf_len, timeval *timeout);
    int read_data(int fd, void * buf, int buf_len, timeval * timeout);
    int send_result(SocketHandle *hdle,const void *buf, int buf_len);        
    int generate_http_header(std::string & header, int length);
    int writen_timeout(int fd, const void *buf, int buf_len, timeval *timeout);

protected:
    Worker_Loader *m_workerloader;
    // Mem_Cache* m_cache;
private:
    int _epoll_fd;    
    int _pipe_read;
    int _pipe_write;
    int _socket_server_listen;
    int _epoll_ready_event_num;
    pthread_mutex_t _epoll_mutex;
    pthread_mutex_t _map_mutex;
    int _stop_task;
    int _now_connections;
    pthread_mutex_t _connections_mutex;
    epoll_event _epoll_ready_event[MAX_EPOLL_EVENT_NUM];

    std::map<int, SocketHandle *> m_fd_set;    
	
};


#endif //QO_HTTP_SERVER_HPP

