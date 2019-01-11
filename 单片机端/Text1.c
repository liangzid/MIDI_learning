#include <REG51.H>
#include <intRIns.h>
#include <ABSACC.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define uint unsigned int 
#define uchar unsigned char
#define numLEDs 64
#define N 5


sfr AUXR =0x8E;
sbit P10=P1^0;
sbit P11=P1^1;
//sbit P12=P1^2;
sbit P13=P1^3;
sbit WS2812 = P1^2;

sbit cs=P1^6;
sbit clk=P1^7;
sbit key=P1^5;
sbit dat=P1^4;


unsigned char  buf_R[numLEDs] = { 0 };//??é??o′?
unsigned char  buf_G[numLEDs] = { 0 };
unsigned char  buf_B[numLEDs] = { 0 };
void RGB_Set_Up();  //?í0??
void RGB_Set_Down(); //?í1??
void Delay1ms(uchar k);
void SetPointColour(unsigned int num, unsigned char r, unsigned char g, unsigned char b);
void SentPixelColor(unsigned char num,unsigned long c);
void HAL_Delay(unsigned int t)
{
	unsigned int x, y;
	for (x = 114; x>0; x--)
		for (y = t; y>0; y--);
}
void ReSet()
{
	uint i;
	for(i=0;i<64;i++)
	{
	SentPixelColor(i,0);
	}
}


//?′???óê±
void Delay50us()		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}
// 高电平850ns 低电平400ns 误差150ns
void RGB_Set_Up()
{
	WS2812 = 1;
	//22.1184MHz
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_();// _nop_(); _nop_(); _nop_(); _nop_();
	WS2812 = 0;
}
//高电平400ns 低电平850ns 误差150ns
void RGB_Set_Down()
{
	WS2812 = 1;
	//22.1184MHz
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); //_nop_();
	WS2812 = 0;
}

//·￠?í24??êy?Y
void Send_2811_24bits(unsigned char G8, unsigned char R8, unsigned char B8)
{
	unsigned int n = 0;
	//·￠?íG8??
	for (n = 0; n<8; n++)
	{
		G8 <<= n;
		if (G8 & 0x80 == 0x80)
		{
			RGB_Set_Up();
		}
		else
		{
			RGB_Set_Down();
		}
	}
	//·￠?íR8??
	for (n = 0; n<8; n++)
	{
		R8 <<= n;
		if (R8 & 0x80 == 0x80)
		{
			RGB_Set_Up();
		}
		else
		{
			RGB_Set_Down();
		}
	}
	//·￠?íB8??
	for (n = 0; n<8; n++)
	{
		B8 <<= n;
		if (B8 & 0x80 == 0x80)
		{
			RGB_Set_Up();
		}
		else
		{
			RGB_Set_Down();
		}
	}
}
//?′????
void RGB_Rst()
{
	WS2812 = 0;
	Delay50us();
}
//°?24??êy?YGRB??×aRGB
void Set_Colour(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char i;
	for (i = 0; i<numLEDs; i++)
	{
		buf_R[i] = r; //?o3?
		buf_G[i] = g;
		buf_B[i] = b;
	}
	for (i = 0; i<numLEDs; i++)
	{
		Send_2811_24bits(buf_G[i], buf_R[i], buf_B[i]);//·￠?í??ê?
	}
}
//?3ò???μ???ê?μ???é?
void SetPointColour(unsigned int num, unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char i;
	for (i = 0; i<numLEDs; i++)
	{
		buf_R[num-1] = r;//?o3?
		buf_G[num-1] = g;
		buf_B[num-1] = b;
	}
	for (i = 0; i<numLEDs; i++)
	{
		Send_2811_24bits(buf_G[i], buf_R[i], buf_B[i]);//·￠?í??ê?
	}
}

//颜色交换24位不拆分发

void SetPixelColor(unsigned char num,unsigned long c)

