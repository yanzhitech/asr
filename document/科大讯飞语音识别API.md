<h1>科大讯飞语音识别API</h1>
------------------
<h2>主要流程</h2>
![](https://i.imgur.com/nLDJBQ4.png)
------------
##API详细介绍
（1）MSPLogin（）
<pre><code>MSPLogin()
int MSPAPI MSPLogin	(	const char * 	usr,
const char * 	pwd,
const char * 	params 
)	</code></pre>

<strong>作用：</strong>初始化msc，用户登录。</br></br>
**参数：**</br>
usr[in]	此参数保留，传入NULL即可。</br>
pwd[in]	此参数保留，传入NULL即可。</br>
params[in]	参见下表：</br>
格式说明：每个参数和参数值通过key=value的形式组成参数对；如果有多个参数对，再用逗号进行拼接，如：key_1=value_1,key_2=value_2 </br>
注意：每个参数(key)和参数值(value)均不得含有逗号(,)和等号(=)，否则会被截断</br>
![](https://i.imgur.com/ROgHAda.png)
</br></br>
**返回：**成功返回MSP_SUCCESS，否则返回错误代码（错误码解释：[https://shimo.im/sheet/w3yUy39uNKs0J7DT/undefined](https://shimo.im/sheet/w3yUy39uNKs0J7DT/undefined "详见错误码解释")）</br></br>
**备注：**使用其他接口前必须先调用MSPLogin，可以在应用程序启动时调用。</br></br>
**参见：**</br>
<pre><code>const char* usr = NULL;
const char* pwd = NULL;
const char* lgi_param = "appid = ********";
int ret = MSPLogin(usr, pwd, lgi_param);
if( MSP_SUCCESS != ret )
{
printf( "MSPLogin failed, error code is: %d", ret );
}<code></pre>


2.MSPLogout()
<pre><code>int MSPAPI MSPLogout	()	</code></pre>
**作用：**退出登录。</br></br>

**返回:**
如果函数调用成功返回MSP_SUCCESS，否则返回错误代码，详见错误码列表([https://shimo.im/sheet/w3yUy39uNKs0J7DT](https://shimo.im/sheet/w3yUy39uNKs0J7DT)。</br></br>
**备注:**
本接口和MSPLogin配合使用。确保其他接口调用结束之后调用MSPLogout，否则结果不可预期。</br></br>
**参见:**
<pre><code>int ret = MSPLogout( );
if(MSP_SUCCESS != ret)
{
    printf("MSPLogout failed, error code is: %d", ret);
}</code></pre></br></br></br>


3.QISRSessionBegin()</br>
<pre><code>const char* MSPAPI QISRSessionBegin	(	const char * 	grammarList,
const char * 	params,
int * 	errorCode 
)		</code></pre></br>
**作用：**开始一次语音识别。</br></br>

**参数：**</br>
grammarList[in]	参见下表:</br>
<table>
<tr>
	<th><在线/离线业务></th>
	<th>类型</th>	
	<th>说明</th>
</tr>
<tr>
	<th>在线</th>	
	<th>关键词识别(sub=asr)</th>	
	<th>传入调用MSPUploadData接口上传关键词的返回值。关键词会永久生效。</th>
</tr>
<tr>
	<th>在线</th>	
	<th>语法识别(sub=asr)</th>	
	<th>除上述方法外，也可在此处传入语法字符串指针，并在params参数中添加"grammartype=abnf"或" grammartype=xml"。此方法仅在本次识别有效。</th>
</tr>
<tr>
	<th>在线</th>	
	<th>连续语音识别(sub=iat)</th>	
	<th>此参数设为NULL</th>
</tr>
<tr>
	<th>离线</th>
	<th></th>		
	<th>此参数设为NULL</th>
</tr>
</table>
</br></br>
params[in]	参见下表:</br>
格式说明：每个参数和参数值通过key=value的形式组成参数对；如果有多个参数对，再用逗号进行拼接，如：key_1=value_1,key_2=value_2 </br>
注意：每个参数(key)和参数值(value)均不得含有逗号(,)和等号(=)，否则会被截断</br>
<table border="1">
<tr>
	<th><在线/离线业务></th>
	<th>参数</th>	
	<th>名称</th>
	<th>说明</th>
</tr>
<tr>
	<th>通用	</th>	
	<th>engine_type</th>	
	<th>引擎类型</th>
	<th>可取值：</br>
	cloud：在线引擎</br>
	local：离线引擎</br>
	mixed：混合引擎</br>
	默认值：cloud</th>
</tr>
<tr>
	<th>在线</th>	
	<th>sub</th>	
	<th>本次识别请求的类型</th>
	<th>iat:连续语音识别</br>
		asr:语法、关键词识别。</br>
	默认为iat</th>
</tr>
<tr>
	<th>在线</th>	
	<th>language</th>	
	<th>语言</th>
	<th>可取值：</br>
		zh_cn：简体中文</br>
		en_us：英文</br>
		默认值：zh_cn</th>
</tr>
<tr>
	<th>在线</th>
	<th>domain</th>		
	<th>领域</th>
	<th>iat：连续语音识别</br>
		asr：语法、关键词识别</br>
		search：热词</br>
		video：视频</br>
		poi：地名</br>
		music：音乐</br>
		默认为iat。</br>
		注意：sub=asr时，domain只能为asr</th>
</tr>
<tr>
	<th>在线</th>
	<th>accent</th>		
	<th>语言区域</th>
	<th>可取值：</br>
		mandarin：普通话</br>
		cantonese：粤语</br>
		lmz：四川话</br>
		默认值：mandarin</br></th>
</tr>
<tr>
	<th>通用	</th>
	<th>sample_rate</th>		
	<th>音频采样率</th>
	<th>可取值：16000，8000</br>默认值：16000 </br>
		离线识别不支持8000采样率音频</th>
</tr>
<tr>
	<th>离线	</th>
	<th>asr_threshold</th>		
	<th>识别门限</th>
	<th>离线语法识别结果门限值，设置只返回置信度得分大于此门限值的结果</br>可取值：0~100，默认值：0</th>
</tr>
<tr>
	<th>离线	</th>
	<th>asr_denoise</th>		
	<th>是否开启降噪功能</th>
	<th>可取值：0：不开启，1：开启</br>
		默认不开启</th>
</tr>
<tr>
	<th>通用	</th>
	<th>result_type</th>		
	<th>结果格式</th>
	<th>可取值：plain，json</br>
		默认值：plain</th>
</tr>
<tr>
	<th>通用	</th>
	<th>text_encoding</th>		
	<th>文本编码格式</th>
	<th>表示参数中携带的文本编码格式</th>
</tr>
<tr>
	<th>在线	</th>
	<th>cloud_grammar</th>		
	<th>在线语法id</th>
	<th>构建在线语法后获得的语法ID</th>
</tr>
<tr>
	<th>混合</th>
	<th>mixed_type</th>		
	<th>混合引擎策略类型</br>
	(仅engine_type=mixed时生效)</th>
	<th>可取值：</br>
	realtime：实时，同时使用在线引擎和离线引擎，在在线引擎结果超时的情况下，使用离线引擎结果；<br>
	delay：延时，先使用在线引擎，当在线引擎结果超时后自动转向离线引擎。<br>
	默认值：realtime</th>
</tr>
<tr>
	<th>混合</th>
	<th>mixed_timeout</th>		
	<th>在线引擎结果超时时间</br>
	(仅engine_type=mixed时生效)</th>
	<th>使用混合引擎情况下，在线引擎结果超时时间。</br>
	默认值：2000，单位：ms</th>
</tr>
<tr>
	<th>混合</th>
	<th>mixed_threshold</th>		
	<th>混合门限</br>
	(仅engine_type=mixed时生效)</th>
	<th>混合策略为realtime 时使用，当离线引擎给出识别结果大于此门限值时直接给出离线结果，</br>
	可取值：0~100，默认值：60 </th>
</tr>
<tr>
	<th>通用</th>
	<th>ptt</th>		
	<th>添加标点符号(仅sub=iat时有效)</th>
	<th>0:无标点符号;1:有标点符号。默认为1</th>
</tr>
<tr>
	<th>在线	</th>
	<th>sch</th>		
	<th>启用翻译功能</th>
	<th>1</th>
</tr>
<tr>
	<th>在线	</th>
	<th>addcap</th>		
	<th>启用翻译功能(仅在sch=1时有效)</th>
	<th>translate</th>
</tr>
<tr>
	<th>在线	</th>
	<th>addcap</th>		
	<th>启用翻译功能(仅在sch=1时有效)</th>
	<th>translate</th>
</tr>
<tr>
	<th>在线	</th>
	<th>orilang</th>		
	<th>原始语种(仅在启用翻译功能时有效)</th>
	<th>cn:中文,en：英文</th>
</tr>
<tr>
	<th>在线	</th>
	<th>translang</th>		
	<th>目标语种(仅在启用翻译功能时有效)</th>
	<th>cn:中文,en：英文</th>
</tr>
<tr>
	<th>在线	</th>
	<th>trssrc</th>		
	<th>结果格式(仅在启用翻译功能时有效)</th>
	<th>its</th>
</tr>
<tr>
	<th>在线	</th>
	<th>aue</th>		
	<th>音频编码格式和压缩等级</th>
	<th>编码算法：raw；speex；speex-wb；ico</br>
	编码等级：raw：不进行压缩。speex系列：0-10；</br>
	默认为speex-wb;7</br>
	speex对应sample_rate=8000</br>
	speex-wb对应sample_rate=16000</br>
	ico对应sample_rate=16000</th>
</tr>
<tr>
	<th>通用	</th>
	<th>result_encoding</th>		
	<th>识别结果字符串所用编码格式</th>
	<th>GB2312;UTF-8;UNICODE</br>
	不同的格式支持不同的编码：</br>
	plain:UTF-8,GB2312</br>
	json:UTF-8</br></th>
</tr>
<tr>
	<th>通用	</th>
	<th>grammartype</th>		
	<th>语法类型(仅sub=asr时有效)</th>
	<th>abnf</br>
	xml</br></th>
</tr>
<tr>
	<th>通用	</th>
	<th>vad_enable</th>		
	<th>VAD功能开关</th>
	<th>是否启用VAD</br>
	默认为开启VAD </br>
	0（或false）为关闭</th>
</tr>
<tr>
	<th>通用	</th>
	<th>vad_bos</th>		
	<th>允许头部静音的最长时间(目前未开启该功能)</th>
	<th>如果静音时长超过了此值，则认为用户此次无有效音频输入。此参数仅在打开VAD功能时有效。</th>
</tr>
<tr>
	<th>通用	</th>
	<th>vad_eos</th>		
	<th>允许尾部静音的最长时间</th>
	<th>0-10000毫秒。默认为2000</br>
	如果尾部静音时长超过了此值，则认为用户音频已经结束，此参数仅在打开VAD功能时有效。</th>
</tr>
</table>
</br></br>
errorCode[out]	函数调用成功则其值为MSP_SUCCESS，否则返回错误代码，详见错误码列表([https://shimo.im/sheet/w3yUy39uNKs0J7DT](https://shimo.im/sheet/w3yUy39uNKs0J7DT))。</br></br>

**返回:**
函数调用成功返回字符串格式的sessionID，失败返回NULL。sessionID是本次识别的句柄。</br></br>
**备注:**
参数只在当次识别中生效。</br></br>
**参见:**
<pre><code>const char * params = "engine_type = local, asr_res_path = ****, sample_rate = ****, grm_build_path = ****, local_grammar = ****, result_type = json, result_encoding = UTF-8";
int    ret = 0;
const char* sessionID = QISRSessionBegin( NULL, params, &ret );
if( MSP_SUCCESS != ret )
{
    printf( "QISRSessionBegin failed, error code is: %d", ret );
}</code></pre></br></br>

4.QISRAudioWrite()</br>
<pre><code>int MSPAPI QISRAudioWrite	(	const char * 	sessionID,
const void * 	waveData,
unsigned int 	waveLen,
int 	audioStatus,
int * 	epStatus,
int * 	recogStatus 
)	</code></pre></br><br>
	
**作用：**写入本次识别的音频。</br></br>

**参数:**</br>
sessionID[in]	由QISRSessionBegin返回的句柄。</br>
waveData[in]	音频数据缓冲区起始地址。</br>
waveLen[in]	音频数据长度,单位字节。</br>
audioStatus[in]	用来告知MSC音频发送是否完成，典型值如下：</br>
<table>
<tr>
	<th>枚举常量</th>	
	<th>简介</th>
</tr>
<tr>
	<th>MSP_AUDIO_SAMPLE_FIRST = 1</th>	
	<th>第一块音频</th>
</tr>
<tr>
	<th>MSP_AUDIO_SAMPLE_CONTINUE = 2</th>	
	<th>还有后继音频</th>
</tr>
<tr>
	<th>MSP_AUDIO_SAMPLE_LAST = 4</th>	
	<th>最后一块音频</th>
</tr>
</table></br></br>


epStatus[out]	端点检测（End-point detected）器所处的状态，可能的值如下：
<table>
<tr>
	<th>枚举常量</th>	
	<th>简介</th>
</tr>
<tr>
	<th>MSP_EP_LOOKING_FOR_SPEECH = 0</th>	
	<th>还没有检测到音频的前端点。</th>
</tr>
<tr>
	<th>MSP_EP_IN_SPEECH = 1</th>	
	<th>已经检测到了音频前端点，正在进行正常的音频处理。</th>
</tr>
<tr>
	<th>MSP_EP_AFTER_SPEECH = 3</th>	
	<th>检测到音频的后端点，后继的音频会被MSC忽略。</th>
</tr>
<tr>
	<th>MSP_EP_TIMEOUT = 4</th>	
	<th>超时。</th>
</tr>
<tr>
	<th>MSP_EP_ERROR = 5</th>	
	<th>出现错误。</th>
</tr>
<tr>
	<th>MSP_EP_MAX_SPEECH = 6</th>	
	<th>音频过大。</th>
</tr>
</table></br></br>


rsltStatus[out]	识别器返回的状态，提醒用户及时开始\停止获取识别结果。典型值如下：</br>
<table>
<tr>
	<th>枚举常量</th>	
	<th>简介</th>
</tr>
<tr>
	<th>MSP_REC_STATUS_SUCCESS = 0</th>	
	<th>识别成功，此时用户可以调用QISRGetResult来获取（部分）结果。</th>
</tr>
<tr>
	<th>MSP_REC_STATUS_NO_MATCH = 1</th>	
	<th>识别结束，没有识别结果。</th>
</tr>
<tr>
	<th>MSP_REC_STATUS_INCOMPLETE = 2</th>	
	<th>正在识别中。</th>
</tr>
<tr>
	<th>MSP_REC_STATUS_COMPLETE = 5</th>	
	<th>识别结束。</th>
</tr>
</table></br></br>

**返回:**</br>
函数调用成功则其值为MSP_SUCCESS，否则返回错误代码，详见错误码列表。</br>
**备注:**</br>
本接口需不断调用，直到音频全部写入为止。上传音频时，需更新audioStatus的值。具体来说:</br>
当写入首块音频时,将audioStatus置为MSP_AUDIO_SAMPLE_FIRST</br>
当写入最后一块音频时,将audioStatus置为MSP_AUDIO_SAMPLE_LAST</br>
其余情况下,将audioStatus置为MSP_AUDIO_SAMPLE_CONTINUE</br>
同时，需定时检查两个变量：epStatus和rsltStatus。具体来说:</br>
当epStatus显示已检测到后端点时，MSC已不再接收音频，应及时停止音频写入</br>
当rsltStatus显示有识别结果返回时，即可从MSC缓存中获取结果</br></br>

**参见:**
<pre><code>char audio_data[ 5120 ] ={'\0'};
unsigned int audio_len = 0;
int audio_status = 2;
int ep_status = 0;
int rec_status = 0;
int ret = 0;
while(MSP_AUDIO_SAMPLE_LAST != audio_status )
{
    // 读取音频到缓冲区audio_data 中，设置音频长度audio_len，音频状态audio_status。
    ret = QISRAudioWrite( sessionID, audio_data, audio_len, audio_status, &ep_status, &rec_status );
    if( MSP_SUCCESS  ! = ret )
    {
        printf( "QISRAudioWrite failed, error code is: %d", ret );
        break;
    }
    else if(MSP_EP_AFTER_SPEECH == ep_status ) // 检测到音频后端点，停止写入音频
    {
        printf( "end point of speech has been detected!" );
        break;
    }
    // 如果是实时采集音频，可以省略此操作。5KB 大小的16KPCM 持续的时间是160 毫秒
    Sleep( 160 );
}	</code></pre>
	
5. QISRGetResult()</br></br>
<pre><code>const char* MSPAPI QISRGetResult	(	const char * 	sessionID,
int * 	rsltStatus,
int 	waitTime,
int * 	errorCode 
)	</code></pre></br></br>
	
**作用：**获取识别结果。</br></br>


**参数:**</br>
sessionID[in]	由QISRSessionBegin返回的句柄。</br>
rsltStatus[out]	识别结果的状态，其取值范围和含义请参考QISRAudioWrite 的参数recogStatus。</br>
waitTime[in]	此参数做保留用。</br>
errorCode[out]	函数调用成功则其值为MSP_SUCCESS，否则返回错误代码，详见错误码列表([https://shimo.im/sheet/w3yUy39uNKs0J7DT](https://shimo.im/sheet/w3yUy39uNKs0J7DT))。</br>
**返回:**</br>
函数执行成功且有识别结果时，返回结果字符串指针；其他情况(失败或无结果)返回NULL。</br>
**备注:**</br>
当写入音频过程中已经有部分识别结果返回时，可以获取结果。在音频写入完毕后，用户需反复调用此接口，直到识别结果获取完毕（rlstStatus值为5）或返回错误码。 注意：如果某次成功调用后暂未获得识别结果，请将当前线程sleep一段时间，以防频繁调用浪费CPU资源。</br>
**参见:**</br>
<pre><code>char rslt_str[ 2048 ] ={'\0'};
const char* rec_result = NULL;
int rslt_status = 0;
int ret = 0;
while(MSP_REC_STATUS_SPEECH_COMPLETE != rslt_status )
{
    rec_result = QISRGetResult ( sessionID, &rslt_status, 5000, &ret );
    if( MSP_SUCCESS  != ret )
    {
        printf( "QISRGetResult failed, error code is: %d", ret );
        break;
    }
    if( NULL != rec_result )
    {
     // 用户可以用其他的方式保存识别结果
        strcat( rslt_str, rec_result );
        continue;
    }
    // sleep 一下很有必要，防止MSC 端无缓存的识别结果时浪费CPU 资源
    Sleep( 200 );
}	</code></pre></br></br>
	
6.QISRSessionEnd()</br>
<pre><code>int MSPAPI QISRSessionEnd	(	const char * 	sessionID,
const char * 	hints 
)	</code></pre></br></br>	

**作用：**结束本次语音识别。</br></br>

**参数:**</br>
sessionID[in]	由QISRSessionBegin返回的句柄。</br>
hints[in]	结束本次语音识别的原因描述，为用户自定义内容。
返回</br>
函数调用成功则其值为MSP_SUCCESS，否则返回错误代码，详见错误码列表。([https://shimo.im/sheet/w3yUy39uNKs0J7DT](https://shimo.im/sheet/w3yUy39uNKs0J7DT))</br></br>

**备注:**</br>
本接口和QISRSessionBegin对应,调用此接口后，该句柄对应的相关资源（参数、语法、音频、实例等）都会被释放，用户不应再使用该句柄。</br></br>

**参见:**</br>
<pre><code>int ret = QISRSessionEnd ( sessionID, "normal end" );
if( MSP_SUCCESS  != ret )
{
    printf( "QISRSessionEnd failed, error code is: %d", ret );
}	</code></pre>
	
	
	
	
	

