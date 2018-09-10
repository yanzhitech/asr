#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <iostream>
#include <string>
#include "worker.hpp"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

class Output_Package
{
public:
    Output_Package(){};
    virtual ~Output_Package(){};	
    static void toXML(Worker* worker);
    static void toJson(Worker* worker);
private:
    static void mk_rlt(Worker* worker);
};

#endif