{

	  unsigned char i;

		for(i=0;i<numLEDs;i++)

	  {

			  buf_R[num] = (unsigned char)(c>>16);

			  buf_G[num] = (unsigned char)(c>>8);

			  buf_B[num] = (unsigned char)(c);

		}

		for(i=0;i<numLEDs;i++)

		{

			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);

		}

}
//颜色交换24位不拆分发

void SentPixelColor(unsigned char num,unsigned long c)

{

	  unsigned char i;

		for(i=0;i<numLEDs;i++)

	  {

			  buf_R[num-1] = (unsigned char)(c>>16);

			  buf_G[num-1] = (unsigned char)(c>>8);

			  buf_B[num-1] = (unsigned char)(c);

		}

		for(i=0;i<numLEDs;i++)

		{

			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);

		}

}

 

//复位

void PixelUpdate()

{

	RGB_Rst();

}

//颜色

unsigned long Color(unsigned char r, unsigned char g, unsigned char b)

{

  return ((unsigned long)r << 16) | ((unsigned long)g <<  8) | b;

}

 

//颜色算法

unsigned long Wheel(unsigned char WheelPos)

{

  WheelPos = 255 - WheelPos;

  if(WheelPos < 85) 

	{

    return Color(255 - WheelPos * 3, 0, WheelPos * 3);

  }

  if(WheelPos < 170) {

    WheelPos -= 85;

    return Color(0, WheelPos * 3, 255 - WheelPos * 3);

  }

  WheelPos -= 170;

  return Color(WheelPos * 3, 255 - WheelPos * 3, 0);

}

 

//彩虹

void rainbow(unsigned int wait)

{

  unsigned int i, j;

 

  for(j=0; j<2; j++) 

	{

    for(i=0; i<numLEDs; i++)

		{

      SetPixelColor(i, Wheel((i+j) & 255));

    }

		PixelUpdate();

    HAL_Delay(wait);

  }
  ReSet();
  
}

 

//稍微不同的是，这使得彩虹均匀分布

void rainbowCycle(unsigned int wait) 

{

  unsigned int i, j;

 

  for(j=0;j<256*5;j++) 

	{ // 5 cycles of all colors on wheel  车轮上所有颜色的5个循环

    for(i=0;i<numLEDs;i++) 

	 {

     SetPixelColor(i, Wheel(((i * 256 / numLEDs) + j) & 255));

    }

	  PixelUpdate();

    HAL_Delay (wait);

  }

}

 

//Theatre-style crawling lights.呼吸灯

void theaterChase(unsigned long c, unsigned int wait) 

{

	int j,q;

	unsigned int i;

  for (j=0; j<1; j++) 

	{  //do 10 cycles of chasing  做10个循环

    for (q=0; q < 3; q++) 

		{

      for (i=0; i<numLEDs; i=i+3)

			{

        SetPixelColor(i+q, c);    //turn every third pixel on  把每一个第三个像素

      }

			PixelUpdate();

      HAL_Delay(wait);

 

      for (i=0; i<numLEDs; i=i+3) 

			{

       SetPixelColor(i+q, 0);        //turn every third pixel off   把每一个第三个像素关掉

      }

			PixelUpdate();

    }

  }

}

 

//Theatre-style crawling lights with rainbow effect

//带有彩虹效果的戏剧式爬行灯

void theaterChaseRainbow(unsigned int wait) 

{

	int j,q;

	unsigned int i;

  for (j=0; j < 1; j++) 

	{     // cycle all 256 colors in the wheel 在轮子上循环所有256色

    for (q=0; q < 3; q++)

		{

      for (i=0; i < numLEDs; i=i+3) 

			{

        SetPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel off 把每一个第三个像素

      }

      PixelUpdate();

 

      HAL_Delay(wait);

 

      for (i=0; i < numLEDs; i=i+3)

			{

        SetPixelColor(i+q, 0);        //turn every third pixel off  把每一个第三个像素关掉

      }

    }

  }
	ReSet();
}

