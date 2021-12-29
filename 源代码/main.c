

#include	<reg52.h>
#include	<delay.h>
#include	<1602.h>
#include	<DHT11.h>
#include	<shot.h>
#include	<uart.h>
#include <24c02.h>

#include <intrins.h>

#define MAX_R 50




void SendByte(char dat);
void SendStr(char *s);


//led灯为p1.0
//蜂鸣器为P1.1
//电机为P1.2
//DHT11为P3.6
//模式按键为p3.2
//历史记录按键 p3.3；
//存储串口为SCL p2.0和SDA p2.1


sbit led=P1^0;
sbit dianji=P1^2;
sbit key=P3^2;		//控制模式按键
sbit key1=P3^3;		//查看上一条记录		外部中断1
sbit key3=P1^4;		
//sbit test_led=P1^7; 		//测试灯




extern char rec_dat[9];   	//全局变量，接收温湿度的字符串
char eep_dat[6];		//读取存储器的缓冲区

char eep_write[6];			//存储字符串

unsigned int mode;		//模式
unsigned int eep_w=0;		//存储位置
unsigned int eep_r=0;		//读取位置
unsigned int time=0;  //时间
unsigned int x=0;			//记录单双次
static int xnum=0;						//记录温湿度次数
	int i=0;
	static int num=0;				//记录当前查看的历史记录位置。
	unsigned char eep_his[6];		//历史记录查看

void main()   
{   
	
	int RH;
	int compRH;
	
//	int j=0;
	LCD_Init();
	led=1;
	
	dianji=1;
  DHT11_delay_ms(100);    //DHT11
	
	//外部中断初始化
	EX0=1;IT0=1; 
	EX1=1;IT1=1;
	EA=1;
	TR1=1;
	
	//串口初始化
	PCON=0;
	SCON=0x50;//MOD 2 9600,can receive;
	ES=1;		//chuan kou Init
	TMOD=0x20;
	TL1=0xFD;
	TH1=0xFD;	//串口初始化
	
	
	
	while(1)   
	{  	
			switch(mode){
				case 0 :
					
					
					RH=(rec_dat[2]-'0')*10+(rec_dat[3]-'0');
					compRH=(eep_dat[0]-'0')*10+(eep_dat[1]-'0');
					mode=0;
					if(xnum==99){
							xnum=0;
							eep_w=0;
					}
					eep_write[0]=rec_dat[2];		//获取温湿度
					eep_write[1]=rec_dat[3];
					eep_write[2]=rec_dat[7];
					eep_write[3]=rec_dat[8];	
					eep_write[4]=(xnum/10)+'0';		//记录次数的十位数
					eep_write[5]=(xnum%10)+'0';		//个位数
					
					
					LCD_Write_Char(10,1,eep_his[0]);
					LCD_Write_Char(11,1,eep_his[1]);
					LCD_Write_Char(12,1,eep_his[2]);
					LCD_Write_Char(13,1,eep_his[3]);
					//显示最后一条温湿度记录
				/*	LCD_Write_Char(10,1,eep_dat[0]);
					LCD_Write_Char(11,1,eep_dat[1]);	
					LCD_Write_Char(10,0,rec_dat[2]);
					LCD_Write_Char(11,0,rec_dat[3]);*/
					
		

					
					IRcvStr(0xae,eep_r*6,eep_dat,6);		//读取数据并显示
					for(i=0;i<4;i++){
						LCD_Write_Char(i,1,eep_dat[i]);
					}
					
								
					if(compRH!=RH)
						{
							ISendStr(0xae,eep_w*6,eep_write,6);  
							DelayMs(125);					//延时125毫秒，等待写入操作完成
							xnum++;				//次数+1
							eep_w++;					//位置+1
							eep_r=eep_w-1;//写入的位置已经+1，应-1后在赋值给读取的位置
							
					}
						
					LCD_Write_Char(7,1,(xnum%100)/10+'0');			//十位数
					LCD_Write_Char(8,1,(xnum%10)+'0');			//个位数
					
				
				
					
				
			
				
					SendStr(rec_dat);
					
					DHT11_receive();	
					
				//显示当前温湿度
					for(i=0;i<9;i++){
						LCD_Write_Char(i,0,rec_dat[i]);	
					}
					
					//判断是否超出阈值
					if(RH>MAX_R){
						shout();
						led=~led;
					}
					else{
						dianji=1;
						led=1;
					}
				
					break;
					
					
					
				case 1:
					dianji=0;
					mode=1;
					RH=(rec_dat[2]-'0')*10+(rec_dat[3]-'0');
					DHT11_receive();
					//LCD_Write_String(0,0,rec_dat);
					for(i=0;i<9;i++){
						LCD_Write_Char(i,0,rec_dat[i]);	
					}
					LCD_Write_Char(0,1,mode+'0');
					if(RH>MAX_R){
						shout();
						led=~led;
					}
					else{
						led=1;
					}
					break;
			}
		
	}   
	
}   

void it0 (void) interrupt 0			//外部中断0
{
		DelayMs(10);
		mode=(mode+1)%2;
}




void it1 (void) interrupt 2			//外部中断1
{
	if(num==xnum){
			num==0;
	}
		num++;
		eep_r=eep_r-num-1;
		IRcvStr(0xae,eep_r*6,eep_his,6);
		eep_r=eep_r+num+1;
		
}


void ck (void	) interrupt 4				//串口中断
{
	if(RI)
	{
		mode=SBUF;
		RI=0;
	}
}
