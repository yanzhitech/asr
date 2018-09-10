#include "util_common.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include<time.h>
#include "glog/logging.h"

using namespace std;
using namespace Json;
static inline char unhex(unsigned char c)
{
    if (c < '@')
	return c - '0';
    return (c & 0x0f) + 9;
}

//static int min3Value(int a, int b, int c)
//{
//	int tmp = (a <= b? a:b);
//	return (tmp<=c? tmp: c);
//}
int string_gbk2utf8(string &ori_str,string &result)
{
    char buf[10240];
    memset((void *)buf,0,10240);
    int in_len=ori_str.size();
    int out_len=sizeof(buf);
    int ret =  gbk2utf8(ori_str.c_str(), &in_len, buf, &out_len);
    if(ret>0)
    {
	//// _INFO("line1=[%s]\n",buf);
    }else{
	// _INFO("ret=%d\n",ret);
	return 1;
    }
    result = buf;
    return 0;
}
int string_utf82gbk(string &ori_str,string &result)
{
    char buf[10240];
    memset((void *)buf,0,10240);
    int in_len=ori_str.size();
    int out_len=sizeof(buf);
    int ret =  utf82gbk(ori_str.c_str(), &in_len, buf, &out_len);
    if(ret>0)
    {
	//// _INFO("line1=[%s]\n",buf);
    }else{
	// _INFO("ret=%d\n",ret);
	return 1;
    }
    result = buf;
    return 0;
}

int utf162gbk(const char * in, int * inlen,char * out, int * outlen)
{
    if ((out == NULL) || (outlen == NULL) || (inlen == NULL)) 
	return -1;

    if (in == NULL) 
    {
	*outlen = 0; 
	*inlen = 0; 
	return 0;
    } 

    char * outbuf = (char *)out;
    char * inbuf = (char *)in;
    size_t outlenbuf = *outlen;
    size_t inlenbuf = *inlen;
    size_t ret = U162GConverter::instance()->conv(&inbuf, &inlenbuf, &outbuf, &outlenbuf);

    if(ret == size_t(-1) && errno == EILSEQ) 
    {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	//_ERROR("utf162gbk error(-2),input:%s,inlen:%d",in,*inlen);
	return -2;
    } else {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	if(ret == size_t(-1) && errno == E2BIG)
	{
	    //_ERROR("utf162gbk error(-1),input:%s,inlen:%d",in,*inlen);
	    return -1;
	}
	else if(ret == size_t(-1))
	{
	    //_ERROR("utf162gbk error(-3),input:%s,inlen:%d",in,*inlen);
	    return -3;
	}
	else
	    return *outlen;
    }
}

int gbk2utf16(const char * in, int * inlen,char * out, int * outlen)
{
    if ((out == NULL) || (outlen == NULL) || (inlen == NULL)) 
	return -1;

    if (in == NULL) 
    {
	*outlen = 0; 
	*inlen = 0; 
	return 0;
    } 

    char * outbuf = (char *)out;
    char * inbuf = (char *)in;
    size_t outlenbuf = *outlen;
    size_t inlenbuf = *inlen;
    size_t ret = G2U16Converter::instance()->conv(&inbuf, &inlenbuf, &outbuf, &outlenbuf);

    if(ret == size_t(-1) && errno == EILSEQ) 
    {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	//_ERROR("gbk2utf16 error(-2),input:%s,inlen:%d",in,*inlen);
	return -2;
    } else {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	if(ret == size_t(-1) && errno == E2BIG)
	{
	    //_ERROR("gbk2utf16 error(-1),input:%s,inlen:%d",in,*inlen);
	    return -1;
	}
	else if(ret == size_t(-1))
	{
	    //_ERROR("gbk2utf16 error(-3),input:%s,inlen:%d",in,*inlen);
	    return -3;
	}
	else
	    return *outlen;
    }
}

