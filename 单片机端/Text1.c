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


unsigned char  buf_R[numLEDs] = { 0 };//??��??o��?
unsigned char  buf_G[numLEDs] = { 0 };
unsigned char  buf_B[numLEDs] = { 0 };
void RGB_Set_Up();  //?��0??
void RGB_Set_Down(); //?��1??
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


//?��???������
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
// �ߵ�ƽ850ns �͵�ƽ400ns ���150ns
void RGB_Set_Up()
{
	WS2812 = 1;
	//22.1184MHz
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_();// _nop_(); _nop_(); _nop_(); _nop_();
	WS2812 = 0;
}
//�ߵ�ƽ400ns �͵�ƽ850ns ���150ns
void RGB_Set_Down()
{
	WS2812 = 1;
	//22.1184MHz
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); //_nop_();
	WS2812 = 0;
}

//����?��24??��y?Y
void Send_2811_24bits(unsigned char G8, unsigned char R8, unsigned char B8)
{
	unsigned int n = 0;
	//����?��G8??
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
	//����?��R8??
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
	//����?��B8??
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
//?��????
void RGB_Rst()
{
	WS2812 = 0;
	Delay50us();
}
//��?24??��y?YGRB??��aRGB
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
		Send_2811_24bits(buf_G[i], buf_R[i], buf_B[i]);//����?��??��?
	}
}
//?3��???��???��?��???��?
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
		Send_2811_24bits(buf_G[i], buf_R[i], buf_B[i]);//����?��??��?
	}
}

//��ɫ����24λ����ַ�

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
//��ɫ����24λ����ַ�

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

 

//��λ

void PixelUpdate()

{

	RGB_Rst();

}

//��ɫ

unsigned long Color(unsigned char r, unsigned char g, unsigned char b)

{

  return ((unsigned long)r << 16) | ((unsigned long)g <<  8) | b;

}

 

//��ɫ�㷨

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

 

//�ʺ�

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

 

//��΢��ͬ���ǣ���ʹ�òʺ���ȷֲ�

void rainbowCycle(unsigned int wait) 

{

  unsigned int i, j;

 

  for(j=0;j<256*5;j++) 

	{ // 5 cycles of all colors on wheel  ������������ɫ��5��ѭ��

    for(i=0;i<numLEDs;i++) 

	 {

     SetPixelColor(i, Wheel(((i * 256 / numLEDs) + j) & 255));

    }

	  PixelUpdate();

    HAL_Delay (wait);

  }

}

 

//Theatre-style crawling lights.������

void theaterChase(unsigned long c, unsigned int wait) 

{

	int j,q;

	unsigned int i;

  for (j=0; j<1; j++) 

	{  //do 10 cycles of chasing  ��10��ѭ��

    for (q=0; q < 3; q++) 

		{

      for (i=0; i<numLEDs; i=i+3)

			{

        SetPixelColor(i+q, c);    //turn every third pixel on  ��ÿһ������������

      }

			PixelUpdate();

      HAL_Delay(wait);

 

      for (i=0; i<numLEDs; i=i+3) 

			{

       SetPixelColor(i+q, 0);        //turn every third pixel off   ��ÿһ�����������عص�

      }

			PixelUpdate();

    }

  }

}

 

//Theatre-style crawling lights with rainbow effect

//���вʺ�Ч����Ϸ��ʽ���е�

void theaterChaseRainbow(unsigned int wait) 

{

	int j,q;

	unsigned int i;

  for (j=0; j < 1; j++) 

	{     // cycle all 256 colors in the wheel ��������ѭ������256ɫ

    for (q=0; q < 3; q++)

		{

      for (i=0; i < numLEDs; i=i+3) 

			{

        SetPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel off ��ÿһ������������

      }

      PixelUpdate();

 

      HAL_Delay(wait);

 

      for (i=0; i < numLEDs; i=i+3)

			{

        SetPixelColor(i+q, 0);        //turn every third pixel off  ��ÿһ�����������عص�

      }

    }

  }
	ReSet();
}

