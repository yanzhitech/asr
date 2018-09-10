#include "worker_loader.h"
#include "http_server.hpp"
//#include "service_log.hpp"
#include <errno.h>
#include <iconv.h>
#include <string>
#include "util_common.h"
#include "common/json/json.h"
#include "glog/logging.h"

Worker_Loader::Worker_Loader() {
}

Worker_Loader::~Worker_Loader() {
}

void Worker_Loader::register_httpserver(Http_Server * server) {
    m_httpserver = server;
}

void Worker_Loader::set_config() {
    this->config = Configuration::getInstance();
}

int Worker_Loader::submit_worker(Worker* &worker) {
    m_task_list.put(*worker);
    worker = NULL;
    return 0;	
}
int Worker_Loader::open(size_t thread_num, size_t stack_size) {

    return task_base::open(thread_num, stack_size);
}

int Worker_Loader::stop() {
    m_task_list.flush();
    join();
    return 0;
}

int Worker_Loader::parse_content(const char *key, Worker* worker, std::string& result) {
    char buf[8192];
    result.clear();
    std::string request = worker->request;
    int key_start = request.find(key);
    if(key_start < 0 ) {
        LOG(ERROR) << "can't find the key symbol(" << key << ")";
        return -1;
    }
    key_start += strlen(key);
    int key_end;
    if((key_end= request.find("&",key_start)) <0 ) {
        key_end = request.length();
    }
    int key_len = key_end - key_start;	
    int ret = -1;
    if(worker->source == Worker::DEFAULT_SOURCE)
        ret = url_utf8_decode(request.c_str()+key_start, key_len, buf, sizeof(buf)-1);
    else
        ret = url_utf16_decode(request.c_str()+key_start, key_len, buf, sizeof(buf)-1);
    if(ret > 0 && (size_t)ret <= sizeof(buf)-1) {
        buf[ret] = 0;
        result = buf;
        return 0;
    }
    return -1;
}

int Worker_Loader::parse_worker(Worker * worker) {
    LOG(INFO) << "log-request-encode: " << worker->request;
    char buf[2048];
    memset(buf, 0, sizeof(char)*2048);
    if(url_utf8_decode(worker->request.c_str(), worker->request.size(), buf, 2048)) {
        LOG(INFO) << "request decode: " << buf;
        worker->request = buf;
        string request = worker->request;
        request = replace(request, "\n" , " ");
        LOG(INFO) << "log-request: " << request;
    }
    /////new api////////////////////////////
    std::string encode = "";
    std::string version = "";
    std::string type = "";
    std::string requestText = "";
    std::string deviceCxt = "";
    parse_content("encode=", worker, encode);
    parse_content("type=", worker, type);
    parse_content("deviceCxt=", worker, deviceCxt);
    parse_content("request=", worker, requestText);
    if(type.size() > 0) {
        worker->request_type_str = type;
    }
    if(deviceCxt.size() > 0) {
        Json::Reader reader;
        Json::Value res;
        if(reader.parse(deviceCxt, res) and res.isObject()) {
            Json::Value::Members member = res.getMemberNames();
            for(Json::Value::Members::iterator iter = member.begin(); iter != member.end(); ++iter) {
                worker->store->deviceCxt[*iter] = get_json_string(res, *iter);
            }
            worker->store->appid = get_json_string(res, "appId");
            worker->store->userid = get_json_string(res, "userId");
            version = get_json_string(res, "appVersion");
        }
    }
    if(requestText.size() > 0) {
        Json::Reader reader;
        Json::Value res;
        if(reader.parse(requestText, res) && res.isObject()) {
            Json::Value::Members member = res.getMemberNames();
            for(Json::Value::Members::iterator iter = member.begin(); iter != member.end(); ++iter) {
                worker->store->request[*iter] = get_json_string(res, *iter);
                LOG(INFO) << *iter << ", " << worker->store->request[*iter];
            }
        }
    }
    /////////////////////////////////////////
    string forcequery;
    parse_content("forcequery=", worker, forcequery);
    if(forcequery == "true") {
        worker->force_query=true;
    }
    else{
        worker->force_query=false;	
    }
    return 0;
}

int Worker_Loader::svc()
{
    Worker * worker;

    int ret = -1;
    while ((worker = m_task_list.get()) != NULL)
    {	
        ret = parse_worker(worker);
        //////////////////////////
        if(ret < 0)
        {
            m_httpserver->retrieve_worker(worker);
            continue;
        }
        //////////////////////////
        // Worker_Cache::get(worker->store);
        ///////NLU PROCESS////////
        // m_qo->proc(worker);
        //////////////////////////
        // string cache_key = worker->get_cache_key();
        // bool hit_cache = false;
        if(worker==NULL)
        {
            LOG(ERROR) << "RequestFactory: worker is NULL";
            m_httpserver->retrieve_worker(worker);
            continue;
        }
        if(config==NULL)
        {
            LOG(ERROR) << "RequestFactory: config is NULL";
            m_httpserver->retrieve_worker(worker);
            continue;
        }
        // RequestBase* request;
        // request->request();
        m_httpserver->retrieve_worker(worker);
        // delete request;
        continue;
    }
    return 0;
}