int utf82gbk(const char * in, int * inlen,char * out, int * outlen)
{
    if ((out == NULL) || (outlen == NULL) || (inlen == NULL)) 
	return -1;

    if (in == NULL) 
    {
	*outlen = 0; 
	*inlen = 0; 
	return 0;
    } 

    char * outbuf = (char *)out;
    char * inbuf = (char *)in;
    size_t outlenbuf = *outlen;
    size_t inlenbuf = *inlen;
    size_t ret = U82GConverter::instance()->conv(&inbuf, &inlenbuf, &outbuf, &outlenbuf);

    if(ret == size_t(-1) && errno == EILSEQ) 
    {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	//_ERROR("utf82gbk error(-2),input:%s,inlen:%d",in,*inlen);
	//fprintf(stderr,"errno=%s\n",strerror(errno));
	return -2;
    } else {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	if(ret == size_t(-1) && errno == E2BIG)
	{
	    //_ERROR("utf82gbk error(-1),input:%s,inlen:%d",in,*inlen);
	    return -1;
	}
	else if(ret == size_t(-1))
	{
	    //_ERROR("utf82gbk error(-3),input:%s,inlen:%d",in,*inlen);
	    return -3;
	}
	else
	    return *outlen;
    }
}

int gbk2utf8(const char * in, int * inlen,char * out, int * outlen)
{
    if ((out == NULL) || (outlen == NULL) || (inlen == NULL)) 
	return -1;

    if (in == NULL) 
    {
	*outlen = 0; 
	*inlen = 0; 
	return 0;
    } 

    char * outbuf = (char *)out;
    char * inbuf = (char *)in;
    size_t outlenbuf = *outlen;
    size_t inlenbuf = *inlen;
    iconv_t m_utf162gbk_ = iconv_open("utf-8//IGNORE", "GB18030//IGNORE");
    size_t ret = iconv(m_utf162gbk_, &inbuf, &inlenbuf, &outbuf, &outlenbuf);
    iconv_close(m_utf162gbk_);

    if(ret == size_t(-1) && errno == EILSEQ) 
    {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	//_ERROR("gbk2utf8 error(-2),input:%s,inlen:%d",in,*inlen);
	return -2;
    } else {
	*outlen = outbuf - out;
	*inlen = inbuf - in;
	if(ret == size_t(-1) && errno == E2BIG)
	{
	    //_ERROR("gbk2utf8 error(-1),input:%s,inlen:%d",in,*inlen);
	    return -1;
	}
	else if(ret == size_t(-1))
	{
	    //_ERROR("gbk2utf8 error(-3),input:%s,inlen:%d",in,*inlen);
	    return -3;
	}
	else
	    return *outlen;
    }
}
int url_utf16_decode(const char *url, const int url_len, char *dst, const int max_len)
{
    const char* url_end = url+url_len;
    const char* dst_end = dst+max_len-1;
    char * dst_uri = dst;
    while ( url < url_end && dst_uri < dst_end)
    {
	if(*url == '+')
	{
	    *dst_uri = ' ';
	    url++;
	}
	else
	    if (url[0] == '%' && url + 2 <= url_end && isxdigit(url[1]) && isxdigit(url[2]))
	    {
		// 以%开始的串转化为二进制
		*dst_uri = (unhex(url[1]) << 4) | unhex(url[2]);
		url += 3;
	    }
	// 其它直接拷贝
	    else
	    {
		*dst_uri = *url;
		url++;
	    }
	dst_uri++;
    }
    *dst_uri = 0;
    return dst_uri - dst;
}
int url_utf8_decode(const char *url, const int url_len, char *dst, const int max_len)
{
    //change by linan: not convert to gbk
    const char* url_end = url+url_len;
    const char* dst_end = dst+max_len-1;
    char * dst_uri = dst;
    while ( url < url_end && dst_uri < dst_end)
    {
	// 以%开始的串转化为二进制
	if(*url == '+')
	{
	    *dst_uri = ' ';
	    url++;
	}
	else
	    if (url[0] == '%' && url + 2 <= url_end && isxdigit(url[1]) && isxdigit(url[2]))
	    {
		*dst_uri = (unhex(url[1]) << 4) | unhex(url[2]);
		url += 3;
	    }
	// 其它直接拷贝
	    else
	    {
		*dst_uri = *url;
		url++;
	    }
	dst_uri++;
    }
    *dst_uri = 0;
    return dst_uri - dst;
}

