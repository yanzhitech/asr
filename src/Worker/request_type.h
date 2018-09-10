#ifndef _REQUEST_TYPE_H__
#define _REQUEST_TYPE_H__
#include <iostream>
enum RequestType{
    TEST,
    DIALOGUE,		
    UNKNOW,
};

RequestType get_request_type(std::string str);
#endif
