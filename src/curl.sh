#########################################################################
# File Name: curl.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: 2018年09月09日 星期日 23时47分10秒
#########################################################################
#!/bin/bash

curl -H "Content-Type:application/json" -X POST --data 'encode=utf8&request={"title":"你好"}' http://127.0.0.1:8808/request