vector<string> util_common::split(const string & src, string delimit)
{
    string null_subst = delimit;

    vector<string> v;

    if (src.empty() || delimit.empty())
    {
	// _DEBUG("split:empty string\n");
	return v;
    }

    size_t deli_len = delimit.size();

    size_t index = string::npos, last_search_position = 0;

    while ((index = src.find(delimit, last_search_position)) != string::npos)
    {
	if (index == last_search_position)
	{
	    if(index != src.size() - 1)
	    {
		v.push_back(null_subst);
	    }
	}
	else
	{
	    v.push_back(src.substr(last_search_position, index - last_search_position));
	}
	last_search_position = index + deli_len;
    }
    string last_one = src.substr(last_search_position);

    if(!last_one.empty() && last_one != "\r" && last_one != "\n")
    {
	v.push_back(last_one);
    }
    return v;
}

bool util_common::split(const std::string &src, std::string delimit, std::string &key, std::string &value)
{
    key.clear();
    value.clear();
    vector<string> parts = split(src,delimit);
    if(parts.size() < 2)
    {
	return false;	
    }
    key = parts[0];
    value = parts[1];
    return true;
}


vector<string> util_common::split(const string& s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> elems;
    elems.reserve(16);
    while (!ss.eof())
    {
	std::getline(ss, item, delim);
	elems.push_back(item);
    }
    return elems;
}
void RET_ILSEQ() {
    cout << "WRONG FROM OF THE SEQUENCE" << endl;
    exit(1);
}

