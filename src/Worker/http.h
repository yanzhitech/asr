#ifndef __HTTP_H_
#define __HTTP_H_

#include <string>
#include <ace/SOCK_Stream.h>
#include <fstream>
ssize_t read_nReal_timeout(const ACE_SOCK_Stream& stream, void *buf, size_t len, ACE_Time_Value *timeout, size_t *bt = 0);
int readHttpResponse(const ACE_SOCK_Stream& stream, ACE_Time_Value *timeout, std::string& result);
void URLDecode(const std::string &input, std::string& output);
void URLEncode(const char *input, size_t len, std::string& output);
std::string URLCode(const std::string &input);

int ParseHttpContentLength(const std::string &input, ssize_t content_length);
int ParseHttpContentChunk(std::string &input, std::string& result);
int ParseHttpHeader(std::string &input, ssize_t &content_length);

int add_param(std::string& content, const std::string& key, const std::string& value);
#endif

