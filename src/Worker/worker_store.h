#ifndef _WORKER_STORE_H_
#define _WORKER_STORE_H_

#include <string>
#include <sys/types.h>
#include "request_type.h"
#include <sys/time.h>
#include <vector>
#include <map>
#include "common/json/json.h"


enum {CREATE_TIME,BEGIN_TIME,QUERY_TIME,BACK_TIME,END_TIME,TIME_COUNT};
class Worker;

class Store
{
    public:
        std::string request_type_str;
        std::map<std::string, std::string> deviceCxt;
        std::map<std::string, std::string> request;

    public:
        Store();
        virtual ~Store();
        void Init(const Worker * worker);
};

#endif
