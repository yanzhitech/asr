#ifndef _WORKER_LOADER_H_
#define _WORKER_LOADER_H_

#include "task_base.hpp"
#include "worker.hpp"
#include "wait_list.hpp"
#include <vector>
#include <string>
#include "configuration.hpp"

class Http_Server;

class Worker_Loader : public task_base
{
public:
	Worker_Loader();
	~Worker_Loader();

	void register_httpserver(Http_Server* http_server);
	void set_config();
	int submit_worker(Worker*& worker);

	int open(size_t thread_num, size_t stack_size);
	int stop();	
	int svc();
private:
	int parse_content(const char *key, Worker* worker, std::string& result);
	int parse_worker(Worker * worker);
private:
	Http_Server*                                  m_httpserver;
	wait_list_t<Worker, &Worker::task_list_node>  m_task_list;
	Configuration* config;
};


#endif 


