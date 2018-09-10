#include "http.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "service_log.hpp"
#include <sstream>

static int char2num(char ch) 
{ 
    if(ch >= '0' && ch <= '9')
        return (ch - '0'); 
    if(ch >= 'a' && ch <= 'f')
        return (ch- 'a' + 10); 
    if(ch >= 'A' && ch <= 'F')
        return (ch- 'A' + 10); 
    return -1; 
} 

void URLDecode(const std::string &input, std::string& output)
{
	output.clear();
	size_t i = 0;
	size_t len = input.size();
	int num1, num2;
	while(i < len)
	{
		char ch = input[i];
		switch(ch)
		{
		case '+':
			output.append(1, ' ');
			i++;
			break;
		case '%':
			if(i+2 < len)
			{
				num1 = char2num(input[i+1]);
				num2 = char2num(input[i+2]);
				if(num1 != -1 && num2 != -1) 
				{
					char res = (char)((num1 << 4) | num2);
					output.append(1, res);
					i += 3;
					break;
				}
			}
			//go through
		default:
			output.append(1, ch);
			i++;
			break;
		}
	}
}
void URLEncode(const char *input, size_t len, std::string& output)
{
	const unsigned char* c = (const unsigned char*)input;
	for(size_t j = 0; j < len; ++j)
	{
		char s[8] = {0};
		snprintf(s, 8, "%%%.2X", c[j]);
		s[7] = 0;
		output += s;
	}
}

std::string URLCode(const std::string &input)
{
	std::string conv_words = "";
	const unsigned char* c = (const unsigned char*)input.c_str();
	for(size_t j = 0; j < input.size(); ++j) 
	{
		char s[8] = {0};
		snprintf(s, 8, "%%%.2X", c[j]);
		s[7] = 0;
		conv_words += s;
	}  
	return conv_words;
}

ssize_t read_nReal_timeout(const ACE_SOCK_Stream& stream, void *buf, size_t len, ACE_Time_Value *timeout, size_t *bt)
{
    ssize_t n;
	size_t temp;
	size_t &bytes_transferred = bt == 0 ? temp : *bt;
	ACE_Time_Value t1, t2;
	ACE_Time_Value t;

	for (bytes_transferred = 0; bytes_transferred < len; bytes_transferred += n)
	{
		t1 = ACE_OS::gettimeofday();
		n = stream.recv((char *)buf + bytes_transferred, len - bytes_transferred, timeout);
		if(n <=0 )
		{
			return n;
		}
		t2 = ACE_OS::gettimeofday();
		if(t2 < t1)
			break;

		t = t2 - t1;
		if(t >= *timeout)
		{
			*timeout = ACE_Time_Value::zero;
			break;
		}
		*timeout -= t;
	}

	return bytes_transferred;
}

static int readHttpIs_Chunk(const ACE_SOCK_Stream& stream, ACE_Time_Value *timeout, std::string& header, const size_t header_length, std::string& result)
{
	char buf[20480];
	ACE_Time_Value t1, t2;
	ACE_Time_Value t;

	size_t pos_num = header_length;
	size_t pos_chunk_start = 0;
	do{
		pos_chunk_start = header.find("\r\n", pos_num);
		while(pos_chunk_start == std::string::npos)
		{
			t1 = ACE_OS::gettimeofday();
			ssize_t len = stream.recv(buf, sizeof(buf), timeout);
			if(len < 0){
				_ERROR("recv http response failed: %s\n", strerror(errno));
				break;
			} else if(len == 0) {
				_INFO("connection closed by peer\n");
				break;
			}

			header.append(buf, len);

			t2 = ACE_OS::gettimeofday();
			if(t2 < t1)
				break;
			t = t2 - t1;

			if(t >= *timeout)
			{
				*timeout = ACE_Time_Value::zero;
			}else
			{	
				*timeout -= t;
			}

			pos_chunk_start = header.find("\r\n", pos_num);
			if(pos_chunk_start != std::string::npos)
				break;
			if(*timeout == ACE_Time_Value::zero)
				break;
		}

		if(pos_chunk_start == std::string::npos)
			return -1;

		std::string str_size = header.substr(pos_num, pos_chunk_start-pos_num);
		std::stringstream str_stream(str_size);
		size_t chunk_size = 0;
		str_stream>>std::hex>>chunk_size;
		if(chunk_size == 0)
		{
			break;	
		}

		pos_num = pos_chunk_start + 4 + chunk_size;
		ssize_t remain = pos_num - header.size();

		if(remain > 0)
		{	
			if((size_t)remain > sizeof(buf)) 
				return -1;

			ssize_t len = read_nReal_timeout(stream, buf, remain, timeout);
			if(len < remain)
			{
				_ERROR("recv http response failed: %s\n", strerror(errno));
				return -1;
			}
			header.append(buf, len);
		}
		result += header.substr(pos_chunk_start+2, chunk_size);
	}while(true);	

	size_t last_chunk_end = header.find("\r\n\r\n", pos_chunk_start);
	while(last_chunk_end == std::string::npos)
	{
		t1 = ACE_OS::gettimeofday();
		ssize_t len = stream.recv(buf, sizeof(buf), timeout);
		if(len < 0){
			_ERROR("recv http response failed: %s\n", strerror(errno));
			break;
		} else if(len == 0) {
			_INFO("connection closed by peer\n");
			break;
		}
		header.append(buf, len);

		t2 = ACE_OS::gettimeofday();
		if(t2 < t1)
			break;
		t = t2 - t1;
		if(t >= *timeout)
		{
			*timeout = ACE_Time_Value::zero;
		}
		{
			*timeout -= t;
		}

		last_chunk_end = header.find("\r\n\r\n", pos_chunk_start);
		if(last_chunk_end != std::string::npos)
			break;
		if(*timeout == ACE_Time_Value::zero)
			break;
	}

	if(last_chunk_end == std::string::npos)
		return -1;

	return 0;
}

