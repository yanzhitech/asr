#include "configuration.hpp"
#include <netdb.h>
#include "config_map.hpp"
#include "service_log.hpp"
#include <sstream>
#include <fstream>
Configuration *Configuration::instance = NULL;

Configuration *Configuration::getInstance()
{
    return instance;
}

bool Configuration::initializeInstance(Configuration *config)
{
    if(instance == NULL)
    {
        instance = config;
    }else
    {
        return false;
    }
    return true;
}

int Configuration::open(const char *filename, const char *key)
{
    if (readParameter(filename, key))
        return -1;
    if (readThreadNumber(filename, key))
        return -1;
    //if (readRequest(filename, key))
    //    return -1;
    return 0;
}

int Configuration::SeparateIP(const char * value,char * ip, int * port)
{
    const char *pos = strchr(value,':');
    if(!pos)
    {
        _ERROR("error ip address.%s\n",value);
        return -1;
    }
    *port = atoi(pos+1);
    int len = pos - value;
    memcpy(ip,value,len);
    ip[len] = '\0';
    return 0;
}

int Configuration::readParameter(const char *filename, const char * key)
{
    config_map config;
    if (config.import(filename))
        _ERROR_RETURN(-1, "[config_map::import(%s)]\n", filename);

    std::string full_key(std::string(key) + "/Parameter");
    _INFO("[readParameter]key:%s",full_key.c_str());
    config.set_section(full_key.c_str());
    const char *value;

    if (config.get_value("ListenPort", value))
    {
        _ERROR("no config ListenPort\n");
        return -1;
    }

    //listen_port = htons(atoi(value));

    char *end = NULL;
    listen_port = strtol(value, &end, 10);
    _INFO("listen_port: %s\n", value);
    if(value == end || listen_port < 0)
    {
        return -1;
    }

    if (config.get_value("threadstacksize", value))
    {
        _ERROR("no config threadstacksize,use default (16m)\n");
        thread_stack_size = 16 * (1 << 20);
    }else{
        thread_stack_size = atoi(value) * (1 << 10);
    }

    return 0;
}

int Configuration::readThreadNumber(const char *filename, const char * key)
{
    config_map config;
    if (config.import(filename))
        _ERROR_RETURN(-1, "[config_map::import(%s)]\n", filename);

    std::string full_key(std::string(key) + "/ThreadNumber");
    _INFO("[readThreadNumber]key:%s\n",full_key.c_str());
    config.set_section(full_key.c_str());
    const char *value;

    if (config.get_value("ReceiverThread", value))
    {
        _ERROR("no config ReceiverThread, use default(4) \n");
        receiver_num = 3;
    }
    else
        receiver_num = atoi(value);

    if (config.get_value("WorkerLoaderThread", value))
    {
        _ERROR("no config WorkerLoaderThread,use default(16)\n");
        worker_loader_num = 16;
    }
    else 
        worker_loader_num = atoi(value);

    return 0;
}

