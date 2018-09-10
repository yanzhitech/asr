#ifndef QUERY_CORE_SERVER_CONFIGURATION_HPP
#define QUERY_CORE_SERVER_CONFIGURATION_HPP

#include <vector>
#include <netinet/in.h>
#include <string>
#include <map>
#include "common/json/json.h"

struct SphinxServer {
    std::string host;
    int port;
    int conn_num;
};

class Configuration {
    public:
    static Configuration *getInstance();
    static bool initializeInstance(Configuration *config);
    int open(const char *filename, const char *key);
    static std::string readString(char const *filename, char const *path, char const *name);

    protected:
    int readParameter(const char *filename, const char * key);
    int readThreadNumber(const char *filename, const char * key);
    int readRequest(const char *filename, const char * key);
    int SeparateIP(const char *value,char * ip,int * port);
    private:
    static Configuration *instance;
    public:
    unsigned int listen_port;
    bool concurrent_load;
    bool whitelist_filterflag;

    size_t thread_stack_size;


    std::string result_cache_addrs;
    int cache_default_timeout;
    int network_news_timeout;

    std::string qo_whitelist_file;

    //sphinx configs
    int sphinx_server_count_normal;
    SphinxServer* sphinx_servers_normal;    
    int sphinx_server_count_inst;
    SphinxServer* sphinx_servers_inst;   

    std::string news_summary_index;

    std::string news_addr;
    std::string news_service_type;
    uint64_t news_timeout;
    std::string info_addr;
    std::string info_service_type;
    uint64_t info_timeout;

    size_t receiver_num;
    size_t worker_loader_num;
    size_t sphinx_loader_num;
    size_t preprocessor_num;
    size_t processor_num;

    std::string seg_model;
    std::string seg_ne_dict;
    std::string ner_dict;
    std::string domain_pattern;
    std::string weather_code;
    std::string stock_code;
    ///////////////////////

    std::string home_hot_exam_xml;
    std::string home_doc_xml;
    std::string home_online_video_xml;
    std::string home_online_school_xml;
    std::string home_nav_xml;
    std::string home_banner_xml;
    std::string home_hot_word_xml;
    std::string home_exam_remind_xml;
    std::string course_channel_xml;
    std::string course_cate_id;
    std::string city_list_xml;
    std::string iplocal_txt;
    std::string type_sort;
    std::string exam_channel_xml;
    std::string exam_names;
    std::string exam_query_news;
    std::string doc_ad_xml;
    std::string doc_channel_banner_xml;
    std::string doc_channel_hot_doc_xml;
    std::string doc_channel_detail_xml;
    std::string doc_choice;
    std::string doc_cateid;

    std::string home_hot_exam_index;
    unsigned int update_timer_interval;

    std::string mysql_host;
    std::string mysql_passport;
    std::string mysql_user;
    std::string mysql_db;
    unsigned int mysql_port;
    ////////////////////////
    std::string symbols;
    std::string sc_model;
    std::string trie_dict;
    std::string trie_prefix;
    std::string trie_surfix;
    uint32_t sc_bin;
    ////////////////////////
    Json::Value appconf;
public:
    std::string getAppValue(const std::string appid, const std::string key);
};

#endif /* QUERY_CORE_SERVER_CONFIGURATION_HPP */

