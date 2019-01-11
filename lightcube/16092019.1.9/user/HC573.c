#include"STC12C5A60S2.H" //芯片头文件
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int  uint; 
sbit cs1 = P2^7;   //低电平时，数据锁存
sbit cs2 = P2^6;
sbit cs3 = P2^5;
sbit cs4 = P2^4;
sbit cs5 = P2^3;
sbit cs6 = P2^2;
sbit cs7 = P2^1;
sbit cs8 = P2^0;
#define DATA_573 P0 //573的数据端口
void hang1()
{

	cs1 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs1 = 0; //
	cs2 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs2 = 0; //
	cs3 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs3 = 0; //
	cs4 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs4 = 0; //
	cs5 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs5 = 0; //
	cs6 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs6 = 0; //
	cs7 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs7 = 0; //
	cs8 = 1; _nop_(); DATA_573 = 0xff; _nop_(); cs8 = 0; //
}

void hang2()
{
	cs1 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs1 = 0; //
	cs2 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs2 = 0; //
	cs3 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs3 = 0; //
	cs4 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs4 = 0; //
	cs5 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs5 = 0; //
	cs6 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs6 = 0; //
	cs7 = 1; _nop_(); DATA_573 = 0xff; _nop_(); cs7 = 0; //
	cs8 = 1; _nop_(); DATA_573 = 0x00; _nop_(); cs8 = 0; //
}

void hang3()
{

	cs1 = 1; DATA_573 = 0x00; cs1 = 0; //
	cs2 = 1; DATA_573 = 0x00; cs2 = 0; //
	cs3 = 1; DATA_573 = 0x00; cs3 = 0; //
	cs4 = 1; DATA_573 = 0x00; cs4 = 0; //
	cs5 = 1; DATA_573 = 0x00; cs5 = 0; //
	cs6 = 1; DATA_573 = 0xff; cs6 = 0; //
	cs7 = 1; DATA_573 = 0x00; cs7 = 0; //
	cs8 = 1; DATA_573 = 0x00; cs8 = 0; //
}

void hang4()
{
	cs1 = 1; DATA_573 = 0x00; cs1 = 0; //
	cs2 = 1; DATA_573 = 0x00; cs2 = 0; //
	cs3 = 1; DATA_573 = 0x00; cs3 = 0; //
	cs4 = 1; DATA_573 = 0x00; cs4 = 0; //
	cs5 = 1; DATA_573 = 0xff; cs5 = 0; //
	cs6 = 1; DATA_573 = 0x00; cs6 = 0; //
	cs7 = 1; DATA_573 = 0x00; cs7 = 0; //
	cs8 = 1; DATA_573 = 0x00; cs8 = 0; //
}

void hang5()
{
	cs1 = 1; DATA_573 = 0x00; cs1 = 0; //
	cs2 = 1; DATA_573 = 0x00; cs2 = 0; //
	cs3 = 1; DATA_573 = 0x00; cs3 = 0; //
	cs4 = 1; DATA_573 = 0xff; cs4 = 0; //
	cs5 = 1; DATA_573 = 0x00; cs5 = 0; //
	cs6 = 1; DATA_573 = 0x00; cs6 = 0; //
	cs7 = 1; DATA_573 = 0x00; cs7 = 0; //
	cs8 = 1; DATA_573 = 0x00; cs8 = 0; //
}

void hang6()
{
	cs1 = 1; DATA_573 = 0x00; cs1 = 0; //
	cs2 = 1; DATA_573 = 0x00; cs2 = 0; //
	cs3 = 1; DATA_573 = 0xff; cs3 = 0; //
	cs4 = 1; DATA_573 = 0x00; cs4 = 0; //
	cs5 = 1; DATA_573 = 0x00; cs5 = 0; //
	cs6 = 1; DATA_573 = 0x00; cs6 = 0; //
	cs7 = 1; DATA_573 = 0x00; cs7 = 0; //
	cs8 = 1; DATA_573 = 0x00; cs8 = 0; //
}

void hang7()
{
	cs1 = 1; DATA_573 = 0x00; cs1 = 0; //
	cs2 = 1; DATA_573 = 0xff; cs2 = 0; //
	cs3 = 1; DATA_573 = 0x00; cs3 = 0; //
	cs4 = 1; DATA_573 = 0x00; cs4 = 0; //
	cs5 = 1; DATA_573 = 0x00; cs5 = 0; //
	cs6 = 1; DATA_573 = 0x00; cs6 = 0; //
	cs7 = 1; DATA_573 = 0x00; cs7 = 0; //
	cs8 = 1; DATA_573 = 0x00; cs8 = 0; //
}

void hang8()
{
	cs1 = 1; DATA_573 = 0xff; cs1 = 0; //
	cs2 = 1; DATA_573 = 0x00; cs2 = 0; //
	cs3 = 1; DATA_573 = 0x00; cs3 = 0; //
	cs4 = 1; DATA_573 = 0x00; cs4 = 0; //
	cs5 = 1; DATA_573 = 0x00; cs5 = 0; //
	cs6 = 1; DATA_573 = 0x00; cs6 = 0; //
	cs7 = 1; DATA_573 = 0x00; cs7 = 0; //
	cs8 = 1; DATA_573 = 0x00; cs8 = 0; //
}