void RET_TOOFEW() {
    cout << "MISSING FROM THE SEQUENCE" << endl;
    exit(1);
}
std::string util_common::substr_utf8(const std::string & src, size_t len)
{
    string sin = src;
    int l = sin.length();
    vector<string> ret;
    ret.clear();
    for(int p = 0; p < l; ) {
	int size, n = l - p;
	unsigned char c = sin[p], cc = sin[p + 1];
	if(c < 0x80)
	{
	    size = 1;
	} else if(c < 0xc2)
	{
	    RET_ILSEQ();
	} else if(c < 0xe0)
	{
	    if(n < 2)
	    {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40))
	    {
		RET_ILSEQ();
	    }
	    size = 2;
	} else if(c < 0xf0) {
	    if(n < 3) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(c >= 0xe1 || cc >= 0xa0)))
	    {
		RET_ILSEQ();
	    }
	    size = 3;
	} else if(c < 0xf8) {
	    if(n < 4) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(sin[p + 3] ^ 0x80) < 0x40 &&
			(c >= 0xf1 || cc >= 0x90)))
	    {
		RET_ILSEQ();
	    }
	    size = 4;
	} else if (c < 0xfc) {
	    if(n < 5) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(sin[p + 3] ^ 0x80) < 0x40 &&
			(sin[p + 4] ^ 0x80) < 0x40 &&
			(c >= 0xfd || cc >= 0x88))) {
		RET_ILSEQ();
	    }
	    size = 5;
	} else if (c < 0xfe) {
	    if(n < 6) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(sin[p + 3] ^ 0x80) < 0x40 &&
			(sin[p + 4] ^ 0x80) < 0x40 &&
			(sin[p + 5] ^ 0x80) < 0x40 &&
			(c >= 0xfd || cc >= 0x84))) {
		RET_ILSEQ();
	    }
	    size = 6;
	} else {
	    RET_ILSEQ();
	}
	string temp = "";
	temp = sin.substr(p, size);
	ret.push_back(temp);
	p += size;
    }
    string rlt = "";
    for(size_t i = 0; i < len && i < ret.size(); i++)
    {
	rlt += ret[i];
    }
    return rlt;
}
bool util_common::is_english_string(const string & str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
	if (static_cast<unsigned int>(str[i]) >= 128)
	{
	    return false;
	}
    }
    return true;
}
vector<string> util_common::ngram(const string & str_r)
{
    string tmp("");
    set<string> ngram,del_list;
    del_list.clear();
    vector<string>vec_ngram;
    ngram.clear();
    char space(' ');
    string str=string_convert(str_r);
    for (size_t i = 0; i < str.length();)
    {
	int word_len = 1;
	tmp.clear();
	if(str[i] == space)
	{
	    i++;
	    continue;	
	}
	for (size_t j = i; j < str.length();)
	{
	    if(str[j] == space)
	    {
		break;
	    }
	    else
		if(word_len>5 && !is_english_string(tmp))
		{
		    break;
		}
		else
		    if (static_cast<unsigned int>(str[j]) >= 128)
		    {
			tmp+=str.substr(j,2);
			if(word_len>=2 && (word_len<=5|| is_english_string(tmp)) && ngram.find(tmp)==ngram.end())
			{
			    ngram.insert(tmp);
			    vec_ngram.push_back(tmp);
			}
			j+=2;
			word_len++;
		    }
		    else
		    {
			tmp+=str.substr(j,1);
			if(word_len>=2 && (word_len<=5 || is_english_string(tmp)) && ngram.find(tmp)==ngram.end())
			{
			    ngram.insert(tmp);
			    vec_ngram.push_back(tmp);
			}
			j++;
			word_len++;
		    }
	}
	if (static_cast<unsigned int>(str[i]) >= 128 )
	{
	    i+=2;
	}
	else
	{
	    i++;
	}
    }
    /*	for(size_t i=0; i<vec_ngram.size(); ++i)
	{
	string tmp=vec_ngram[i];
    // _INFO("n:%s",tmp.c_str());
    for(size_t j=i+1; j<vec_ngram.size();++j)
    {
    tmp=vec_ngram[i]+vec_ngram[j];
    if(ngram.find(tmp)!=ngram.end())
    {
    del_list.insert(tmp);
    }
    }
    }
     */
    vector<string> ngram_result;
    ngram_result.clear();
    for(size_t i=0; i<vec_ngram.size(); ++i)
    {
	if(del_list.find(vec_ngram[i])==del_list.end())
	{
	    ngram_result.push_back(vec_ngram[i]);
	}
    }

    return ngram_result;
}

IplocationMapPositon::IplocationMapPositon()
{
    m_iplocation_to_Positon = new std::tr1::unordered_map<std::string, struct IPLocation >;
    pthread_mutex_init(&mutex,NULL);
}
IplocationMapPositon::~IplocationMapPositon()
{
    delete m_iplocation_to_Positon;
    pthread_mutex_destroy(&mutex);
}

int IplocationMapPositon::init(const char *filename)
{
    std::ifstream ifs(filename);

    if (!ifs)
    {
	//_ERROR("fail to open %s\n" , filename);
	return -1;
    }

    // _INFO("open file: %s success\n" , filename);
    std::string line, ip, nat, pro, cap , cit;

    pthread_mutex_lock(&mutex);
    m_iplocation_to_Positon->clear();
    char buf[1024];
    int in_len;
    int out_len=sizeof(buf);
    while(getline(ifs, line))
    {
	in_len=line.size();
	out_len=sizeof(buf);
	if(gbk2utf8(line.c_str(),&in_len,buf,&out_len)<=0)
	{
	    //_ERROR("gbk2utf8 failed\n");
	    continue;
	}else{
	    //printf("gbk2utf8 ok\n");
	    //exit(0);
	}
	line=buf;

	struct IPLocation iplocation;
	std::istringstream is(line);
	if(is >> ip >> cit >> cap >> pro >> nat)
	{
	    char buf[256]={'\xe5','\xb8','\x82','\0'};      //utf8 "市"
	    size_t pos = cit.find(buf);
	    if(pos!=string::npos)
	    {
		cit.replace(pos,strlen(buf),"");
	    }

	    m_iplocation_to_Positon->insert(std::make_pair(ip,IPLocation(nat, pro, cit, "", cap)));
	}else
	{
	    //_ERROR("file:%s format error line is:%s\n" , filename, line.c_str());
	}
    }
    pthread_mutex_unlock(&mutex);

    ifs.close();
    return 0;
}