static int readHttpConten_Length(const ACE_SOCK_Stream& stream, ACE_Time_Value *timeout, std::string &header, const size_t header_length, std::string& result)
{
	size_t pos = header.find("Content-Length:");
	if(pos == std::string::npos || pos >= header_length)
	{
		return -1;
	}

	pos += strlen("Content-Length:");
	ssize_t content_length = atoi(&header[pos]);
	if(content_length < 0)
		return -1;
	ssize_t remain = header_length + content_length - header.size();
	if(remain < 0) 
	{
		return -1;
	}
	if(remain > 0) 
	{
		char buf[20480];
		if((size_t)remain > sizeof(buf)) 
			return -1;

		ssize_t len = read_nReal_timeout(stream, buf, remain, timeout);
		if(len == remain)
		{
			header.append(buf, len);
			_INFO("recv complete http response\n");
		} else { 
			_ERROR("recv http content length response failed: %s\n", strerror(errno));
			return -1;
		}
	} 				

	result = header.substr(header_length);
	return 0;

}

int ParseHttpContentLength(const std::string &input, ssize_t content_length)
{
	if(content_length < 0)
	{
		return -1;
	}
	if(input.size() < (size_t)content_length)
	{
		return 1;
	}else if(input.size() > (size_t)content_length)
	{
		return -1;
	}

	return 0;
}

int ParseHttpContentChunk(std::string &input, std::string& result)
{
	do{
		size_t pos_chunk_start = input.find("\r\n");
		if(pos_chunk_start == std::string::npos)
		{
			return 1;
		}

		std::string str_size = input.substr(0, pos_chunk_start);
		std::stringstream str_stream(str_size);
		size_t chunk_size = 0;
		str_stream>>std::hex>>chunk_size;
		if(chunk_size == 0)
		{
			break;	
		}

		size_t pos_num = pos_chunk_start + 4 + chunk_size;
		if(pos_num > input.size())
		{
			return 1;
		}

		result += input.substr(pos_chunk_start+2, chunk_size);
		input = input.substr(pos_num);
	}while(true);	

	size_t last_chunk_end = input.find("\r\n\r\n");
	if(last_chunk_end == std::string::npos)
	{
		return 1;
	}

	return 0;		
}

int ParseHttpHeader(std::string &input, ssize_t &content_length)
{
	content_length = -1;
	size_t header_length = 0;
	if((header_length = input.find("\r\n\r\n")) == std::string::npos)
	{
		if(input.size() > 10240)
			return -1;
		else
			return 1;
	}

	header_length += 4;
	if(header_length > 10240)
		return -1;

	size_t pos = input.find("Transfer-Encoding: chunked\r\n");
	if(pos != std::string::npos && pos < header_length)
	{
		input = input.substr(header_length);
	}else
	{
		//_INFO("Content-Length:%s\n", input.c_str());
		pos = input.find("Content-Length:");
		if(pos == std::string::npos || pos >= header_length)
		{
			return -1;
		}

		pos += strlen("Content-Length:");
		content_length = atoi(&input[pos]);
		if(content_length < 0)
			return -1;
		input = input.substr(header_length);
	}
	return 0;
}

int readHttpResponse(const ACE_SOCK_Stream& stream, ACE_Time_Value *timeout, std::string& result)
{
	int ret = -1;
	result.clear();
	result.reserve(20480);
	char buf[20480];
	ACE_Time_Value t1, t2;
	ACE_Time_Value t;
	size_t header_length = 0;
	std::string header;
	header.reserve(20480);
	while(1)
	{
		t1 = ACE_OS::gettimeofday();
		ssize_t len = stream.recv(buf, sizeof(buf), timeout);
		if(len < 0){
			_ERROR("recv http response failed: %s\n", strerror(errno));
			break;
		} else if(len == 0) {
			_INFO("connection closed by peer\n");
			break;
		}

		header.append(buf, len);

		t2 = ACE_OS::gettimeofday();
		if(t2 < t1)
			break;

		t = t2 - t1;
		if(t >= *timeout)
		{
			*timeout = ACE_Time_Value::zero;
		}else
		{
			*timeout -= t;
		}

		if((header_length = header.find("\r\n\r\n")) != std::string::npos)
		{
			header_length += 4;
			ret = 0;
			break;
		}

		if(header.size() > 4096)
			break;

	    if(*timeout == ACE_Time_Value::zero)
			break;
		
	}

	if(ret != 0)
		return -1;
	
	size_t pos = header.find("Transfer-Encoding: chunked\r\n");
	if(pos != std::string::npos && pos < header_length)
	{
		ret = readHttpIs_Chunk(stream, timeout, header, header_length, result);
	}else
	{
	    ret = readHttpConten_Length(stream, timeout, header, header_length, result);
	}
	
	if(ret == 0)
	{
		_INFO("recv http response succeed !\n");
		return 0;
	}
	return -1;
}

int add_param(std::string& content, const std::string& key, const std::string& value)
{
	std::string s;
	URLEncode(value.c_str(), value.size(), s);
	content += key;
	content.append(s);

	return 0;
}
