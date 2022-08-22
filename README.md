1. 使用模块 

	a)DHT11  
	b)LCD1602   
	c)蜂鸣器  
	d)直流电机  
	e)24c02  
2 使用I/O口  
  	led灯为p1.0  
  	蜂鸣器为P1.1  
  	电机为P1.2  
  	DHT11为P3.6  
  模式按键为p3.2  
  历史记录按键 p3.3；  
  存储串口为SCL p2.0和SDA p2.1  

3使用说明  
  	1.LCD1602 第一行显示为当前实时温湿度。  
  	第二行前4位数字位当前最新的历史记录，其中前两位为湿度，后两位为温度。   
  	中间两位数字为当前记录次数，共100次，记录至100此时清零，重新从第0位开始记录。  
  	最后6位为查看历史记录位，按下按键在此查看历史记录，每按下一次按键，历史记录向前查看一位。
  
  2.按键：  
  有两个，按键1按下，可转换模式，分有两个模式，第一个为自动模式，自动检测温湿度并控制。  
  当按下按键后，转换为手动模式，开启电机。当处于手动模式时，再次按下按键1，将转换为自动模式，  
  在手动模式 中，也会实时检测当前的温湿度，如果超出阈值，将会开启报警（led灯闪烁，蜂鸣器响）  

 3.串口：  
  单片机会不断向上位机发送当前的温湿度数据，可以实时在上位机串口助手上查看，波特率为9600，  
  同样，也可在上位机控制单片机的模式转化，通过串口助手发送0和1来控制。0为自动模式，1为手动模式。	  
	

 3.遇到的问题：  
  1.LCD屏不显示以及无法正常显示  
    原因及解决方法  
   a)杜邦线连接不正确  
     将线连接正确后正常显示  
       
   b)显示屏未初始化  
     使用LCD_Init()函数进行初始化  
       
   c)显示屏显示的数据不正确   
     查看显示中的字符串中的数据是否正确  
       
   d)显示屏显示乱码  
     LCD1602显示屏显示的数据为ASCII编码，如果直接向显示屏发送数字会导致显示屏显示的为该数字代表的ASCII码的内容，   
     可在数字后加“ ‘0’ “来显示数字或直接加上”0x30“来让显示屏显示数字 
       
   e)显示屏不断闪烁  
     1.可能时电压供电不足，导致显示屏不断闪烁，可将其他的器件换一个I/O口相连  
     2.清屏函数LCD_clear()，该函数会清空显示屏上的内容，如果在主函数中写入该函数，显示屏会不断清屏导致屏幕上的内容不断闪烁。  

    2.DHT11无法正常获取温湿度的数据。  
      原因及解决方法:  
       a) data线连接不正确  
	data线定义在I/O口P3.6上，并且需要连接VCC和GND。  
	  
       b) 没有正确接收数据  
	需定义一个全局变量rec_dat[9]来接收温湿度数据。  
	  
     3.按键按下无响应  
	可能原因及解决方法  
	a)I/O口没有正确连接
	 将线连接正确即可  
	   
	b) 串口中断编写错误  
        1.中断号错误。外部中断0的中断号为0，外部中断1的中断号为2  
        2.中断允许未设置。需设置中断总允许EA=1。  
        c) 存在按键抖动
         可在中断程序中延迟10ms再执行函数。  
	   

      4.串口无法收发信息或发送信息不正确
	可能原因及解决方法  
        a)usb线或芯片有问题  
	  更换设备后再次尝试。  
	    
        b)中断设置不正确  
          由上位机向单片机发送信息需设置接收位为1，并设置串口中断函数，中断号为4.同时需启动定时器并设置波特率。    
	  
        c)上位机串口助手波特率设置不正确  
          可将波特率更改至与串口相同。  

       5.数据无法正确存储至存储器中  
	可能原因及解决办法    
	a)器件连接不正确  
	 再进行编写程序时可能会使用到1750光学传感器模块，该模块的针脚同样有SCL和SDA，再编写程序及连接时应当格外注意。  
	   
	b)函数的参数填写不正确  
	向存储器中写数据的函数中有4个参数，分别为器件总地址，字地址，要写的数据内容和位数。应当注意，总地址为存储器的地址，为固定值，字地址是存储地址，每位数据的地址都不同，应当注意。
	  
	c)存储的数据不正确   
	 1.存储数据时应当注意，写入数据的大小应当与函数参数中的位数一致，否则会导致写入的数据缺少，或者浪费空间。  
	 2.每写完数据，字地址应当变动，不可在字地址的参数上使用常量，否则将会一直在该地址上存储，导致只能存一条数据。同理读取数据的时候也不可以在该位置上填写常量，否则只能读取一位数据，这也是导致按键看似不管用的原因之一。  

    
