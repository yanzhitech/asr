# 实时转写Restful接口协议

* 客户端：

```
{
    "appid":string,
    "userid":string,
    "ts":string,
    "sid":string, //每段连续音频，一个唯一值
    "data":binary message //最后发空字段结束
}
```

* 服务端：

```
{
        "action":string,
        "code":string,
        "data":json,
        "desc":string,
        "sid":string
}
```
样例：
```
data：
    {
        "cn":{
            "st":{
                "bg":"820",
                "ed":"0",
                "rt":[{
                    "ws":[{
                        "cw":[{
                            "w":"啊",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"喂",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"！",
                            "wp":"p"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"你好",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"！",
                            "wp":"p"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"我",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"是",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                                "w":"上",
                                "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    }]
                }],
                "type":"1"
            }
        },
        "seg_id":5
    }
```

| 字段 | 含义 | 描述 |
| ------ | ------ | ------ |
| bg | 句子开始时间 | 中间结果的bg为准确值 |
| ed | 句子结束时间 | 中间结果的ed为0 |
| w | 词识别结果 |  |
| wp | 词标识 | n-普通词；s-顺滑词；p-标点 |
| wb | 词开始时间 | 中间结果的 wb 为 0 |
| we | 词结束时间 | 中间结果的 we 为 0 |
| type | 结果类型标识 | 0-最终结果；1-中间结果 |
| seg_id | 转写结果序号 | 从0开始 |



# 预研

## 语音 WebSocket协议定义

### 1.握手阶段：

* 客户端：
{
    "appid":string,
    "ts":string
}

 样例：
 {

   "action":"595f23df",

   "ts":"1512041814"

 }

说明：

![1](https://github.com/yanzhitech/asrService/blob/master/document/1.PNG)


* 服务器：
{

    "action":string,
    "code":string,
    "data":string,
    "desc":string,
    "sid":string
}

样例：

失败：

 {

   "action":"error",

   "code":"10110",

   "data":"",

   "desc":"invalid authorization|illegal signa",

   "sid":"rta0000000b@ch312c0e3f65f09f0900"

 }

成功：

 {

   "action":"started",

   "code":"0",

   "data":"",

   "desc":"success",

   "sid":"rta0000000a@ch312c0e3f63609f0900"

 }

![2](https://github.com/yanzhitech/asrService/blob/master/document/2.PNG)

### 2.通信阶段：

* 客户端：
{
    "appid":string,
    "ts":string,
    "sid":string,
    "data":binary message
}

* 服务器：
{
        "action":string,
        "code":string,
        "data":json,
        "desc":string,
        "sid":string
}

样例：

data：
    {
        "cn":{
            "st":{
                "bg":"820",
                "ed":"0",
                "rt":[{
                    "ws":[{
                        "cw":[{
                            "w":"啊",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"喂",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"！",
                            "wp":"p"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"你好",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"！",
                            "wp":"p"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"我",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                            "w":"是",
                            "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    },{
                        "cw":[{
                                "w":"上",
                                "wp":"n"
                        }],
                        "wb":0,
                        "we":0
                    }]
                }],
                "type":"1"
            }
        },
        "seg_id":5
    }
	
	
![3](https://github.com/yanzhitech/asrService/blob/master/document/3.PNG)

#### 3.错误码说明：

![4](https://github.com/yanzhitech/asrService/blob/master/document/4.PNG)