// Fill the dots one after the other with a color
//用一种颜色填充这些圆点
void colorWipe(unsigned long c, unsigned int wait)

{

	unsigned int i = 0;

	for (i = 0; i<numLEDs; i++)

	{

		SetPixelColor(i, c);

		PixelUpdate();

		//HAL_Delay(wait);

	}
	ReSet();

}
void zidingyi1()

{



	while (1)

	{

		rainbow(45);

		rainbowCycle(40);

		theaterChase(Color(0, 0, 255), 80); // Blue

		theaterChase(Color(0, 255, 0), 80); // Blue

		theaterChase(Color(255, 0, 0), 80); // Blue

		theaterChaseRainbow(40);

		colorWipe(255, 255);

	}

}
//按下就亮 



void LightEffect8(uint num)
{
	SetPointColour(num, 255, 0, 0);
	Delay1ms(200);
	SetPointColour(num,255,255,255);
	Delay1ms(200);
	SetPointColour(num,0,0,255);
	Delay1ms(200);
	//SentPixelColor(num, 0);
	ReSet();
}
void LightEffect1_1(void)       //左上角第一个按键的灯效
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)		//连续N次颜色递进
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(i, Color(0, 0, i + k * 64 / (N)));	 //第一行
			SentPixelColor(i * 8, Color(0, 0, i + k * 64 / (N)));	 //第一列
		}
	}
	for (i = 0; i<8; i++)	          //clear all
	{
		SentPixelColor(i, 0);
		SentPixelColor(i * 8, 0);
	}
}
void LightEffect1_8(void)		 //右上角第一个按键的灯效
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(7 - i, Color(i + k * 64 / (N), 0, 0));		 //第一行
			SentPixelColor(i * 8 + 7, Color(i + k * 64 / (N), 0, 0));	 //最后一列
		}
	}
	for (i = 0; i<8; i++)	          //clear all
	{
		SentPixelColor(7 - i, 0);
		SentPixelColor(i * 8 + 7, 0);
	}
}
void LightEffect8_1(void)		 //左下角第一个按键的灯效
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(56 + i, Color(0, i + k * 64 / (N), 0));		 //第一行
			SentPixelColor((7 - i) * 8, Color(0, i + k * 64 / (N), 0));	 //最后一列
		}
	}
	for (i = 0; i<8; i++)	          //clear all
	{
		SentPixelColor(56 + i, 0);
		SentPixelColor((7 - i) * 8, 0);
	}
}
void LightEffect8_8(void)		 //右下角第一个按键的灯效
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(63 - i, Color(i + k * 64 / (N), 0, i + k * 64 / (N)));		 //第一行	紫色
			SentPixelColor((7 - i) * 8 + 7, Color(i + k * 64 / (N), 0, i + k * 64 / (N)));	 //最后一列	 紫色
		}
	}
	for (i = 0; i<8; i++)	          //clear all
	{
		SentPixelColor(63 - i, 0);
		SentPixelColor((7 - i) * 8 + 7, 0);
	}
}
int GetNum()
	{
		return rand() % 7;
	}
