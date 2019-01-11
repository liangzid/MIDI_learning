#include <STC15F2K60S2.H>
#include "intrins.h"
 
sbit WS2812 = P1^7;
 
#define numLEDs 8   //灯的个数
unsigned char buf_R[numLEDs] = {0};//颜色缓存
unsigned char buf_G[numLEDs] = {0};
unsigned char buf_B[numLEDs] = {0};
 
void RGB_Set_Up();  //送0码
void RGB_Set_Down(); //送1码
 
 
 
void HAL_Delay(unsigned int t)
{
		unsigned int x,y;
	  for(x=114;x>0;x--)
	  for(y=t;y>0;y--);
}
 
 
	 
//复位延时
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
 
 
//1码，高电平850ns 低电平400ns 误差正负150ns
void RGB_Set_Up()
{
		WS2812 = 1;
	  //经过逻辑分析仪调试的的延时
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	  _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 
		WS2812 = 0;
}
 
//1码，高电平400ns 低电平850ns 误差正负150ns
void RGB_Set_Down()
{
		WS2812 = 1;
	  //经过逻辑分析仪调试的的延时
		_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();  
		WS2812 = 0;
}
 
//发送24位数据
void Send_2811_24bits(unsigned char G8,unsigned char R8,unsigned char B8)
{
	  
	  unsigned int n = 0;
	  //发送G8位
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
		//发送R8位
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
		//发送B8位
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
//复位码
void RGB_Rst()
{
		WS2812 = 0;
		Delay50us();
}
//把24位数据GRB码转RGB
void Set_Colour(unsigned char r,unsigned char g,unsigned char b)
{
	  unsigned char i;
		for(i=0;i<numLEDs;i++)
	  {
				buf_R[i] = r; //缓冲
			  buf_G[i] = g;
			  buf_B[i] = b;
		}
		for(i=0;i<numLEDs;i++)
		{
			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//发送显示
		}
}
//某一个点显示的颜色
void SetPointColour(unsigned int num,unsigned char r,unsigned char g,unsigned char b)
{
	  unsigned char i;
		for(i=0;i<numLEDs;i++)  //为什么要加循环
	  {
				buf_R[num] = r;//缓冲
			  buf_G[num] = g;
			  buf_B[num] = b;
		}
		for(i=0;i<numLEDs;i++)
		{
			Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//发送显示
		}
}
 
//颜色交换24位不拆分发
void SetPixelColor(unsigned char num,unsigned long c)
{
	  unsigned char i;
		for(i=0;i<numLEDs;i++)
	  {
				buf_R[num] = (unsigned char)(c>>16);   //？？？
			  buf_G[num] = (unsigned char)(c>>8);
			  buf_B[num] = (unsigned char)(c);
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
 
//颜色算法  //------------------------------------------------------------------------------？
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
 
//稍微不同的是，这使得彩虹均匀分布
void rainbowCycle(unsigned int wait) 
{
  unsigned int i, j;
 
  for(j=0;j<256*5;j++) 
	{ // 5 cycles of all colors on wheel  车轮上所有颜色的5个循环
    for(i=0;i<numLEDs;i++) 
	 {
		SetPixelColor(i, Wheel(((i * 256 / numLEDs) + j) & 255));  //----------------？
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
  for (j=0; j<10; j++) 
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
  for (j=0; j < 256; j++) 
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
}
 
// Fill the dots one after the other with a color
//用一种颜色填充这些圆点
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
 
void main()
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
