#ifndef _DHT11_H
#define _DHT11_H

#define uchar unsigned char
#define uint unsigned int
	
void DHT11_delay_us(unsigned char n);
void DHT11_delay_ms(unsigned int z);
void DHT11_start();
uchar DHT11_rec_byte();
void DHT11_receive();
		
#endif