void IplocationMapPositon::get_location(const std::string &ip, struct IPLocation &Location)
{
    Location.reset();
    if(m_iplocation_to_Positon)
    {
	if(m_iplocation_to_Positon->find(ip) != m_iplocation_to_Positon->end())
	{
	    Location = (*m_iplocation_to_Positon)[ip];
	}
    }

    return ;
}
string string_convert(const string & str_src)
{
    //处理无法转换的特殊字符
    char a1 = 0x81;
    char a2 = 0x36;
    char a3 = 0xa6;
    char a4 = 0x31;
    string src("");
    for (size_t i = 0; i < str_src.length(); )
    {
	if (i + 4 < str_src.length())
	{
	    if (str_src[i] == a1 && str_src[i + 1] == a2 && str_src[i + 2] == a3 && str_src[i + 3] == a4)
	    {
		src.append("·");
		i += 4;
	    }
	    else
	    {
		src.append(&str_src[i], 1);
		i += 1;
	    }
	}
	else
	{
	    src.append(&str_src[i], 1);
	    i += 1;
	}
    }
    string srchalf("");
    for (size_t i = 0; i < src.length(); )
    {
	if(src[i]>='A' && src[i]<='Z')
	{
	    char tmp = src[i]+32;
	    srchalf.append(&tmp,1);
	}
	else
	{
	    srchalf.append(&src[i],1);
	}
	i+=1;
    }
    if (srchalf != str_src)
    {
	// _DEBUG("string_convert\t%s\t%s\n",str_src.c_str(), srchalf.c_str());
    }

    return srchalf;
}

string mk_cdata(const char* data)
{
    string result;
    stringstream ss(result);
    ss<<"<![CDATA["<<data<<"]]>";
    result = ss.str();
    return result;
}
string mk_cdata(const string data)
{
    return mk_cdata(data.c_str());
}
string mk_cdata(const int data)
{
    string result;
    stringstream ss(result);
    ss<<"<![CDATA["<<data<<"]]>";
    result = ss.str();
    return result;
}

int min(int lst, int rst)
{
    if(lst < rst)
    {
	return lst;
    }
    return rst;
}

int int2string(const int & ori_int, string &str)
{
    str="";
    stringstream ss;
    ss << ori_int;
    str= ss.str();
    return 0;
}
string int2string(const int & ori_int)
{
    string str="";
    stringstream ss;
    ss << ori_int;
    str= ss.str();
    return str;
}
void red_replace(string &ori,const string &replace_begin,const string &replace_end,string &result)
{
    string result_red("");
    int a=0xfffffffd;
    int b=0xffffffa1;
    int c=0xffffffa2;
    for (size_t i = 0; i < ori.length();)
    {
	if (static_cast<unsigned int>(ori[i]) >= 128)
	{
	    if(a==(int)ori[i] && (int)ori[i+1]==b)
	    {
		result_red+=replace_begin;
	    }
	    else
		if(a==(int)ori[i] && (int)ori[i+1]==c)
		{
		    result_red+=replace_end;
		}
		else
		{
		    result_red+=ori.substr(i,2);
		}
	    i+=2;
	}
	else
	{
	    result_red+=ori.substr(i,1);
	    i++;
	}
    }
    result=result_red;
}