//=========================================跟随音量产生震动的效果=============================================================
void LightSunShineWithMusic(void)
{
	//    纯白   大红   橙色    浅黄      浅绿      青色       蓝色     紫色     深蓝   
	// R  255   255    255     251      128       116      0        219     14
	// G  255    0     153     255      255       234      0         0      43
	// B  255    0     0        66       0        254      255      219     143
	//返回的是0-6之间的某一个数
	

	//需要：产生一个0-7的随机数的函数，
	int RandNumber;
	int i, j;
	unsigned int XuHao; //存储需要写入的灯的序号
	for (i = 0; i < 8; i++)
	{
		RandNumber = GetNum() + 2;
		for (j = 0; j < 8; j++)
		{
			XuHao = (7 - j) * 8 + i;//得到了那个灯的序号
			if (j = RandNumber - 1)//最高的那个一定亮蓝色，用以区分
			{
				SetPointColour(XuHao, 14, 43, 143);//最高点一定亮深蓝色
			}
			else
			{
				if (j >= RandNumber)
				{
					SetPointColour(XuHao, 255, 255, 255);//纯白色

				}
				else
				{
					switch (j)
					{
					case 0:SetPointColour(XuHao, 219, 0, 219); break;   //紫色
					case 1:SetPointColour(XuHao, 0, 0, 255); break;     //蓝色
					case 2:SetPointColour(XuHao, 116, 234, 254); break; //青色
					case 3:SetPointColour(XuHao, 128, 255, 0); break;   //浅绿
					case 4:SetPointColour(XuHao, 255, 153, 0); break;   //橙色
					case 5:SetPointColour(XuHao, 251, 255, 66); break;  //黄色
					case 6:SetPointColour(XuHao, 255, 0, 0); break;     //红色
					default:
						break;
					}
				}
			}
		}
	}
}
void LightEffect2(unsigned char time, unsigned int fre)
{
	uint i;
	double pertime = (double)time / fre;
	for (i = 0; i < fre; i++)
	{
		LightSunShineWithMusic();
		Delay1ms(pertime);
	}
}
void LightEffect1(unsigned char time, unsigned int j)
{
	int i;
unsigned char  R[6][64] = { 
	{
	000,000,000,000,000,000,000,000,
	000,255,255,000,000,255,255,000,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	000,255,255,255,255,255,000,000,
	000,000,255,255,255,255,000,000,
	000,000,000,255,255,000,000,000},

	{000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,255,000,000,255,000,000,
	000,255,253,255,255,253,255,000,
	000,255,253,253,253,253,255,000,
	000,000,255,253,253,255,000,000,
	000,000,000,255,255,000,000,000,
	000,000,000,000,000,000,000,000},
	
	{000,000,000,000,000,000,000,000,
	000,000,000,053,000,000,000,000,
	000,000,053,060,053,000,000,000,
	000,053, 28,060,060,053,000,000,
	000,000,053, 28,060,060,053,000,
	000,000,000,053,28,053,000,000,
	000,000,000,000,053,000,000,000,
	000,000,000,000,000,000,000,000},

	{000,000,000,003,003,000,000,000,
	000,000,001,001,001,001,000,000,
	000,197,197,197,197,197,197,000,
	254,254,254,254,254,254,254,254,
	000,197,197,197,197,197,197,000,
	000,000,001,001,001,001,000,000,
	000,000,000,003,003,000,000,000,
	000,000,000,000,000,000,000,000},

	{255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,000,000,000,000,255,255,
	255,255,000,000,000,000,255,255,
	255,255,000,000,000,000,255,255,
	255,255,000,000,000,000,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255},
	
	{000,000,000,000,000,000,000,000,
	000,000,003,003,003,003,000,000,
	000,003,242,233,233,221,003,000,
	000,003,233,242,221,233,003,000,
	000,003,233,221,242,233,003,000,
	000,003,221,233,233,242,003,000,
	000,000,003,003,003,003,000,000,
	000,000,000,000,000,000,000,000} 
};


unsigned char G[6][64] = {
	{000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,128,000,000,128,000,000,
	000,000,128,128,128,128,000,000,
	000,000,000,128,128,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000},
	
	{000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,128,000,000,128,000,000,
	000,128,197,128,128,197,128,000,
	000,128,197,197,197,197,128,000,
	000,000,128,197,197,128,000,000,
	000,000,000,128,128,000,000,000,
	000,000,000,000,000,000,000,000},
	
	{000,000,000,255,000,000,000,000,
	000,000,255,202,255,000,000,000,
	000,255,202,185,202,255,000,000,
	255,202,203,185,185,202,255,000,
	000,255,202,203,185,185,202,255,
	000,000,255,202,203,202,255,000,
	000,000,000,255,202,255,000,000,
	000,000,000,000,255,000,000,000},

	{000,000,000,252,252,000,000,000,
	000,000,254,254,254,254,000,000,
	000,254,254,254,254,254,254,000,
	166,166,166,166,166,166,166,166,
	000,254,254,254,254,254,254,000,
	000,000,254,254,254,254,000,000,
	000,000,000,252,252,000,000,000,
	000,000,000,000,000,000,000,000},

	{128,128,128,128,128,128,128,128,
	128,255,255,255,255,255,255,128,
	128,255,255,255,255,255,255,128,
	128,255,255,255,255,255,255,128,
	128,255,255,255,255,255,255,128,
	128,255,255,255,255,255,255,128,
	128,255,255,255,255,255,255,128,
	128,128,128,128,128,128,128,128},
	
	{000,000,032,032,032,032,000,000,
	000,032,252,252,252,252,032,000,
	032,252,128,233,233, 38,252,032,
	032,252,233,128, 38,233,252,032,
	032,252,233, 38,128,233,252,032,
	032,252, 38,233,233,128,252,032,
	000,032,252,252,252,252,032,000,
	000,000,032,032,032,032,000,000} 
};
unsigned char B[6][64] = { 
	{000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,128,000,000,128,000,000,
	000,128,192,128,128,192,128,000,
	000,128,192,192,192,192,128,000,
	000,000,128,192,192,128,000,000,
	000,000,000,128,128,000,000,000,
	000,000,000,000,000,000,000,000},

	{000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,066,000,000,066,000,000,
	000,000,066,066,066,066,000,000,
	000,000,000,066,066,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000},
	
	{000,000,000,000,000,000,000,000,
	000,000,000, 98,000,000,000,000,
	000,000, 98,196,98,000,000,000,
	000, 98,227,196,196,98,000,000,
	000,000, 98,227,196,196,98,000,
	000,000,000, 98,227,98,000,000,
	000,000,000,000, 98,000,000,000,
	000,000,000,000,000,000,000,000},

	{000,000,000,152,152,000,000,000,
	000,000,033,033,033,033,000,000,
	000,001,001,001,001,001,001,000,
	001,001,001,001,001,001,001,001,
	001,001,001,001,001,001,001,001,
	000,001,001,001,001,001,001,000,
	000,000,033,033,033,033,000,000,
	000,000,000,152,152,000,000,000},

	{000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,255,255,000,000,000,
	000,000,000,255,255,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000},

	{000,000,255,255,255,255,000,000,
	000,255,103,103,103,103,255,000,
	255,103,013,022,022,034,103,255,
	255,103,022,013,034,022,103,255,
	255,103,022,034,013,022,103,255,
	255,103,034,022,022,013,103,255,
	000,255,103,103,103,103,255,000,
	000,000,255,255,255,255,000,000} 
};

for (i = 0; i<63; i++) {
	SetPointColour(i, 0, 0, 0);
	SetPointColour(i, R[j][i], G[j][i], B[j][i]);
	HAL_Delay(time);
}
	ReSet();
					}
