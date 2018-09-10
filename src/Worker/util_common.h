#ifndef _UTIL_COMMON_H_
#define _UTIL_COMMON_H_
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <string>
#include <set>
#include <vector>
#include <iconv.h>
#include <tr1/unordered_map>
#include "common/json/json.h"
#include <map>
#include <algorithm>
#include "base64.h"

using std::tr1::unordered_map;
using std::string;

void RET_ILSEQ();
void RET_TOOFEW();
int gbk2utf16(const char *in, int *inlen, char *out, int *outlen);
int gbk2utf8(const char *in, int *inlen, char *out, int *outlen);
int utf162gbk(const char *in, int *inlen, char *out, int *outlen);
int utf82gbk(const char *in, int *inlen, char *out, int *outlen);
int url_utf16_decode(const char *url, const int url_len, char *dec, const int max_len);
int url_utf8_decode(const char *url, const int url_len, char *dec, const int max_len);
//将字符串进行大写转小写-
std::string string_convert(const std::string & src);
string mk_cdata(const char* data);
string mk_cdata(const string data);
string mk_cdata(const int data);
//换标红字符
void red_replace(std::string &ori,const std::string &replace_begin,const std::string &replace_end,std::string &result); 
std::string &replace(std::string &str, std::string lst, std::string rst);
int string_utf82gbk(std::string &ori_str,std::string &result);
int string_gbk2utf8(std::string &ori_str,std::string &result);
int int2string(const int & ori_int, std::string &str);
string int2string(const int & ori_int);
std::string& trim(std::string &s);
int random(int max);
int min(int lst, int rst);
namespace util_common
{
    std::vector<std::string> split(const std::string & src, char delim);
    std::vector<std::string> split(const std::string & src, std::string delimit);
    std::vector<std::string> split_new(const std::string & str, char &delimit);
    std::vector<std::string> ngram(const std::string &str_r);
    bool split(const std::string &src, std::string delimit, std::string &key, std::string &value);
    bool is_english_string(const std::string &str);
    std::string substr_utf8(const std::string & src, size_t len);
};
class G2U16Converter
{
    public:
	G2U16Converter();
	~G2U16Converter();
	static G2U16Converter *instance();
	size_t conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
    private:
	iconv_t cd_;
};

class U162GConverter
{
    public:
	U162GConverter();
	~U162GConverter();
	static U162GConverter *instance();
	size_t conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
    private:
	iconv_t cd_;
};

class U16LE2GConverter
{
    public:
	U16LE2GConverter();
	~U16LE2GConverter();
	static U16LE2GConverter *instance();
	size_t conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
    private:
	iconv_t cd_;
};

class U82GConverter
{
    public:
	U82GConverter();
	~U82GConverter();
	static U82GConverter *instance();
	size_t conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
    private:
	iconv_t cd_;
};

typedef struct IPLocation
{
    std::string country;
    std::string province;
    std::string city;
    std::string district;
    std::string capital;//省会

    IPLocation(const std::string &cot = "",
	    const std::string &pro = "",
	    const std::string &cit = "", const std::string & dis = "", const std::string &cap = ""):
	country(cot), province(pro), city(cit), district(dis), capital(cap){}

    void reset()
    {
	country = "";
	province = "";
	capital = "";
	city = "";
	district = "";
    }
    bool empty()
    {
	return country.empty() && province.empty() && capital.empty() && city.empty() && district.empty();
    }

    bool operator<( const IPLocation & another)const
    {
	return country + province + capital + city + district < another.country + another.province + another.capital + another.city +
	    another.district;
    }
}Location;

class IplocationMapPositon
{
    public:
	IplocationMapPositon();
	~IplocationMapPositon();
	static IplocationMapPositon* instance ()
	{   
	    return ACE_Singleton<IplocationMapPositon, ACE_Null_Mutex>::instance();
	}   

	int init(const char *filename);
	void get_location(const std::string &ip, struct IPLocation &Location);

    private:
	std::tr1::unordered_map<std::string, struct IPLocation> *m_iplocation_to_Positon;
	pthread_mutex_t mutex;
};
void format_json_string(const string &src, string &rlt);

#endif

