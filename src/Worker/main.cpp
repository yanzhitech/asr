#include "http_server.hpp"
#include "worker_loader.h"
#include "configuration.hpp"
#include <malloc.h>
#include <signal.h>
// #include "connpool/conn_pool.h"
#include "glog/logging.h"  

#define DEFAULT_CONFIG_KEYNAME      "Worker"

void sigterm_handler(int signo) {}
void sigint_handler(int signo) {}


int main(int argc, char* argv[])
{
    google::InitGoogleLogging("worker_server");  //参数为自己的可执行文件名  
    google::SetLogDestination(google::GLOG_INFO,"./nls.INFO");  
    google::SetLogDestination(google::GLOG_ERROR, "./nls.ERROR");
    google::SetLogDestination(google::GLOG_WARNING, "./nls.WARNING");
    google::SetLogDestination(google::GLOG_FATAL, "./nls.FATAL");
    FLAGS_logbufsecs = 0; //缓冲日志输出，默认为30秒，此处改为立即输出
    FLAGS_max_log_size = 100; //最大日志大小为 100MB
    FLAGS_stop_logging_if_full_disk = true; //当磁盘被写满时，停止日志输出
    /////////////////////////////////////
    mallopt(M_MMAP_THRESHOLD, 64*1024);
    const char *config_filename = argv[1];
    const char *config_keyname = DEFAULT_CONFIG_KEYNAME;
    if(argc>=3) config_keyname = argv[2];	

    close(STDIN_FILENO);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, &sigterm_handler);
    signal(SIGINT, &sigint_handler);

    LOG(INFO) << "[Worker start...] \n";
    Configuration config;
    {
        int ret;
        if (config.open(config_filename, config_keyname))
        {
            LOG(ERROR) << "open configure file and key name < " << config_filename << " : " << config_keyname << "error";
            return -1;
        }
        Configuration::initializeInstance(&config);

        Http_Server htp;
        Worker_Loader worker_loader;

        worker_loader.register_httpserver(&htp);
        worker_loader.set_config();
        htp.register_worker_loader(&worker_loader);
        //		htp.register_cache(&cache);
        LOG(INFO) << "[worker start...] ";
        if((ret = worker_loader.open(config.worker_loader_num,config.thread_stack_size)) < 0)
        {
            LOG(ERROR) << "open worker loader error! ret:" << ret << ",thread:" << (int)config.worker_loader_num << ",stacksize:" << (int)config.thread_stack_size;
            exit(-1);
        }

        LOG(INFO) << "[http server start...] ";
        if((ret = htp.open(config.receiver_num,config.thread_stack_size,config.listen_port)) <0 )
        {
            LOG(ERROR) << "open http server error! ret:" << ret << ",thread:" << (int)config.receiver_num << ",listen port:" << (int)config.listen_port;
            exit(-1);	
        }
        ////////////////////////////
        ////////////////////////////
        worker_loader.activate();
        htp.activate();
        LOG(INFO) << "Worker start OK";

        pause();

        htp.stop();
        worker_loader.stop();
        //		update_timer.stop();
        //		cache.close();
    }
    LOG(INFO) << "[QueryOptimizer stop]";

    return 0;
}

