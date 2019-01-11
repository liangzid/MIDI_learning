#include <reg51.h>
#include "intrins.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define numLEDs 64   //�Ƶĸ���
sbit WS2812 = P1^3;

/*�ƹ���ƺ��������ݷ���ɨ���ֵ���Ƶ�Ч����64����*/

unsigned char buf_R[numLEDs] = { 0 };//��ɫ����
unsigned char buf_G[numLEDs] = { 0 };
unsigned char buf_B[numLEDs] = { 0 };
void RGB_Set_Up();  //��0��
void RGB_Set_Down(); //��1��
void HAL_Delay(unsigned in t)
{
	unsigned int x, y;
	for (x = 114; x>0; x--)
		for (y = t; y>0; y--);
}
//��λ��ʱ
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
//1�룬�ߵ�ƽ850ns �͵�ƽ400ns �������150ns
void RGB_Set_Up()
{
	WS2812 = 1;
	//�����߼������ǵ��Եĵ���ʱ
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	WS2812 = 0;
}
//1�룬�ߵ�ƽ400ns �͵�ƽ850ns �������150ns
void RGB_Set_Down()
{
	WS2812 = 1;
	//�����߼������ǵ��Եĵ���ʱ
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	WS2812 = 0;
}

//����24λ����
void Send_2811_24bits(unsigned char G8, unsigned char R8, unsigned char B8)
{
	unsigned int n = 0;
	//����G8λ
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
	//����R8λ
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
	//����B8λ
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
//��λ��
void RGB_Rst()
{
	WS2812 = 0;
	Delay50us();
}
//��24λ����GRB��תRGB
void Set_Colour(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char i;
	for (i = 0; i<numLEDs; i++)
	{
		buf_R[i] = r; //����
		buf_G[i] = g;
		buf_B[i] = b;
	}
	for (i = 0; i<numLEDs; i++)
	{
		Send_2811_24bits(buf_G[i], buf_R[i], buf_B[i]);//������ʾ
	}
}
//ĳһ������ʾ����ɫ
void SetPointColour(unsigned int num, unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char i;
	for (i = 0; i<numLEDs; i++)
	{
		buf_R[num] = r;//����
		buf_G[num] = g;
		buf_B[num] = b;
	}
	for (i = 0; i<numLEDs; i++)
	{
		Send_2811_24bits(buf_G[i], buf_R[i], buf_B[i]);//������ʾ
	}
}

//��ɫ����24λ����ַ�
void SetPixelColor(unsigned char num, unsigned long c)
{
	unsigned char i;
	for (i = 0; i<numLEDs; i++)
	{
		buf_R[num] = (unsigned char)(c >> 16);
		buf_G[num] = (unsigned char)(c >> 8);
		buf_B[num] = (unsigned char)(c);
	}
	for (i = 0; i<numLEDs; i++)
	{
		Send_2811_24bits(buf_G[i], buf_R[i], buf_B[i]);
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
	return ((unsigned long)r << 16) | ((unsigned long)g << 8) | b;
}

//��ɫ�㷨
unsigned long Wheel(unsigned char WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
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



	for (j = 0; j<256; j++)

	{

		for (i = 0; i<numLEDs; i++)

		{

			SetPixelColor(i, Wheel((i + j) & 255));

		}

		PixelUpdate();

		HAL_Delay(wait);

	}

}



//��΢��ͬ���ǣ���ʹ�òʺ���ȷֲ�

void rainbowCycle(unsigned int wait)

{

	unsigned int i, j;



	for (j = 0; j<256 * 5; j++)

	{ // 5 cycles of all colors on wheel  ������������ɫ��5��ѭ��

		for (i = 0; i<numLEDs; i++)

		{

			SetPixelColor(i, Wheel(((i * 256 / numLEDs) + j) & 255));

		}

		PixelUpdate();

		HAL_Delay(wait);

	}

}



//Theatre-style crawling lights.������

void theaterChase(unsigned long c, unsigned int wait)

{

	int j, q;

	unsigned int i;

	for (j = 0; j<10; j++)

	{  //do 10 cycles of chasing  ��10��ѭ��

		for (q = 0; q < 3; q++)

		{

			for (i = 0; i<numLEDs; i = i + 3)

			{

				SetPixelColor(i + q, c);    //turn every third pixel on  ��ÿһ������������

			}

			PixelUpdate();

			HAL_Delay(wait);



			for (i = 0; i<numLEDs; i = i + 3)

			{

				SetPixelColor(i + q, 0);        //turn every third pixel off   ��ÿһ�����������عص�

			}

			PixelUpdate();

		}

	}

}



//Theatre-style crawling lights with rainbow effect

//���вʺ�Ч����Ϸ��ʽ���е�

void theaterChaseRainbow(unsigned int wait)

{

	int j, q;

	unsigned int i;

	for (j = 0; j < 256; j++)

	{     // cycle all 256 colors in the wheel ��������ѭ������256ɫ

		for (q = 0; q < 3; q++)

		{

			for (i = 0; i < numLEDs; i = i + 3)

			{

				SetPixelColor(i + q, Wheel((i + j) % 255));    //turn every third pixel off ��ÿһ������������

			}

			PixelUpdate();



			HAL_Delay(wait);



			for (i = 0; i < numLEDs; i = i + 3)

			{

				SetPixelColor(i + q, 0);        //turn every third pixel off  ��ÿһ�����������عص�

			}

		}

	}

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

		HAL_Delay(wait);

	}

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


//------------------------------------------------------------------------------------------------------------------------------------------------------
void LightEffect8(num)
{
	SetPixelColor(num, Color(244, 138, 242));
	delay();
	SetPixelColor(num, 0);
}
void LightEffect1_1(void)       //���Ͻǵ�һ�������ĵ�Ч
{
	int i, j, k;
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
	int i, k, N = 64;
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
//=========================================�������������𶯵�Ч��=============================================================
void LightSunShineWithMusic(void)
{
	//    ����   ���   ��ɫ    ǳ��      ǳ��      ��ɫ       ��ɫ     ��ɫ     ����   
	// R  255   255    255     251      128       116      0        219     14
	// G  255    0     153     255      255       234      0         0      43
	// B  255    0     0        66       0        254      255      219     143
	//���ص���0-6֮���ĳһ����
	int GetNum()
	{
		return rand() % 7;
	}

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
		DDelay_ms(pertime);
	}
}
void LightEffect1(unsigned char time, unsigned int j)
unsigned char R[][] = { [000,000,000,000,000,000,000,000,
000,255,255,000,000,255,255,000,
255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,
000,255,255,255,255,255,000,000,
000,000,255,255,255,255,000,000,
000,000,000,255,255,000,000,000],[000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,255,000,000,255,000,000,
000,255,253,255,255,253,255,000,
000,255,253,253,253,253,255,000,
000,000,255,253,253,255,000,000,
000,000,000,255,255,000,000,000,
000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,
000,000,000,053,000,000,000,000,
000,000,053,060,053,000,000,000,
000,053,028,060,060,053,000,000,
000,000,053,028,060,060,053,000,
000,000,000,053,028,053,000,000,
000,000,000,000,053,000,000,000,
000,000,000,000,000,000,000,000],[000,000,000,003,003,000,000,000,
000,000,001,001,001,001,000,000,
000,197,197,197,197,197,197,000,
254,254,254,254,254,254,254,254,
000,197,197,197,197,197,197,000,
000,000,001,001,001,001,000,000,
000,000,000,003,003,000,000,000,
000,000,000,000,000,000,000,000],[255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,
255,255,000,000,000,000,255,255,
255,255,000,000,000,000,255,255,
255,255,000,000,000,000,255,255,
255,255,000,000,000,000,255,255,
255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255],[000,000,000,000,000,000,000,000,
000,000,003,003,003,003,000,000,
000,003,242,233,233,221,003,000,
000,003,233,242,221,233,003,000,
000,003,233,221,242,233,003,000,
000,003,221,233,233,242,003,000,
000,000,003,003,003,003,000,000,
000,000,000,000,000,000,000,000] };
unsigned char G[][] = { [000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,128,000,000,128,000,000,
000,000,128,128,128,128,000,000,
000,000,000,128,128,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,128,000,000,128,000,000,
000,128,197,128,128,197,128,000,
000,128,197,197,197,197,128,000,
000,000,128,197,197,128,000,000,
000,000,000,128,128,000,000,000,
000,000,000,000,000,000,000,000],[000,000,000,255,000,000,000,000,
000,000,255,202,255,000,000,000,
000,255,202,185,202,255,000,000,
255,202,203,185,185,202,255,000,
000,255,202,203,185,185,202,255,
000,000,255,202,203,202,255,000,
000,000,000,255,202,255,000,000,
000,000,000,000,255,000,000,000],[000,000,000,252,252,000,000,000,
000,000,254,254,254,254,000,000,
000,254,254,254,254,254,254,000,
166,166,166,166,166,166,166,166,
000,254,254,254,254,254,254,000,
000,000,254,254,254,254,000,000,
000,000,000,252,252,000,000,000,
000,000,000,000,000,000,000,000],[128,128,128,128,128,128,128,128,
128,255,255,255,255,255,255,128,
128,255,255,255,255,255,255,128,
128,255,255,255,255,255,255,128,
128,255,255,255,255,255,255,128,
128,255,255,255,255,255,255,128,
128,255,255,255,255,255,255,128,
128,128,128,128,128,128,128,128],[000,000,032,032,032,032,000,000,
000,032,252,252,252,252,032,000,
032,252,128,233,233,038,252,032,
032,252,233,128,038,233,252,032,
032,252,233,038,128,233,252,032,
032,252,038,233,233,128,252,032,
000,032,252,252,252,252,032,000,
000,000,032,032,032,032,000,000] };
unsigned char B[][] = { [000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,128,000,000,128,000,000,
000,128,192,128,128,192,128,000,
000,128,192,192,192,192,128,000,
000,000,128,192,192,128,000,000,
000,000,000,128,128,000,000,000,
000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,066,000,000,066,000,000,
000,000,066,066,066,066,000,000,
000,000,000,066,066,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,
000,000,000,098,000,000,000,000,
000,000,098,196,098,000,000,000,
000,098,227,196,196,098,000,000,
000,000,098,227,196,196,098,000,
000,000,000,098,227,098,000,000,
000,000,000,000,098,000,000,000,
000,000,000,000,000,000,000,000],[000,000,000,152,152,000,000,000,
000,000,033,033,033,033,000,000,
000,001,001,001,001,001,001,000,
001,001,001,001,001,001,001,001,
001,001,001,001,001,001,001,001,
000,001,001,001,001,001,001,000,
000,000,033,033,033,033,000,000,
000,000,000,152,152,000,000,000],[000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,255,255,000,000,000,
000,000,000,255,255,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000,
000,000,000,000,000,000,000,000],[000,000,255,255,255,255,000,000,
000,255,103,103,103,103,255,000,
255,103,013,022,022,034,103,255,
255,103,022,013,034,022,103,255,
255,103,022,034,013,022,103,255,
255,103,034,022,022,013,103,255,
000,255,103,103,103,103,255,000,
000,000,255,255,255255,000,000] };
int i;
for (i = 0; i<63; i++) {
	SetpointColour(i, 0, 0, 0);
	SetpointColour(i, R[j][i], G[j][i], B[j][i]);
	HAL_Delay(time);
}
						}
						void LightControll(uchar keyvalue)
						{
							switch (keyvalue)
							{
							case A1_1:LightEffect1_1(); break;
							case A1_2:theaterChase(Color(0, 0, 255), 80); break;
							case A1_3:theaterChase(Color(0, 0, 255), 80); break;
							case A1_4:LightEffect2(40, 5); break;
							case A1_5:LightEffect2(40, 5); break;
							case A1_6:theaterChase(Color(0, 0, 255), 80); break;
							case A1_7:theaterChase(Color(0, 0, 255), 80); break;
							case A1_8:LightEffect1_8(); break;
							case A2_1:rainbow(40); break;
							case A2_2:LightEffect8(9); break;
							case A2_3:LightEffect8(10); break;
							case A2_4:LightEffect8(11); break;
							case A2_5:LightEffect8(12); break;
							case A2_6:LightEffect8(13); break;
							case A2_7:LightEffect8(14); break;
							case A2_8:theaterChaseRainbow(40); break;
							case A3_1:rainbow(40); break;
							case A3_2:LightEffect8(17); break;
							case A3_3:LightEffect8(18); break;
							case A3_4:LightEffect8(19); break;
							case A3_5:LightEffect8(20); break;
							case A3_6:LightEffect8(21); break;
							case A3_7:LightEffect8(22); break;
							case A3_8:theaterChaseRainbow(40); break;
							case A4_1:LightEffect2(40, 5); break;
							case A4_2:LightEffect8(25); break;
							case A4_3:LightEffect1(20, 0); break;
							case A4_4:LightEffect1(20, 1); break;
							case A4_5:LightEffect1(20, 2); break;
							case A4_6:LightEffect1(20, 3); break;
							case A4_7:LightEffect8(30); break;
							case A4_8:LightEffect2(40, 5); break;
							case A5_1:LightEffect2(40, 5); break;
							case A5_2:LightEffect8(33); break;
							case A5_3:LightEffect1(20, 4); break;
							case A5_4:LightEffect1(20, 5); break;
							case A5_5:LightEffect1(20, 6); break;
							case A5_6:LightEffect1(20, 7); break;
							case A5_7:LightEffect8(38); break;
							case A5_8:LightEffect2(40, 5); break;
							case A6_1:rainbow(40); break;
							case A6_2:LightEffect8(41); break;
							case A6_3:LightEffect8(42); break;
							case A6_4:LightEffect8(43); break;
							case A6_5:LightEffect8(44); break;
							case A6_6:LightEffect8(45); break;
							case A6_7:LightEffect8(46); break;
							case A6_8:theaterChaseRainbow(40); break;
							case A7_1:rainbow(40); break;
							case A7_2:LightEffect8(49); break;
							case A7_3:LightEffect8(50); break;
							case A7_4:LightEffect8(51); break;
							case A7_5:LightEffect8(52); break;
							case A7_6:LightEffect8(53); break;
							case A7_7:LightEffect8(54); break;
							case A7_8:theaterChaseRainbow(40); break;
							case A8_1:LightEffect8_1(); break;
							case A8_2:colorWipe(255, 255); break;
							case A8_3:colorWipe(255, 255); break;
							case A8_4:LightEffect2(40, 5); break;
							case A8_5:LightEffect2(40, 5); break;
							case A8_6:colorWipe(255, 255); break;
							case A8_7:colorWipe(255, 255); break;
							case A8_8:LightEffect8_8(); break;
							default:break;
							}
						}


#include <reg51.h>
#include "intrins.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define numLEDs 64   //�Ƶĸ���
sbit WS2812 = P1^7;

/*�ƹ���ƺ��������ݷ���ɨ���ֵ���Ƶ�Ч����64����*/

unsigned char buf_R[numLEDs] = {0};//��ɫ����

unsigned char buf_G[numLEDs] = {0};

unsigned char buf_B[numLEDs] = {0};

void RGB_Set_Up();  //��0��

void RGB_Set_Down(); //��1��

void HAL_Delay(unsigned int t)

{

		unsigned int x,y;

	  for(x=114;x>0;x--)

	  for(y=t;y>0;y--);

}


//��λ��ʱ

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

//1�룬�ߵ�ƽ850ns �͵�ƽ400ns �������150ns

void RGB_Set_Up()

{

		WS2812 = 1;

	  //�����߼������ǵ��Եĵ���ʱ

		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 

	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 

		WS2812 = 0;

}

 

//1�룬�ߵ�ƽ400ns �͵�ƽ850ns �������150ns

void RGB_Set_Down()
{
		WS2812 = 1;
	  //�����߼������ǵ��Եĵ���ʱ
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();  
		WS2812 = 0;
}

 

//����24λ����

void Send_2811_24bits(unsigned char G8,unsigned char R8,unsigned char B8)

{

	  

	  unsigned int n = 0;

	  //����G8λ

		for(n=0;n<8;n++)

		{

			G8<<=n;

			if(G8&0x80 == 0x80)

			{

				RGB_Set_Up();

			}

			else  

			{

			  RGB_Set_Down();

			}

		}

		//����R8λ

		for(n=0;n<8;n++)

		{

			R8<<=n;

			if(R8&0x80 == 0x80)

			{

				RGB_Set_Up();

			}

			else  

			{

				RGB_Set_Down();

			}

			

		}

		//����B8λ

	  for(n=0;n<8;n++)

		{

			B8<<=n;

			if(B8&0x80 == 0x80)

			{

				RGB_Set_Up();

			}

			else  

			{

			  RGB_Set_Down();

			}

		}

}

//��λ��

void RGB_Rst()

{

		WS2812 = 0;

		Delay50us();

}

//��24λ����GRB��תRGB

void Set_Colour(unsigned char r,unsigned char g,unsigned char b)

{

	  unsigned char i;

		for(i=0;i<numLEDs;i++)

	  {

				buf_R[i] = r; //����

			  buf_G[i] = g;

			  buf_B[i] = b;

		}

		for(i=0;i<numLEDs;i++)

		{

			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//������ʾ

		}

}

//ĳһ������ʾ����ɫ

void SetPointColour(unsigned int num,unsigned char r,unsigned char g,unsigned char b)

{

	  unsigned char i;

		for(i=0;i<numLEDs;i++)

	  {

			  buf_R[num] = r;//����

			  buf_G[num] = g;

			  buf_B[num] = b;

		}

		for(i=0;i<numLEDs;i++)

		{

			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//������ʾ

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

 

  for(j=0; j<256; j++) 

	{

    for(i=0; i<numLEDs; i++)

		{

      SetPixelColor(i, Wheel((i+j) & 255));

    }

		PixelUpdate();

    HAL_Delay(wait);

  }

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

  for (j=0; j<10; j++) 

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

  for (j=0; j < 256; j++) 

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

}


// Fill the dots one after the other with a color
//��һ����ɫ�����ЩԲ��
void colorWipe(unsigned long c, unsigned int wait) 

{

	unsigned int i=0;

  for( i=0; i<numLEDs; i++) 

	{

    SetPixelColor(i, c);

    PixelUpdate();

    HAL_Delay(wait);

  }

}
void zidingyi1()

{

	  

		while(1)

		{

			  rainbow(45);

              rainbowCycle(40);

              theaterChase(Color(0,0,255),80); // Blue

			  theaterChase(Color(0,255,0),80); // Blue

			  theaterChase(Color(255,0,0),80); // Blue

    		  theaterChaseRainbow(40);

		 		colorWipe(255,255);

		}

}
//���¾��� 


//------------------------------------------------------------------------------------------------------------------------------------------------------
void LightEffect8(num)
{
	SetPixelColor(num, Color(244,138,242));
	delay(  );
	SetPixelColor(num, 0);
}
void LightEffect1_1(void)       //���Ͻǵ�һ�������ĵ�Ч
{
	int i,j,k;
	for (k=0;k<8*N ;k=k+8)		//����N����ɫ�ݽ�
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(i,Color(0,0,i+k*64/(N)));	 //��һ��
			SentPixelColor(i*8,Color(0,0,i+k*64/(N)));	 //��һ��
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(i,0);
		SentPixelColor(i*8,0);		
	}		
}
void LightEffect1_8(void)		 //���Ͻǵ�һ�������ĵ�Ч
{
	int i,k;
	for (k=0;k<8*N;k=k+8)
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(7-i,Color(i+k*64/(N),0,0));		 //��һ��
			SentPixelColor(i*8+7,Color(i+k*64/(N),0,0));	 //���һ��
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(7-i,0);
		SentPixelColor(i*8+7,0);		
	}
}
void LightEffect8_1(void)		 //���½ǵ�һ�������ĵ�Ч
{
	int i,k;
	for (k=0;k<8*N;k=k+8)
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(56+i,Color(0,i+k*64/(N),0));		 //��һ��
			SentPixelColor((7-i)*8,Color(0,i+k*64/(N),0));	 //���һ��
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(56+i,0);
		SentPixelColor((7-i)*8,0);		
	}
}
void LightEffect8_8(void)		 //���½ǵ�һ�������ĵ�Ч
{
	int i,k,N=64;
	for (k=0;k<8*N;k=k+8)
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(63-i,Color(i+k*64/(N),0,i+k*64/(N)));		 //��һ��	��ɫ
			SentPixelColor((7-i)*8+7,Color(i+k*64/(N),0,i+k*64/(N)));	 //���һ��	 ��ɫ
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(63-i,0);
		SentPixelColor((7-i)*8+7,0);		
	}
}
//=========================================�������������𶯵�Ч��=============================================================
void LightSunShineWithMusic(void)
{
	//    ����   ���   ��ɫ    ǳ��      ǳ��      ��ɫ       ��ɫ     ��ɫ     ����   
	// R  255   255    255     251      128       116      0        219     14
	// G  255    0     153     255      255       234      0         0      43
	// B  255    0     0        66       0        254      255      219     143
		//���ص���0-6֮���ĳһ����
	int GetNum()
{
	return rand() % 7;
}

	//��Ҫ������һ��0-7��������ĺ�����
	int RandNumber;
	int i, j;
	unsigned int XuHao; //�洢��Ҫд��ĵƵ����
	for (i = 0; i < 8; i++)
	{
		RandNumber = GetNum()+2;
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
void LightEffect2(unsigned char time,unsigned int fre)
{
	uint i;
	double pertime = (double)time/fre;
	for (i = 0; i < fre; i++)
	{
		LightSunShineWithMusic();
		DDelay_ms(pertime);
	}
}
void LightEffect1(unsigned char time,unsigned int j)
		unsigned char R[][]={[000,000,000,000,000,000,000,000,	
     	      000,255,255,000,000,255,255,000,
                      255,255,255,255,255,255,255,255,
	      255,255,255,255,255,255,255,255,									
	      255,255,255,255,255,255,255,255,
                      000,255,255,255,255,255,000,000,
	      000,000,255,255,255,255,000,000,
	      000,000,000,255,255,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,255,000,000,255,000,000,
	      000,255,253,255,255,253,255,000,									
	      000,255,253,253,253,253,255,000,
                      000,000,255,253,253,255,000,000,
	      000,000,000,255,255,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,053,000,000,000,000,
                      000,000,053,060,053,000,000,000,
	      000,053,028,060,060,053,000,000,									
	      000,000,053,028,060,060,053,000,
                      000,000,000,053,028,053,000,000,
	      000,000,000,000,053,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,003,003,000,000,000,	
     	      000,000,001,001,001,001,000,000,
                      000,197,197,197,197,197,197,000,
	      254,254,254,254,254,254,254,254,									
	      000,197,197,197,197,197,197,000,
                      000,000,001,001,001,001,000,000,
	      000,000,000,003,003,000,000,000,
	      000,000,000,000,000,000,000,000],[255,255,255,255,255,255,255,255,	
     	      255,255,255,255,255,255,255,255,
                      255,255,000,000,000,000,255,255,
	      255,255,000,000,000,000,255,255,									
	      255,255,000,000,000,000,255,255,
                      255,255,000,000,000,000,255,255,
	      255,255,255,255,255,255,255,255,
	      255,255,255,255,255,255,255,255],[000,000,000,000,000,000,000,000,
     	      000,000,003,003,003,003,000,000,
                      000,003,242,233,233,221,003,000,
	      000,003,233,242,221,233,003,000,									
	      000,003,233,221,242,233,003,000,
                      000,003,221,233,233,242,003,000,
	      000,000,003,003,003,003,000,000,
	      000,000,000,000,000,000,000,000]};
		unsigned char G[][]={[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,128,000,000,128,000,000,									
	      000,000,128,128,128,128,000,000,
                      000,000,000,128,128,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,128,000,000,128,000,000,
	      000,128,197,128,128,197,128,000,									
	      000,128,197,197,197,197,128,000,
                      000,000,128,197,197,128,000,000,
	      000,000,000,128,128,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,255,000,000,000,000,	
     	      000,000,255,202,255,000,000,000,
                      000,255,202,185,202,255,000,000,
	      255,202,203,185,185,202,255,000,									
	      000,255,202,203,185,185,202,255,
                      000,000,255,202,203,202,255,000,
	      000,000,000,255,202,255,000,000,
	      000,000,000,000,255,000,000,000],[000,000,000,252,252,000,000,000,	
     	      000,000,254,254,254,254,000,000,
                      000,254,254,254,254,254,254,000,
	      166,166,166,166,166,166,166,166,									
	      000,254,254,254,254,254,254,000,
                      000,000,254,254,254,254,000,000,
	      000,000,000,252,252,000,000,000,
	      000,000,000,000,000,000,000,000],[128,128,128,128,128,128,128,128,	
     	      128,255,255,255,255,255,255,128,
                      128,255,255,255,255,255,255,128,
	      128,255,255,255,255,255,255,128,									
	      128,255,255,255,255,255,255,128,
                      128,255,255,255,255,255,255,128,
	      128,255,255,255,255,255,255,128,
	      128,128,128,128,128,128,128,128],[000,000,032,032,032,032,000,000,	
     	      000,032,252,252,252,252,032,000,
                      032,252,128,233,233,038,252,032,
	      032,252,233,128,038,233,252,032,									
	      032,252,233,038,128,233,252,032,
                      032,252,038,233,233,128,252,032,
	      000,032,252,252,252,252,032,000,
	      000,000,032,032,032,032,000,000]};
		unsigned char B[][]={[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,128,000,000,128,000,000,
	      000,128,192,128,128,192,128,000,									
	      000,128,192,192,192,192,128,000,
                      000,000,128,192,192,128,000,000,
	      000,000,000,128,128,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,066,000,000,066,000,000,									
	      000,000,066,066,066,066,000,000,
                      000,000,000,066,066,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,098,000,000,000,000,
                      000,000,098,196,098,000,000,000,
	      000,098,227,196,196,098,000,000,									
	      000,000,098,227,196,196,098,000,
                      000,000,000,098,227,098,000,000,
	      000,000,000,000,098,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,152,152,000,000,000,	
     	      000,000,033,033,033,033,000,000,
                      000,001,001,001,001,001,001,000,
	      001,001,001,001,001,001,001,001,
	      001,001,001,001,001,001,001,001,									
	      000,001,001,001,001,001,001,000,
                      000,000,033,033,033,033,000,000,
	      000,000,000,152,152,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,255,255,000,000,000,									
	      000,000,000,255,255,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,255,255,255,255,000,000,	
     	      000,255,103,103,103,103,255,000,
                      255,103,013,022,022,034,103,255,
	      255,103,022,013,034,022,103,255,									
	      255,103,022,034,013,022,103,255,
                      255,103,034,022,022,013,103,255,
	      000,255,103,103,103,103,255,000,
	      000,000,255,255,255255,000,000]};
		int i;
				for(i=0;i<63;i++){
					SetpointColour(i,0,0,0);
					SetpointColour(i,R[j][i],G[j][i],B[j][i]);
					HAL_Delay(time);
							     }
						}
void LightControll(uchar keyvalue)
{
 	switch(keyvalue)
 	{
	  case A1_1:LightEffect1_1();break;
	  case A1_2:theaterChase(Color(0,0,255),80);break;
	  case A1_3:theaterChase(Color(0,0,255),80);break;
	  case A1_4:LightEffect2(40,5);break;
	  case A1_5:LightEffect2(40,5);break;
	  case A1_6:theaterChase(Color(0,0,255),80);break;
	  case A1_7:theaterChase(Color(0,0,255),80);break;
	  case A1_8:LightEffect1_8();break;
	  case A2_1:rainbow(40);break;
	  case A2_2:LightEffect8(9);break;
	  case A2_3:LightEffect8(10);break;
	  case A2_4:LightEffect8(11);break;
	  case A2_5:LightEffect8(12);break;
	  case A2_6:LightEffect8(13);break;
	  case A2_7:LightEffect8(14);break;
	  case A2_8:theaterChaseRainbow(40);break;
	  case A3_1:rainbow(40);break;
	  case A3_2:LightEffect8(17);break;
	  case A3_3:LightEffect8(18);break;
	  case A3_4:LightEffect8(19);break;
	  case A3_5:LightEffect8(20);break;
	  case A3_6:LightEffect8(21);break;
	  case A3_7:LightEffect8(22);break;
	  case A3_8:theaterChaseRainbow(40);break;
	  case A4_1:LightEffect2(40,5);break;
	  case A4_2:LightEffect8(25);break;
	  case A4_3:LightEffect1(20,0);break;
	  case A4_4:LightEffect1(20,1);break;
	  case A4_5:LightEffect1(20,2);break;
	  case A4_6:LightEffect1(20,3);break;
	  case A4_7:LightEffect8(30);break;
	  case A4_8:LightEffect2(40,5);break;
	  case A5_1:LightEffect2(40,5);break;
	  case A5_2:LightEffect8(33);break;
	  case A5_3:LightEffect1(20,4);break;
	  case A5_4:LightEffect1(20,5);break;
	  case A5_5:LightEffect1(20,6);break;
	  case A5_6:LightEffect1(20,7);break;
	  case A5_7:LightEffect8(38);break;
	  case A5_8:LightEffect2(40,5);break;
	  case A6_1:rainbow(40);break;
	  case A6_2:LightEffect8(41);break;
	  case A6_3:LightEffect8(42);break;
	  case A6_4:LightEffect8(43);break;
	  case A6_5:LightEffect8(44);break;
	  case A6_6:LightEffect8(45);break;
	  case A6_7:LightEffect8(46);break;
	  case A6_8:theaterChaseRainbow(40);break;
	  case A7_1:rainbow(40);break;
	  case A7_2:LightEffect8(49);break;
	  case A7_3:LightEffect8(50);break;
	  case A7_4:LightEffect8(51);break;
	  case A7_5:LightEffect8(52);break;
	  case A7_6:LightEffect8(53);break;
	  case A7_7:LightEffect8(54);break;
	  case A7_8:theaterChaseRainbow(40);break;
	  case A8_1:LightEffect8_1();break;
	  case A8_2:colorWipe(255,255);break;
	  case A8_3:colorWipe(255,255);break;
	  case A8_4:LightEffect2(40,5);break;
	  case A8_5:LightEffect2(40,5);break;
	  case A8_6:colorWipe(255,255);break;
	  case A8_7:colorWipe(255,255);break;
	  case A8_8:LightEffect8_8();break;
  	  default:break;
 	}
}   
	

#include <reg51.h>
#include "intrins.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define numLEDs 64   //�Ƶĸ���
sbit WS2812 = P1^7;

/*�ƹ���ƺ��������ݷ���ɨ���ֵ���Ƶ�Ч����64����*/

unsigned char buf_R[numLEDs] = {0};//��ɫ����

unsigned char buf_G[numLEDs] = {0};

unsigned char buf_B[numLEDs] = {0};

void RGB_Set_Up();  //��0��

void RGB_Set_Down(); //��1��

void HAL_Delay(unsigned int t)

{

		unsigned int x,y;

	  for(x=114;x>0;x--)

	  for(y=t;y>0;y--);

}


//��λ��ʱ

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

//1�룬�ߵ�ƽ850ns �͵�ƽ400ns �������150ns

void RGB_Set_Up()

{

		WS2812 = 1;

	  //�����߼������ǵ��Եĵ���ʱ

		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 

	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 

		WS2812 = 0;

}

 

//1�룬�ߵ�ƽ400ns �͵�ƽ850ns �������150ns

void RGB_Set_Down()
{
		WS2812 = 1;
	  //�����߼������ǵ��Եĵ���ʱ
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();  
		WS2812 = 0;
}

 

//����24λ����

void Send_2811_24bits(unsigned char G8,unsigned char R8,unsigned char B8)

{

	  

	  unsigned int n = 0;

	  //����G8λ

		for(n=0;n<8;n++)

		{

			G8<<=n;

			if(G8&0x80 == 0x80)

			{

				RGB_Set_Up();

			}

			else  

			{

			  RGB_Set_Down();

			}

		}

		//����R8λ

		for(n=0;n<8;n++)

		{

			R8<<=n;

			if(R8&0x80 == 0x80)

			{

				RGB_Set_Up();

			}

			else  

			{

				RGB_Set_Down();

			}

			

		}

		//����B8λ

	  for(n=0;n<8;n++)

		{

			B8<<=n;

			if(B8&0x80 == 0x80)

			{

				RGB_Set_Up();

			}

			else  

			{

			  RGB_Set_Down();

			}

		}

}

//��λ��

void RGB_Rst()

{

		WS2812 = 0;

		Delay50us();

}

//��24λ����GRB��תRGB

void Set_Colour(unsigned char r,unsigned char g,unsigned char b)

{

	  unsigned char i;

		for(i=0;i<numLEDs;i++)

	  {

				buf_R[i] = r; //����

			  buf_G[i] = g;

			  buf_B[i] = b;

		}

		for(i=0;i<numLEDs;i++)

		{

			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//������ʾ

		}

}

//ĳһ������ʾ����ɫ

void SetPointColour(unsigned int num,unsigned char r,unsigned char g,unsigned char b)

{

	  unsigned char i;

		for(i=0;i<numLEDs;i++)

	  {

			  buf_R[num] = r;//����

			  buf_G[num] = g;

			  buf_B[num] = b;

		}

		for(i=0;i<numLEDs;i++)

		{

			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//������ʾ

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

 

  for(j=0; j<256; j++) 

	{

    for(i=0; i<numLEDs; i++)

		{

      SetPixelColor(i, Wheel((i+j) & 255));

    }

		PixelUpdate();

    HAL_Delay(wait);

  }

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

  for (j=0; j<10; j++) 

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

  for (j=0; j < 256; j++) 

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

}


// Fill the dots one after the other with a color
//��һ����ɫ�����ЩԲ��
void colorWipe(unsigned long c, unsigned int wait) 

{

	unsigned int i=0;

  for( i=0; i<numLEDs; i++) 

	{

    SetPixelColor(i, c);

    PixelUpdate();

    HAL_Delay(wait);

  }

}
void zidingyi1()

{

	  

		while(1)

		{

			  rainbow(45);

              rainbowCycle(40);

              theaterChase(Color(0,0,255),80); // Blue

			  theaterChase(Color(0,255,0),80); // Blue

			  theaterChase(Color(255,0,0),80); // Blue

    		  theaterChaseRainbow(40);

		 		colorWipe(255,255);

		}

}
//���¾��� 


//------------------------------------------------------------------------------------------------------------------------------------------------------
void LightEffect8(num)
{
	SetPixelColor(num, Color(244,138,242));
	delay(  );
	SetPixelColor(num, 0);
}
void LightEffect1_1(void)       //���Ͻǵ�һ�������ĵ�Ч
{
	int i,j,k;
	for (k=0;k<8*N ;k=k+8)		//����N����ɫ�ݽ�
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(i,Color(0,0,i+k*64/(N)));	 //��һ��
			SentPixelColor(i*8,Color(0,0,i+k*64/(N)));	 //��һ��
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(i,0);
		SentPixelColor(i*8,0);		
	}		
}
void LightEffect1_8(void)		 //���Ͻǵ�һ�������ĵ�Ч
{
	int i,k;
	for (k=0;k<8*N;k=k+8)
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(7-i,Color(i+k*64/(N),0,0));		 //��һ��
			SentPixelColor(i*8+7,Color(i+k*64/(N),0,0));	 //���һ��
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(7-i,0);
		SentPixelColor(i*8+7,0);		
	}
}
void LightEffect8_1(void)		 //���½ǵ�һ�������ĵ�Ч
{
	int i,k;
	for (k=0;k<8*N;k=k+8)
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(56+i,Color(0,i+k*64/(N),0));		 //��һ��
			SentPixelColor((7-i)*8,Color(0,i+k*64/(N),0));	 //���һ��
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(56+i,0);
		SentPixelColor((7-i)*8,0);		
	}
}
void LightEffect8_8(void)		 //���½ǵ�һ�������ĵ�Ч
{
	int i,k,N=64;
	for (k=0;k<8*N;k=k+8)
	{
		for (i=0;i<8;i++)
		{
			SentPixelColor(63-i,Color(i+k*64/(N),0,i+k*64/(N)));		 //��һ��	��ɫ
			SentPixelColor((7-i)*8+7,Color(i+k*64/(N),0,i+k*64/(N)));	 //���һ��	 ��ɫ
		}
	}
	for (i=0;i<8;i++)	          //clear all
	{
		SentPixelColor(63-i,0);
		SentPixelColor((7-i)*8+7,0);		
	}
}
//=========================================�������������𶯵�Ч��=============================================================
void LightSunShineWithMusic(void)
{
	//    ����   ���   ��ɫ    ǳ��      ǳ��      ��ɫ       ��ɫ     ��ɫ     ����   
	// R  255   255    255     251      128       116      0        219     14
	// G  255    0     153     255      255       234      0         0      43
	// B  255    0     0        66       0        254      255      219     143
		//���ص���0-6֮���ĳһ����
	int GetNum()
{
	return rand() % 7;
}

	//��Ҫ������һ��0-7��������ĺ�����
	int RandNumber;
	int i, j;
	unsigned int XuHao; //�洢��Ҫд��ĵƵ����
	for (i = 0; i < 8; i++)
	{
		RandNumber = GetNum()+2;
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
void LightEffect2(unsigned char time,unsigned int fre)
{
	uint i;
	double pertime = (double)time/fre;
	for (i = 0; i < fre; i++)
	{
		LightSunShineWithMusic();
		DDelay_ms(pertime);
	}
}
void LightEffect1(unsigned char time,unsigned int j)
		unsigned char R[][]={[000,000,000,000,000,000,000,000,	
     	      000,255,255,000,000,255,255,000,
                      255,255,255,255,255,255,255,255,
	      255,255,255,255,255,255,255,255,									
	      255,255,255,255,255,255,255,255,
                      000,255,255,255,255,255,000,000,
	      000,000,255,255,255,255,000,000,
	      000,000,000,255,255,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,255,000,000,255,000,000,
	      000,255,253,255,255,253,255,000,									
	      000,255,253,253,253,253,255,000,
                      000,000,255,253,253,255,000,000,
	      000,000,000,255,255,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,053,000,000,000,000,
                      000,000,053,060,053,000,000,000,
	      000,053, 28,060,060,053,000,000,									
	      000,000,053, 28,060,060,053,000,
                      000,000,000,053, 28,053,000,000,
	      000,000,000,000,053,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,003,003,000,000,000,	
     	      000,000,001,001,001,001,000,000,
                      000,197,197,197,197,197,197,000,
	      254,254,254,254,254,254,254,254,									
	      000,197,197,197,197,197,197,000,
                      000,000,001,001,001,001,000,000,
	      000,000,000,003,003,000,000,000,
	      000,000,000,000,000,000,000,000],[255,255,255,255,255,255,255,255,	
     	      255,255,255,255,255,255,255,255,
                      255,255,000,000,000,000,255,255,
	      255,255,000,000,000,000,255,255,									
	      255,255,000,000,000,000,255,255,
                      255,255,000,000,000,000,255,255,
	      255,255,255,255,255,255,255,255,
	      255,255,255,255,255,255,255,255],[000,000,000,000,000,000,000,000,
     	      000,000,003,003,003,003,000,000,
                      000,003,242,233,233,221,003,000,
	      000,003,233,242,221,233,003,000,									
	      000,003,233,221,242,233,003,000,
                      000,003,221,233,233,242,003,000,
	      000,000,003,003,003,003,000,000,
	      000,000,000,000,000,000,000,000]};
		unsigned char G[][]={[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,128,000,000,128,000,000,									
	      000,000,128,128,128,128,000,000,
                      000,000,000,128,128,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,128,000,000,128,000,000,
	      000,128,197,128,128,197,128,000,									
	      000,128,197,197,197,197,128,000,
                      000,000,128,197,197,128,000,000,
	      000,000,000,128,128,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,255,000,000,000,000,	
     	      000,000,255,202,255,000,000,000,
                      000,255,202,185,202,255,000,000,
	      255,202,203,185,185,202,255,000,									
	      000,255,202,203,185,185,202,255,
                      000,000,255,202,203,202,255,000,
	      000,000,000,255,202,255,000,000,
	      000,000,000,000,255,000,000,000],[000,000,000,252,252,000,000,000,	
     	      000,000,254,254,254,254,000,000,
                      000,254,254,254,254,254,254,000,
	      166,166,166,166,166,166,166,166,									
	      000,254,254,254,254,254,254,000,
                      000,000,254,254,254,254,000,000,
	      000,000,000,252,252,000,000,000,
	      000,000,000,000,000,000,000,000],[128,128,128,128,128,128,128,128,	
     	      128,255,255,255,255,255,255,128,
                      128,255,255,255,255,255,255,128,
	      128,255,255,255,255,255,255,128,									
	      128,255,255,255,255,255,255,128,
                      128,255,255,255,255,255,255,128,
	      128,255,255,255,255,255,255,128,
	      128,128,128,128,128,128,128,128],[000,000,032,032,032,032,000,000,	
     	      000,032,252,252,252,252,032,000,
                      032,252,128,233,233, 38,252,032,
	      032,252,233,128, 38,233,252,032,									
	      032,252,233, 38,128,233,252,032,
                      032,252, 38,233,233,128,252,032,
	      000,032,252,252,252,252,032,000,
	      000,000,032,032,032,032,000,000]};
		unsigned char B[][]={[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,128,000,000,128,000,000,
	      000,128,192,128,128,192,128,000,									
	      000,128,192,192,192,192,128,000,
                      000,000,128,192,192,128,000,000,
	      000,000,000,128,128,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,066,000,000,066,000,000,									
	      000,000,066,066,066,066,000,000,
                      000,000,000,066,066,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000, 98,000,000,000,000,
                      000,000, 98,196, 98,000,000,000,
	      000, 98,227,196,196, 98,000,000,									
	      000,000, 98,227,196,196, 98,000,
                      000,000,000, 98,227, 98,000,000,
	      000,000,000,000, 98,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,000,152,152,000,000,000,	
     	      000,000,033,033,033,033,000,000,
                      000,001,001,001,001,001,001,000,
	      001,001,001,001,001,001,001,001,
	      001,001,001,001,001,001,001,001,									
	      000,001,001,001,001,001,001,000,
                      000,000,033,033,033,033,000,000,
	      000,000,000,152,152,000,000,000],[000,000,000,000,000,000,000,000,	
     	      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,255,255,000,000,000,									
	      000,000,000,255,255,000,000,000,
                      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000,
	      000,000,000,000,000,000,000,000],[000,000,255,255,255,255,000,000,	
     	      000,255,103,103,103,103,255,000,
                      255,103,013,022,022,034,103,255,
	      255,103,022,013,034,022,103,255,									
	      255,103,022,034,013,022,103,255,
                      255,103,034,022,022,013,103,255,
	      000,255,103,103,103,103,255,000,
	      000,000,255,255,255255,000,000]};
		int i;
				for(i=0;i<63;i++){
					SetpointColour(i,0,0,0);
					SetpointColour(i,R[j][i],G[j][i],B[j][i]);
					HAL_Delay(time);
							     }
						}
void LightControll(uchar keyvalue)
{
 	switch(keyvalue)
 	{
	  case A1_1:LightEffect1_1();break;
	  case A1_2:theaterChase(Color(0,0,255),80);break;
	  case A1_3:theaterChase(Color(0,0,255),80);break;
	  case A1_4:LightEffect2(40,5);break;
	  case A1_5:LightEffect2(40,5);break;
	  case A1_6:theaterChase(Color(0,0,255),80);break;
	  case A1_7:theaterChase(Color(0,0,255),80);break;
	  case A1_8:LightEffect1_8();break;
	  case A2_1:rainbow(40);break;
	  case A2_2:LightEffect8(9);break;
	  case A2_3:LightEffect8(10);break;
	  case A2_4:LightEffect8(11);break;
	  case A2_5:LightEffect8(12);break;
	  case A2_6:LightEffect8(13);break;
	  case A2_7:LightEffect8(14);break;
	  case A2_8:theaterChaseRainbow(40);break;
	  case A3_1:rainbow(40);break;
	  case A3_2:LightEffect8(17);break;
	  case A3_3:LightEffect8(18);break;
	  case A3_4:LightEffect8(19);break;
	  case A3_5:LightEffect8(20);break;
	  case A3_6:LightEffect8(21);break;
	  case A3_7:LightEffect8(22);break;
	  case A3_8:theaterChaseRainbow(40);break;
	  case A4_1:LightEffect2(40,5);break;
	  case A4_2:LightEffect8(25);break;
	  case A4_3:LightEffect1(20,0);break;
	  case A4_4:LightEffect1(20,1);break;
	  case A4_5:LightEffect1(20,2);break;
	  case A4_6:LightEffect1(20,3);break;
	  case A4_7:LightEffect8(30);break;
	  case A4_8:LightEffect2(40,5);break;
	  case A5_1:LightEffect2(40,5);break;
	  case A5_2:LightEffect8(33);break;
	  case A5_3:LightEffect1(20,4);break;
	  case A5_4:LightEffect1(20,5);break;
	  case A5_5:LightEffect1(20,6);break;
	  case A5_6:LightEffect1(20,7);break;
	  case A5_7:LightEffect8(38);break;
	  case A5_8:LightEffect2(40,5);break;
	  case A6_1:rainbow(40);break;
	  case A6_2:LightEffect8(41);break;
	  case A6_3:LightEffect8(42);break;
	  case A6_4:LightEffect8(43);break;
	  case A6_5:LightEffect8(44);break;
	  case A6_6:LightEffect8(45);break;
	  case A6_7:LightEffect8(46);break;
	  case A6_8:theaterChaseRainbow(40);break;
	  case A7_1:rainbow(40);break;
	  case A7_2:LightEffect8(49);break;
	  case A7_3:LightEffect8(50);break;
	  case A7_4:LightEffect8(51);break;
	  case A7_5:LightEffect8(52);break;
	  case A7_6:LightEffect8(53);break;
	  case A7_7:LightEffect8(54);break;
	  case A7_8:theaterChaseRainbow(40);break;
	  case A8_1:LightEffect8_1();break;
	  case A8_2:colorWipe(255,255);break;
	  case A8_3:colorWipe(255,255);break;
	  case A8_4:LightEffect2(40,5);break;
	  case A8_5:LightEffect2(40,5);break;
	  case A8_6:colorWipe(255,255);break;
	  case A8_7:colorWipe(255,255);break;
	  case A8_8:LightEffect8_8();break;
  	  default:break;
 	}
}   
	