// Fill the dots one after the other with a color
//��һ����ɫ�����ЩԲ��
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
//���¾��� 



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
void LightEffect1_1(void)       //���Ͻǵ�һ�������ĵ�Ч
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)		//����N����ɫ�ݽ�
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(i, Color(0, 0, i + k * 64 / (N)));	 //��һ��
			SentPixelColor(i * 8, Color(0, 0, i + k * 64 / (N)));	 //��һ��
		}
	}
	for (i = 0; i<8; i++)	          //clear all
	{
		SentPixelColor(i, 0);
		SentPixelColor(i * 8, 0);
	}
}
void LightEffect1_8(void)		 //���Ͻǵ�һ�������ĵ�Ч
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(7 - i, Color(i + k * 64 / (N), 0, 0));		 //��һ��
			SentPixelColor(i * 8 + 7, Color(i + k * 64 / (N), 0, 0));	 //���һ��
		}
	}
	for (i = 0; i<8; i++)	          //clear all
	{
		SentPixelColor(7 - i, 0);
		SentPixelColor(i * 8 + 7, 0);
	}
}
void LightEffect8_1(void)		 //���½ǵ�һ�������ĵ�Ч
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(56 + i, Color(0, i + k * 64 / (N), 0));		 //��һ��
			SentPixelColor((7 - i) * 8, Color(0, i + k * 64 / (N), 0));	 //���һ��
		}
	}
	for (i = 0; i<8; i++)	          //clear all
	{
		SentPixelColor(56 + i, 0);
		SentPixelColor((7 - i) * 8, 0);
	}
}
void LightEffect8_8(void)		 //���½ǵ�һ�������ĵ�Ч
{
	int i, k;
	for (k = 0; k<8 * N; k = k + 8)
	{
		for (i = 0; i<8; i++)
		{
			SentPixelColor(63 - i, Color(i + k * 64 / (N), 0, i + k * 64 / (N)));		 //��һ��	��ɫ
			SentPixelColor((7 - i) * 8 + 7, Color(i + k * 64 / (N), 0, i + k * 64 / (N)));	 //���һ��	 ��ɫ
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
//=========================================�������������𶯵�Ч��=============================================================
void LightSunShineWithMusic(void)
{
	//    ����   ���   ��ɫ    ǳ��      ǳ��      ��ɫ       ��ɫ     ��ɫ     ����   
	// R  255   255    255     251      128       116      0        219     14
	// G  255    0     153     255      255       234      0         0      43
	// B  255    0     0        66       0        254      255      219     143
	//���ص���0-6֮���ĳһ����
	

	//��Ҫ������һ��0-7��������ĺ�����
	int RandNumber;
	int i, j;
	unsigned int XuHao; //�洢��Ҫд��ĵƵ����
	for (i = 0; i < 8; i++)
	{
		RandNumber = GetNum() + 2;
		for (j = 0; j < 8; j++)
		{
			XuHao = (7 - j) * 8 + i;//�õ����Ǹ��Ƶ����
			if (j = RandNumber - 1)//��ߵ��Ǹ�һ������ɫ����������
			{
				SetPointColour(XuHao, 14, 43, 143);//��ߵ�һ��������ɫ
			}
			else
			{
				if (j >= RandNumber)
				{
					SetPointColour(XuHao, 255, 255, 255);//����ɫ

				}
				else
				{
					switch (j)
					{
					case 0:SetPointColour(XuHao, 219, 0, 219); break;   //��ɫ
					case 1:SetPointColour(XuHao, 0, 0, 255); break;     //��ɫ
					case 2:SetPointColour(XuHao, 116, 234, 254); break; //��ɫ
					case 3:SetPointColour(XuHao, 128, 255, 0); break;   //ǳ��
					case 4:SetPointColour(XuHao, 255, 153, 0); break;   //��ɫ
					case 5:SetPointColour(XuHao, 251, 255, 66); break;  //��ɫ
					case 6:SetPointColour(XuHao, 255, 0, 0); break;     //��ɫ
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
	      000,000,000,000,000,000,000,000};	  //Ц��


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
	      128,000,000,000,000,000,000,128};    //��Xͼ��

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
	      064,064,064,000,000,064,000,064};   //���ͼ��

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







//===================================����7279�����м��̵Ķ�ȡ����===========================================


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


//��Ƭ����7279��������
void SendByteTo7279(uchar out_byte)
   {
   uchar i;
   cs = 0; long_delay();//Ƭѡ�ź�Ϊ�͵�ƽ��Ч
   for (i=0;i<8;i++)
      {
	  if (out_byte & 0x80) { dat = 1; } //10000000
	  else { dat = 0; }                        //��ȡÿһλ�����ݴ����dat��
	  //�൱�ڲ���ʱ���źţ�������һ���½���,�Ӷ��������ݵķ���
	  clk = 1; short_delay(); clk = 0; short_delay();
	  out_byte = out_byte*2;
      }
   dat = 0;
   }
   
//7279��Ƭ����������
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

// ��ȡ��ֵ
unsigned char key_read()
{
	SendByteTo7279(0x15); //15H��7279�еĶ���������ָ��
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
	
	TI=0;//�����λ
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
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0x20;		//�趨��ʱ��ֵ
	TH1 = 0xFE;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
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

//������18.432MHz����ʱNOPռ54ns
//R7�൱��dataa,R6��������ѭ����RLC������λ
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






