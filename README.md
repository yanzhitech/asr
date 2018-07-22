# asr service
## 1. 编译及跑通样例(ubuntu上验证)
### a. 在同一个目录下创建asr, kaldi两个子目录，其中kaldi子目录为kaldi源码
	i. kaldi子目录
		1) git clone https://github.com/kaldi-asr/kaldi.git
		2) 完成编译
	ii. asr子目录
		1) asr_sample.tar.gz解压后的文件	
	目录结构如下：
		xx/speech
			kaldi
				README.md
				INSTALL
				COPYING
				egs
				misc
				scripts
				windows
				tools
				src
			asr
				words.txt
				final.mdl
				HCLG.fst
				fbank.conf
				online.conf
				run.sh
				data
					wav
						T0001W0001.wav
					spk.scp
					wav.scp
		
### b. 在asr子目录下运行run.sh，加载模型，跑出样例结果
## 2. 服务化
	Client/Service(SDK / Service)
### a. SDK
	i. 采集语音信号及音频信号相关处理，熟悉WebRTC
### b. Service
### c. 协议
	i. 音频按照帧分割，流式调用服务器，每次上传一个时间片段的音频帧数据，最后一次会增加一个完成状态标记。服务器接收到找个状态标记，输出完整的ASR识别结果。
## 3. AM，LM训练
	http://kaldi-asr.org/doc/index.html
### a. 熟悉GMM-HMM
### b. 熟悉DNN
	http://kaldi-asr.org/doc/dnn.html 
	http://www.cnblogs.com/welen/p/7567741.html
### c. 熟悉SRILM工具训练LM模型
## 4. HCLG生成
