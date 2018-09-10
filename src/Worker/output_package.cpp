
#include "output_package.h"
#include "util_common.h"
#include "configuration.hpp"

void Output_Package::mk_rlt(Worker* worker)
{
}

void Output_Package::toJson(Worker* worker)
{
}


void Output_Package::toXML(Worker* worker)
{
	string xml="<?xml version=\"1.0\" encoding=\"utf8\"?><DOCUMENT>";
    mk_rlt(worker);
	xml+="</DOCUMENT>";
    if(xml.size() < MAX_HTTP_CONTENT_LENGTH)
    {
	    strcpy(worker->result, xml.c_str());
    }else
    {
		LOG(ERROR) << "xml size " << xml.size() << " > " << MAX_HTTP_CONTENT_LENGTH;
		xml="<?xml version=\"1.0\" encoding=\"utf8\"?><DOCUMENT></DOCUMENT>";
		strcpy(worker->result,xml.c_str());
    }
	worker->result_len=strlen(worker->result);
}