/*
void LightEffect1111(unsigned char time, unsigned int j)
{
	int i;
	uchar R1[]={000,000,000,000,000,000,000,000,	
     	      000,006,006,000,000,006,006,000,
                      006,121,121,006,006,121,121,006,
	      006,121,29,121,121,29,121,006,									
	      006,121,29,29,29,121,121,006,
                      000,006,121,29,29,121,006,000,
	      000,000,006,121,121,006,000,000,
	      000,000,000,006,006,000,000,000};
	uchar G1[]={000,000,000,000,000,000,000,000,	
     	      000,249,249,000,000,249,249,000,
                      249,001,001,249,249,001,001,249,
	      249,001,167,001,001,167,001,249,									
	      249,001,167,167,167,167,001,249,
                      000,249,001,167,167,001,249,000,
	      000,000,249,001,001,249,000,000,
	      000,000,000,249,249,000,000,000};
	uchar B1[]={000,000,000,000,000,000,000,000,	
     	      000,242,242,000,000,242,242,000,
                      242,254,254,242,242,254,254,242,
	      242,254,226,254,254,226,254,242,									
	      242,254,226,226,226,226,254,242,
                      000,242,254,226,226,254,242,000,
	      000,000,242,254,254,242,000,000,
	      000,000,000,242,242,000,000,000}; //large heart  blue purple	

	uchar R2[]={000,000,000,000,000,000,000,000,	
     	      000,255,255,000,000,255,255,000,
                      255,255,255,255,255,255,255,255,
	      255,255,255,255,255,255,255,255,									
	      255,255,255,255,255,255,255,255,
                      000,255,255,255,255,255,000,000,
	      000,000,255,255,255,255,000,000,
	      000,000,000,255,255,000,000,000};
	uchar G2[]={000,000,000,000,000,000,000,000,	
     	      000,128,128,000,000,128,128,000,
                      128,128,128,128,128,128,128,128,
	      128,128,128,128,128,128,128,128,									
	      128,128,128,128,128,128,128,128,
                      000,128,128,128,128,128,128,000,
	      000,000,128,128,128,128,000,000,
	      000,000,000,128,128,000,000,000};
	uchar B2[]={000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,									
	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};  //large heart  orange
	
	uchar R3[]={000,000,000,000,000,000,000,000	,
     	      000,000,255,000,000,000,255,000,
                      000,255,000,255,000,255,000,255,
	      000,000,000,000,000,000,000,000		,							
	      000,000,000,255,000,255,000,000,
                      000,000,000,000,255,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};	
	uchar G3[]={000,000,000,000,000,000,000,000	,
     	      000,000,255,000,000,000,255,000,
                      000,255,000,255,000,255,000,255,
	      000,000,000,000,000,000,000,000		,							
	      000,000,000,255,000,255,000,000,
                      000,000,000,000,255,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};	
	uchar B3[]={000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000  ,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000				,					
	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};	  //笑脸


	uchar R4[]={255,000,000,000,000,000,000,255,	
     	      000,255,000,000,000,000,255,000,
                      000,000,255,000,000,255,000,000,
	      000,000,000,146,146,000,000,000				,					
	      000,000,000,146,146,000,000,000,
                      000,000,255,000,000,255,000,000,
	      000,255,000,000,000,000,255,000,
	      255,000,000,000,000,000,000,255};	
	uchar G4[]={000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,036,036,000,000,000				,					
	      000,000,000,036,036,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};	
	uchar B4[]={128,000,000,000,000,000,000,128,	
     	      000,128,000,000,000,000,128,000,
                      000,000,128,000,000,128,000,000,
	      000,000,000,219,219,000,000,000				,					
	      000,000,000,219,219,000,000,000,
                      000,000,128,000,000,128,000,000,
	      000,128,000,000,000,000,128,000,
	      128,000,000,000,000,000,000,128};    //“X图案

	uchar R5[]={064,000,064,000,000,064,064,064,	
     	      064,000,064,000,000,064,000,064,
                      064,064,064,000,000,064,064,064,
	      000,000,000,242,242,000,000,000				,					
	      000,000,000,242,242,000,000,000,
                      064,064,064,000,000,064,064,064,
	      064,000,064,000,000,064,000,064,
	      064,064,064,000,000,064,064,064};	
	uchar G5[]={064,064,064,000,000,064,064,064,	
     	      064,000,064,000,000,064,000,064,
                      064,064,064,000,000,064,064,064,
	      000,000,000,064,064,000,000,000				,					
	      000,000,000,064,064,000,000,000,
                      064,064,064,000,000,064,064,064  ,
	      064,000,064,000,000,064,000,064,
	      064,000,064,000,000,064,064,064};	
	uchar B5[]={064,000,064,000,000,064,064,064,	
     	      064,000,064,000,000,064,000,064,
                      064,064,064,000,000,064,064,064,
	      000,000,000,126,126,000,000,000				,					
	      000,000,000,126,126,000,000,000,
                      064,064,064,000,000,064,064,064,
	      064,000,064,000,000,064,000,064  ,
	      064,064,064,000,000,064,000,064};   //五口图案

	uchar R6[]={000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000				,					
	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};	
	uchar G6[]={000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
					  000,000,000,000,000,000,000,000,									
	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};	
	uchar B6[]={000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000				,					
	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000};
	uchar R[5][64]={R1,R2,R3,R4,R5};
	uchar G[5][64]={G1,G2,G3,G4,G5};
	uchar B[5][64]={B1,B2,B3,B4,B5};
	
for (i = 0; i<63; i++) {
	SetPointColour(i, 0, 0, 0);
	SetPointColour(i, R[j][i], G[j][i], B[j][i]);
	HAL_Delay(time);
}
					}
*/