//将str中的lst替换成rst
std::string &replace(std::string &str, std::string lst, std::string rst)
{
    while(str.find(lst) != string::npos)
    {
	str = str.replace(str.find(lst), lst.size(), rst);
    }
    return str;
}

std::string& trim(std::string &s)   
{  
    if (s.empty())   
    {  
	return s;  
    }  
    s.erase(0, s.find_first_not_of(" \t\n\r"));  
    s.erase(s.find_last_not_of(" \t\n\r") + 1);  
    return s;  
}
string get_json_string(const Value& root, const string &path, const string &def)                          
{
    Path p(path);
    Value defValue(stringValue);
    defValue = def;
    const Json::Value &ref = p.resolve(root, defValue);
    if (ref.isObject()||ref.isArray())
    {
	return "";
    }
    return ref.asString();
}
int get_json_int(const Value& root, const string &path, int def)
{
    Path p(path);
    Value defValue(intValue);
    defValue = def;
    const Json::Value &ref = p.resolve(root, defValue);
    if (ref.isInt())
    {
	return ref.asInt();
    }
    return def;
}
bool get_json_bool(const Value& root, const string &path, bool boolDef)
{
    Path p(path);
    Value defValue(booleanValue);
    defValue = boolDef;
    const Json::Value &ref = p.resolve(root, defValue);
    if (ref.isObject()||ref.isArray())
    {
	return boolDef;
    }
    return ref.asBool();
}
void get_json_value(const Value& root, const string &path, Value &value)
{
    Path p(path);
    value = p.resolve(root, value);
}


void format_json_string(const string &src, string &rlt)
{
    map<string, string> replacewords;
    replacewords["\n"] = "\\\n";
    replacewords["\""] = "\\\"";
    replacewords["\'"] = "\\\'";
    replacewords["\\"] = "\\\\";
    //theString = theString.Replace(">", "&gt;");  
    //theString = theString.Replace("<", "&lt;");  
    //theString = theString.Replace(" ", "&nbsp;");  
    //theString = theString.Replace("\"", "&quot;");  
    //theString = theString.Replace("\'", "&#39;");  
    //theString = theString.Replace("\\", "\\\\");//对斜线的转义  
    //theString = theString.Replace("\n", "\\n");  
    //theString = theString.Replace("\r", "\\r"); 
    string sin = src;
    int l = sin.length();
    vector<string> ret;
    ret.clear();
    for(int p = 0; p < l; ) 
    {
	int size = 0, n = l - p;
	unsigned char c = sin[p], cc = sin[p + 1];
	if(c < 0x80)
	{
	    size = 1;
	} else if(c < 0xc2)
	{
	    RET_ILSEQ();
	} else if(c < 0xe0)
	{
	    if(n < 2)
	    {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40))
	    {
		RET_ILSEQ();
	    }
	    size = 2;
	} else if(c < 0xf0) {
	    if(n < 3) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(c >= 0xe1 || cc >= 0xa0)))
	    {
		RET_ILSEQ();
	    }
	    size = 3;
	} else if(c < 0xf8) {
	    if(n < 4) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(sin[p + 3] ^ 0x80) < 0x40 &&
			(c >= 0xf1 || cc >= 0x90)))
	    {
		RET_ILSEQ();
	    }
	    size = 4;
	} else if (c < 0xfc) {
	    if(n < 5) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(sin[p + 3] ^ 0x80) < 0x40 &&
			(sin[p + 4] ^ 0x80) < 0x40 &&
			(c >= 0xfd || cc >= 0x88))) {
		RET_ILSEQ();
	    }
	    size = 5;
	} else if (c < 0xfe) 
	{
	    if(n < 6) {
		RET_TOOFEW();
	    }
	    if(!((sin[p + 1] ^ 0x80) < 0x40 &&
			(sin[p + 2] ^ 0x80) < 0x40 &&
			(sin[p + 3] ^ 0x80) < 0x40 &&
			(sin[p + 4] ^ 0x80) < 0x40 &&
			(sin[p + 5] ^ 0x80) < 0x40 &&
			(c >= 0xfd || cc >= 0x84))) {
		RET_ILSEQ();
	    }
	    size = 6;
	} else 
	{
	    RET_ILSEQ();
	}
	string temp = "";
	temp = sin.substr(p, size);
	ret.push_back(temp);
	p += size;
    }
    for(size_t i = 0; i < ret.size(); i++)
    {

	if(replacewords.count(ret[i]))
	{
	    string rst = replacewords[ ret[i] ];
	    rlt += rst;
	    continue;
	}
	rlt += ret[i];
    }
}

