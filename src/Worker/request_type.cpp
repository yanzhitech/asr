#include "request_type.h"
#include<iostream>

RequestType get_request_type(std::string str)
{
    if(str=="Test"){
	return TEST;	
    }else if(str=="Dialogue"){
	return DIALOGUE;
    }else if(str=="Chat"){
	return CHAT;
    }else if(str=="Home"){
	return HOME;
    }else if(str=="PenchoChat"){
	return PENCHOCHAT;
    }else if(str=="Music"){
	return MUSIC;
    }else if(str=="Weather"){
	return WEATHER;
    }else if(str=="Stock"){
	return STOCK;
    }else if(str=="FaceImg"){
	return FACEIMG;			//发图片
    }else if(str=="FaceAno"){
	return FACEANO;			//发标注
    }else if(str=="FaceQuery"){
	return FACEQUERY;			//发标注
    }else if(str=="MateQuery"){
	return MATEQUERY;
    }else if(str=="NLPSegment"){
	return NLPSEGMENT;
    }else if(str=="NewsQuery"){
	return NEWSQUERY;
    }else if(str=="NewsQueryWX"){
	return NEWSQUERYWX;
    }else if(str=="NewsAllScene"){
	return NEWSALLSCENE;
    }else if(str=="NewsRelatedTopic"){
	return NEWSRELATEDTOPIC;
    }else if(str=="NewsDoc"){
	return NEWSDOC;
    }else if(str=="NewsDocApp"){
	return NEWSDOCAPP;
    }else if(str=="NewsHotTopic"){
	return NEWSHOTTOPIC;
    }else if(str=="QA"){
	return QA;
    }else if(str=="RobotMallList"){
	return ROBOTMALLLIST;
    }else if(str=="RobotMallDetail"){
	return ROBOTMALLDETAIL;
    }else{
	return UNKNOW;
    }

}