void LightControll(uchar keyvalue)
						{
							switch (keyvalue)
							{
							case '\x00':LightEffect8_1(); break;
							case '\x01':theaterChase(Color(0, 0, 255), 10); break;
							case '\x02':theaterChase(Color(0, 0, 255), 10); break;
							case '\x03':LightEffect1(20,0); break;
							case '\x04': LightEffect1(20,1);break;
							case '\x05':theaterChase(Color(0, 0, 255), 10); break;
							case '\x06':theaterChase(Color(0, 0, 255), 10); break;
							case '\x07':LightEffect1_1(); break;
							case '\x08':rainbow(40); break;
							case '\t':LightEffect8(9); break;
							case '\n':LightEffect8(10); break;
							case '\x0b':LightEffect8(11); break;
							case '\x0c':LightEffect8(12); break;
							case '\r':LightEffect8(13); break;
							case '\x0e':LightEffect8(14); break;
							case '\x0f':theaterChaseRainbow(40); break;
							case '\x10':rainbow(40); break;
							case '\x11':LightEffect8(17); break;
							case '\x12':LightEffect8(18); break;
							case '\x13':LightEffect8(19); break;
							case '\x14':LightEffect8(20); break;
							case '\x15':LightEffect8(21); break;
							case '\x16':LightEffect8(22); break;
							case '\x17':theaterChaseRainbow(40); break;
							case '\x18':LightEffect1(20,2); break;
							case '\x19':LightEffect8(25); break;
							case '\x1a':LightEffect1(20, 0); break;
							case '\x1b':LightEffect1(10, 1); break;
							case '\x1c':LightEffect1(10, 2); break;
							case '\x1d':LightEffect1(20, 3); break;
							case '\x1e':LightEffect8(30); break;
							case '\x1f':LightEffect1(20,3); break;
							case ' ':LightEffect1_1(); break;
							case '!':LightEffect8(33); break;
							case '"':LightEffect1(20, 4); break;
							case '#':LightEffect1(10, 5); break;
							case '$':LightEffect1(10, 6); break;
							case '%':LightEffect1(20, 7); break;
							case '&':LightEffect8(38); break;
							case '\'':LightEffect1(20,4); break;
							case '(':rainbow(40); break;
							case ')':LightEffect8(41); break;
							case '*':LightEffect8(42); break;
							case '+':LightEffect8(43); break;
							case ',':LightEffect8(44); break;
							case '-':LightEffect8(45); break;
							case '.':LightEffect8(46); break;
							case '/':theaterChaseRainbow(40); break;
							case '0':rainbow(40); break;
							case '1':LightEffect8(49); break;
							case '2':LightEffect8(50); break;
							case '3':LightEffect8(51); break;
							case '4':LightEffect8(52); break;
							case '5':LightEffect8(53); break;
							case '6':LightEffect8(54); break;
							case '7':theaterChaseRainbow(40); break;
							case '8':LightEffect8_8(); break;
							case '9':colorWipe(255, 255); break;
							case ':':colorWipe(255, 255); break;
							case ';':LightEffect1(20,5); break;
							case '<':LightEffect1(20,4); break;
							case '=':colorWipe(255, 255); break;
							case '>':colorWipe(255, 255); break;
							case '?':LightEffect1_8(); break;
							default:break;
							}
						}