int random(int max)
{
    int key = (int)time(NULL) + rand()%1000;
    srand(key);
    int num = rand()%max;
    return num;
}

U82GConverter::U82GConverter()
{
    cd_ = iconv_open("GB18030//IGNORE", "UTF-8");
    if(cd_ == (iconv_t)(-1))
    {
	LOG(ERROR) << "open failed U82GConverter iconv";
    }
}

U82GConverter::~U82GConverter()
{
    if(cd_ != (iconv_t)(-1))
	iconv_close(cd_);
}

U82GConverter * U82GConverter::instance()
{
    return ACE_TSS_Singleton<U82GConverter, ACE_Thread_Mutex>::instance();
}

size_t U82GConverter::conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft)
{
    iconv(cd_,NULL, NULL, NULL, NULL);
    return iconv(cd_, inbuf, inbytesleft, outbuf, outbytesleft);
}

U16LE2GConverter::U16LE2GConverter()
{
    cd_ = iconv_open("utf-16LE//IGNORE", "GB18030");
    if(cd_ == (iconv_t)(-1))
	LOG(ERROR) << "open failed U16LE2GConverter iconv";
}
U16LE2GConverter::~U16LE2GConverter()
{
    if(cd_ != (iconv_t)(-1))
	iconv_close(cd_);
}

U16LE2GConverter* U16LE2GConverter::instance()
{
    return ACE_TSS_Singleton<U16LE2GConverter, ACE_Thread_Mutex>::instance();
}

size_t U16LE2GConverter::conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft)
{
    iconv(cd_,NULL, NULL, NULL, NULL);
    return iconv(cd_, inbuf, inbytesleft, outbuf, outbytesleft);
}

G2U16Converter::G2U16Converter()
{
    cd_ = iconv_open("UTF-16//IGNORE", "GB18030");
    if(cd_ == (iconv_t)(-1))
	LOG(ERROR) << "open failed G2U16Converter iconv";
}
G2U16Converter::~G2U16Converter()
{
    if(cd_ != (iconv_t)(-1))
	iconv_close(cd_);
}

G2U16Converter * G2U16Converter::instance()
{
    return ACE_TSS_Singleton<G2U16Converter, ACE_Thread_Mutex>::instance();
}

size_t G2U16Converter::conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft)
{
    iconv(cd_,NULL, NULL, NULL, NULL);
    return iconv(cd_, inbuf, inbytesleft, outbuf, outbytesleft);
}

U162GConverter::U162GConverter()
{
    cd_ = iconv_open("GB18030//IGNORE", "UTF-16LE");
    if(cd_ == (iconv_t)(-1))
	LOG(ERROR) << "open failed U162GConverter iconv";
}

U162GConverter::~U162GConverter()
{
    if(cd_ != (iconv_t)(-1))
	iconv_close(cd_);
}

U162GConverter * U162GConverter::instance()
{
    return ACE_TSS_Singleton<U162GConverter, ACE_Thread_Mutex>::instance();
}

size_t U162GConverter::conv(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft)
{
    iconv(cd_,NULL, NULL, NULL, NULL);
    return iconv(cd_, inbuf, inbytesleft, outbuf, outbytesleft);
}