int Configuration::readRequest(const char *filename, const char * key)
{
    std::string value;
    value=readString(filename,"Worker/Cache","ResultCacheAddrs");
    if(value=="")
    {
        _ERROR("no config ResultCacheAddrs\n");
        return -1;
    }else{
        result_cache_addrs=value;
    }
    value=readString(filename,"Worker/Cache","DefaultTimeout");
    if(value=="")
    {
        _ERROR("no config DefaultTimeout\n");
        return -1;
    }else{
        cache_default_timeout=atoi(value.c_str());
    }
    value=readString(filename,"Worker/Cache","NetworkNewsTimeout");
    if(value=="")
    {
        _ERROR("no config NetworkNewsTimeout\n");
        return -1;
    }else{
        network_news_timeout=atoi(value.c_str());
    }


    value=readString(filename,"Worker/Sphinx/Normal","SphinxCount");
    if(value=="")
    {
        _ERROR("no config normal SphinxCount\n");
        return -1;
    }else{
        sphinx_server_count_normal=atoi(value.c_str());
        sphinx_servers_normal = new SphinxServer[sphinx_server_count_normal];
        for(int i=0; i<sphinx_server_count_normal; i++)
        {
            std::string host,port,conn_num;
            char buf[1024];
            sprintf(buf,"Sphinx_%d_host", i);
            host=readString(filename, "Worker/Sphinx/Normal", buf);
            sprintf(buf,"Sphinx_%d_port",i);
            port=readString(filename, "Worker/Sphinx/Normal", buf);
            sprintf(buf,"Sphinx_%d_conn_num",i);
            conn_num=readString(filename, "Worker/Sphinx/Normal",buf);
            if(host.empty() || port.empty() || conn_num.empty())
            {
                _ERROR("normal server %d config wrong!\n",i);
                return -1;
            }else{
                sphinx_servers_normal[i].host=host;
                sphinx_servers_normal[i].port=atoi(port.c_str());
            }	sphinx_servers_normal[i].conn_num=atoi(conn_num.c_str());
        }
    }
    value=readString(filename,"Worker/Sphinx/Inst","SphinxCount");
    if(value=="")
    {
        _ERROR("no config normal SphinxCount\n");
        return -1;
    }else{
        sphinx_server_count_inst=atoi(value.c_str());
        sphinx_servers_inst = new SphinxServer[sphinx_server_count_inst];
        for(int i=0;i<sphinx_server_count_inst;i++)
        {
            std::string host,port,conn_num;
            char buf[1024];
            sprintf(buf,"Sphinx_%d_host",i);
            host=readString(filename,"Worker/Sphinx/Inst",buf);
            sprintf(buf,"Sphinx_%d_port",i);
            port=readString(filename,"Worker/Sphinx/Inst",buf);
            sprintf(buf,"Sphinx_%d_conn_num",i);
            conn_num=readString(filename,"Worker/Sphinx/Inst",buf);
            if(host.empty() || port.empty() || conn_num.empty())
            {
                _ERROR("inst server %d config wrong!\n",i);
                return -1;
            }else{
                sphinx_servers_inst[i].host=host;
                sphinx_servers_inst[i].port=atoi(port.c_str());
            }       sphinx_servers_inst[i].conn_num=atoi(conn_num.c_str());
        }
    }

    value=readString(filename,"Worker/Request","MysqlHost");
    if(value=="")
    {
        _ERROR("no config MysqlHost\n");
        return -1;
    }else{
        mysql_host=value;
    }
    value=readString(filename,"Worker/Request","MysqlPort");
    if(value=="")
    {
        _ERROR("no config MysqlPort\n");
        return -1;
    }else{
        mysql_port=atoi(value.c_str());
    }
    value=readString(filename,"Worker/Request","MysqlPassport");
    if(value=="")
    {
        _ERROR("no config MysqlPassport\n");
        return -1;
    }else{
        mysql_passport=value;
    }
    value=readString(filename,"Worker/Request","MysqlUser");
    if(value=="")
    {
        _ERROR("no config MysqlUser\n");
        return -1;
    }else{
        mysql_user=value;
    }
    value=readString(filename,"Worker/Request","MysqlDB");
    if(value=="")
    {
        _ERROR("no config MysqlDB\n");
        return -1;
    }else{
        mysql_db=value;
    }

    value=readString(filename,"Worker/Request/Course","NewsSummIndex");
    if(value=="")
    {
        _ERROR("no config CourseIndex\n");
        return -1;
    }else{
        news_summary_index=value;
    }
    return 0;
}


std::string Configuration::readString(char const * filename, char const * path, char const * name)
{
    config_map config;
    std::string ret = "";
    const char * value;

    if (config.import(filename))
        return ret;

    config.set_section(path);
    if (config.get_value(name, value))
        return ret;

    ret = value;
    return ret;
}

std::string Configuration::getAppValue(const std::string appid, const std::string key)
{
    if(key.size() == 0 || appid.size() == 0)
    {
        return "";
    }
    Json::Value val;
    get_json_value(Configuration::getInstance()->appconf, "data", val);
    if(val.isArray())
    {
        for(int i=0; i<(int)val.size(); i++)
        {
            if(appid == get_json_string(val[i], "appId"))
            {
                return get_json_string(val[i], key);
            }
        }
    }
    return "";
}