//===================================利用7279来进行键盘的读取处理===========================================


//=========================================================================================
void long_delay(void)
   {
   uchar i;
   for (i=0;i<0x30;i++){;}    //00110000
   }

void short_delay(void)
   {
   uchar i;
   for (i=0;i<8;i++){;}
   }		


//单片机向7279发送数据
void SendByteTo7279(uchar out_byte)
   {
   uchar i;
   cs = 0; long_delay();//片选信号为低电平有效
   for (i=0;i<8;i++)
      {
	  if (out_byte & 0x80) { dat = 1; } //10000000
	  else { dat = 0; }                        //读取每一位的数据存放在dat中
	  //相当于产生时钟信号，制造了一个下降沿,从而进行数据的发送
	  clk = 1; short_delay(); clk = 0; short_delay();
	  out_byte = out_byte*2;
      }
   dat = 0;
   }
   
//7279向单片机发送数据
unsigned char ReceivedByteToMCU()
{
	unsigned char i, in_byte;
	dat = 1;   
	long_delay(); in_byte = 0;
	for (i=0;i<8;i++)
	{
		clk = 1; short_delay(); in_byte = in_byte*2;
		if (dat) { in_byte = in_byte|0x01; }
		clk = 0; short_delay();
	}
	dat = 0; 
	return(in_byte);
}

