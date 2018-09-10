#ifndef  WORKER_HPP
#define  WORKER_HPP

#include "worker_store.h"
#include "socket_handle.hpp"
#include "linked_list.hpp"
#include <ace/Malloc_Base.h>
#include <string>
#include <map>
#include "request_type.h"
#include <sstream>
#define MAX_HTTP_CONTENT_LENGTH   1024*1024 

class Worker
{
public:
	Worker()
	:source(DEFAULT_SOURCE),result_len(0),hitcache(false),force_query(false),cache_timeout(0),handle(NULL)
	{
		result[0] = 0;

		pthread_mutex_init(&_mutex, NULL);
		pthread_cond_init(&_cond, NULL);
		store = new Store;
	}

	virtual ~Worker()
	{
		pthread_cond_destroy(&_cond);
		pthread_mutex_destroy(&_mutex);
		if(store)
		{
			delete store;
		}
	}
	
	
public:
	enum REQUEST_SOURCE {DEFAULT_SOURCE, PLUGIN_SOURCE};
	timeval receive_time;
	timeval loadpage_time;
	long long getpnametime;    

	std::string request;
	std::string title;
	std::string url;
	Store *store;

	REQUEST_SOURCE source;

	char result[ MAX_HTTP_CONTENT_LENGTH ];
	size_t result_len;

	string cache_key;
	bool hitcache;
	bool force_query;
	time_t cache_timeout;

	SocketHandle *handle;    
	linked_list_node_t task_list_node;

	pthread_mutex_t _mutex;
	pthread_cond_t _cond;

	RequestType request_type;
	string request_type_str;

private:
	Worker(const Worker& rhs);
	Worker& operator=(const Worker& rhs);
};

#endif //WORKER_HPP

