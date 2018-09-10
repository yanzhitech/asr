#include "worker_store.h"
#include "worker.hpp"

using namespace std;

Store::Store()
:news_status(-1),news_content_length(-1)
{
	gettimeofday(&t_stat[CREATE_TIME],NULL);
}

Store::~Store(){
}

void Store::Init(const Worker * worker)
{
	if (worker != NULL)
	{
		url = worker->url;
		title = worker->title;
	}
}