// 读取键值
unsigned char key_read()
{
	SendByteTo7279(0x15); //15H是7279中的读键盘数据指令
	return(ReceivedByteToMCU());
}

//======================
	

void delayms(uint x)
{
	uint i;
	uint j;
	for(i=0;i<x;i++)
		for(j=0;j<335;j++)
			{;}
}
void SendByte2Computer(uchar dataa)
{
	SBUF=dataa;
	while(TI==0);
	
	TI=0;//软件复位
}
void SendBytes2Computer(unsigned char * datas)
{
	//int i;
	uchar* dd=datas;
	while( *dd != '\0')
	//for(i=0;i<3;i++)
	{
		SendByte2Computer(*dd);
		dd+=1;
		//SendByte(datas[i]);
		
		
	}
}

void UartInit(void)		//9600bps@18.432MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x20;		//设定定时初值
	TH1 = 0xFE;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}



void Delay1ms(uchar k)		//@18.432MHz
{
	unsigned char i, j,mm;
	for(mm=0;mm<k;mm++){
	_nop_();
	_nop_();
	i = 22;
	j = 128;
	do
	{
		while (--j);
	} while (--i);
}}




void main()
{
unsigned char KeyValue;
UartInit();

//rainbowCycle(40);
while(1)
{
	
	//RGB_Set_Up();
	//RGB_Set_Down();
	//SetPointColour(2,255,0,0);
	
	KeyValue=key_read();
	SendByte2Computer(KeyValue);
	LightControll(KeyValue);
	Delay1ms(100);



}



}

//适用于18.432MHz，此时NOP占54ns
//R7相当于dataa,R6用来进行循环，RLC负责移位
void ws2812asm(unsigned char dataa)
{
#pragma asm
	MOV A, R7       
	MOV R6, #0x08   
WS2812LOOP:
	SETB P1.3
	RLC A
	NOP
	MOV P1.3, C
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	CLR P1.3
	NOP
	NOP
	DJNZ R6, WS2812LOOP
#pragma endasm
}






