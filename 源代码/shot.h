#ifndef _shot_H
#define _shot_H

#define uchar unsigned char
#define uint unsigned int
	
sbit SPK=P1^1;    //定义喇叭端口
	
void shout();		//发出声音
#endif