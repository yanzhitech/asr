### nao语音交互协议定义

#### 1.握手阶段：

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
[1](https://github.com/yanzhitech/asrService/blob/master/document/1.PNG)

![1](1.png)

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

![2](2.png)


#### 2.通信阶段：

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
	
	
![3](3.png)

#### 3.错误码说明：

![4](4.png)
