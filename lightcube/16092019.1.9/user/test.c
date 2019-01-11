#include <intrins.h>	 //
#include"STC12C5A60S2.H" //芯片头文件
#include"STC12C5A60S2.H" //芯片头文件
#define channel      0x01		  //设置AD通道为 P1.0
#define ADC_POWER   (1<<7) 
#define ADC_SPEEDHH (0x03<<5) 
#define ADC_START   (1<<3) 
#define PADCH       (1<<5) 
#define ADC_FLAG    (1<<4)
typedef unsigned char uchar;
typedef unsigned int  uint;
extern void clear(char le);
extern void key_scan();
extern void flash_1();//rain
extern void flash_2();//rain
extern void flash_3();//rain
extern void flash_4();//rain

extern void flash_5();//rain
extern void flash_6();//rain
extern void flash_7();//rain
extern void flash_8();//rain
extern void flash_9();//rain
extern void flash_10();//rain
extern void flash_11();//rain
extern void flash_12();//rain

extern void hang1();//rain
extern void hang2();//rain
extern void hang3();//rain
extern void hang4();//rain
extern void hang5();//rain
extern void hang6();//rain
extern void hang7();//rain
extern void hang8();//rain
extern void FFT(); //基2fft
extern void flash_diy(void);

extern code  char LIST_TAB[128] ;
extern int   xdata Fft_Real[128]; 				//FFT的实部
extern int   xdata Fft_Image[128];             //FFT的虚部 
extern uchar xdata LED_TAB[16];				//记录红色柱状
extern uchar key_flag ;
extern uchar xdata LED_TAB[16];				//记录红色柱状  
uchar flag_fft_s;
uchar ADC_Count=0,G=0,T;
uchar code tablew[]={0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE}; 
#define     NEW     1
#if         NEW
sbit x1 = P1^2;         //定义P1.2作为第一层的层选信号
sbit x2 = P1^3;         //定义P1.3作为第二层的层选信号 
sbit x3 = P1^4;         //定义P1.4作为第三层的层选信号
sbit x4 = P3^3;         //定义P3.3作为第四第的层选信号 
sbit x5 = P3^4;         //定义P3.4作为第五层的层选信号
sbit x6 = P3^5;         //定义P3.5作为第六第的层选信号 
sbit x7 = P3^7;         //定义P3.7作为第七层的层选信号
sbit x8 = P3^6;         //定义P3.6作为第八层的层选信号
#else
sbit x1 = P1^4;         //定义P1.4作为第一层的层选信号
sbit x2 = P1^2;         //定义P1.2作为第二层的层选信号 
sbit x3 = P3^2;         //定义P3.2作为第三层的层选信号
sbit x4 = P3^3;         //定义P3.3作为第四第二层选信号 
sbit x5 = P3^4;         //定义P3.4作为第五层的层选信号
sbit x6 = P3^5;         //定义P3.5作为第六第二层选信号 
sbit x7 = P3^6;         //定义P3.6作为第七层的层选信号
sbit x8 = P3^7;         //定义P3.7作为第八层的层选信号
#endif

uchar xdata display[8][8];
uchar xdata display2[8][8];
uchar xdata uart_buffer[65];
uchar rec = 0;
uint  uart_cnt = 0;
void layer_cs(unsigned char cs )
{
    unsigned char i = 0;
    x1=0; x2=0;x3=0;x4=0;x5=0;x6=0;x7=0;x8=0;
    if((cs & 0x01) == 0x01)
    {
        x1 = 1;
    }
    if((cs & 0x02) == 0x02)
    {
        x2 = 1;
    }    
    if((cs & 0x04) == 0x04)
    {
        x3 = 1;
    }    
    if((cs & 0x08) == 0x08)
    {
        x4 = 1;
    }    
    if((cs & 0x10) == 0x10)
    {
        x5 = 1;
    }    
    if((cs & 0x20) == 0x20)
    {
        x6 = 1;
    }    
    if((cs & 0x40) == 0x40)
    {
        x7 = 1;
    }    
    if((cs & 0x80) == 0x80)
    {
        x8 = 1;
    }
}
//功能初始化
void Init()
{  
	P0 = 0x00; //74HC573数据口
	P2 = 0x00; //74HC573位选端
    x1=1;
    x2=1;
    x3=1;
    x4=1;
    x5=1;
    x6=1;
    x7=1;
}

void uart_init(void)
{
	PCON &= 0x7F;		//??????
	SCON = 0x50;		//8???,?????
	AUXR |= 0x04;		//???????????Fosc,?1T
	BRT = 0xFD;		    //?????????????
	AUXR |= 0x01;		//??1?????????????????
	AUXR |= 0x10;		//??????????
    REN = 1;            //允许串口接收
    ES = 1;             //串口中断
    EA = 1;    
}
/******** 主函数 ***************************************************************************************/

/***************************************
函数名称：中断函数
输入参数：无
返 回 值：无
****************************************/
void interrupt_uart (void) interrupt 4
{
    uint i = 0;
    uint j = 0;
    if(RI)
    {
        uart_buffer[uart_cnt] = SBUF;
        RI = 0;
        if(uart_buffer[0] == 0xF2)
        {
            uart_cnt++;
            if((uart_cnt == 65)|| uart_cnt>65)
            {          
                uart_cnt = 0;
                uart_buffer[0] = 0;
                rec = 1;
            }
        }
        else if (uart_buffer[0] == 0xF4)
        {
            uart_cnt++;
            if((uart_cnt == 2)|| uart_cnt>2)
            {  
                CCAP0H = uart_buffer[1];
                CCAP1H = uart_buffer[1];
                uart_cnt = 0;
                uart_buffer[0] = 0;
            }        
        }
        
    }
    else
    {
       TI = 0; 
    }
}

void main()
{
	uchar i = 0; //for循环变量
    uchar j = 0; //for循环变量
	Init();
	while(1)
	{					
		clear(0);	//清空刷新显示缓冲区 
		key_scan();	//扫描按键
		
		
		if(key_flag == 1)
		{
			key_flag = 0;
            IE=0x82;
            TCON=0x01;
            TH0=0xc0;    
            TL0=0;
            TR0=1;
            PS  = 1;
            PT0 = 0;
            uart_init();
            flag_fft_s = 1;  //定时器0功能控制
            while(1)
            {
                if(rec == 0)
                {
                    continue;
                }
                rec = 0;
    
                for(i=0;i<8;i++)
                {                
                    for(j=0;j<8;j++)
                    {
                        display2[i][j] = 0;
                    }                               
                } 
                /*PC数据转换*/
                for(j=0;j<8;j++)
                {                
                    for(i=0;i<8;i++)
                    {
                       display2[j][0] |= ((uart_buffer[0+i+1]>>j)  & 0x01)<<(7-i);
                       display2[j][1] |= ((uart_buffer[8+i+1]>>j)  & 0x01)<<(7-i);
                       display2[j][2] |= ((uart_buffer[16+i+1]>>j) & 0x01)<<(7-i);
                       display2[j][3] |= ((uart_buffer[24+i+1]>>j) & 0x01)<<(7-i);
                       display2[j][4] |= ((uart_buffer[32+i+1]>>j) & 0x01)<<(7-i);
                       display2[j][5] |= ((uart_buffer[40+i+1]>>j) & 0x01)<<(7-i);
                       display2[j][6] |= ((uart_buffer[48+i+1]>>j) & 0x01)<<(7-i);
                       display2[j][7] |= ((uart_buffer[56+i+1]>>j) & 0x01)<<(7-i);                      
                    }                               
                }
                
                TR0 = 0;
                /*清空缓冲区*/
                for(i=0;i<8;i++)
                {                
                    for(j=0;j<8;j++)
                    {
                        display[7-i][j] = display2[i][j];
                    }                               
                }                     
                TR0 = 1;           
           }

		}

		//FFT
		if(key_flag == 2)
		{
            //IO口模式初始化
            P3M0 = 0xff; //ULN2803输出口设置为推挽输出
            P3M1 = 0x00;
            P0M0 = 0xff; //P0、P2用于控制74HC573,推挽输出
            P0M1 = 0x00;
            P2M0 = 0xff;
            P2M1 = 0x00;           
            //----- ADC初始化 ------------------------------------------------------------------------------
        	P1ASF  = 0x02;          //0000,0010, 将 P1.1 置成模拟口
        	AUXR1 &=0xFB;           //1111,1011, 令 ADRJ=0
        	EADC   =1;				//AD中断打开
        	ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | channel; //1110 1001   1打开 A/D （ADC_POWER）转换电源；11速度为90周期一次；
        	    		                                               //0中断标志清零；1启动adc(ADC_START);001AD通道打开（这里为P1.1）;        
        	ET0 = 1;			  //定时器0中断 启用
        	TR0 = 0;			  //定时器0     关闭
        	ET1 = 1;			  //定时器1中断 启用
        	TR1 = 0;              //定时器1     关闭
        	PT1 = 0;			  //定时器1 低优先级
        	PT0 = 1;	 		  //定时器0 高优先级
        	IPH = PADCH;
        	IP  = PADC;			  //中断优先级
        	EA  = 1;			  //总中断打开 
        	clear(0);			  //清空显示缓冲区
			TMOD = 0x12; //T0: 8位自动重装      T1: 16位模式
			TH0  = 0x60; //ADC采样周期（80us或6.66us）  采样率 12.5 KHz                              				  
			TL0  = 0x60; //
			TH1  = 0xfd; //刷新显示周期（300us或25us） 	刷新率 										   
			TL1  = 0Xa8; //
			while(1)
			{
				flag_fft_s = 0;  
				TR1 = 1; //刷新显示
				TR0 = 1; //控制采样速率
				//P0    = 0xaa; //P0 未知作用1010 1010
				ADC_Count=0;				
				TR1  = 1;     //开启定时器
				TR0  = 1;     //开启定时器中断0
				EADC = 1;	  //开启ADC 
				while(ADC_Count<128); //转换128次
				for(i=0;i<128;i++)	  //FFT参数虚部赋值
				{
					Fft_Image[i]=0;	  //原始数据 虚部赋值为0
				}
				FFT();                //FFT运算并转换为各频段幅值	

				//其他按钮，跳出
				//key_scan();
				//if(key_flag != 2) break;
			}																																										
		}

		//顺序循环
		if(key_flag == 3)
		{       
            TMOD=0x11;
            EA  = 0;			  //总卸向开
            flag_fft_s = 1;  //定时器0功能控制
            PT0 = 1;	 		  //定时器0 高优先级
            ET0 = 1;			  //定时器0中断 启用
            ES = 0;             //串口中断
            TR0 = 1;	  //开舳ū器0 
            EA  = 1;			  //总卸向开
            clear(0);
			while(1)
			{
                
                flash_diy(); 
                clear(0);               
                
				flash_2();
				clear(0);
                flash_3();
                flash_4();
                //flash_5();
                flash_5();
                flash_5();
                flash_6();
                flash_7();
                flash_8();
                flash_9();
                flash_10();
                clear (0);
                
                flash_11();
                flash_9();
                flash_5();
                flash_7();
                flash_5();
                flash_6();
                flash_8();
                flash_9();
                flash_10();				
			}				
		}
	} 		
}


//定时器1中断 用于刷新显示(只在频谱显示时使用)
void interrupt_fft_display() interrupt 3					 //中断一次显示一列
{	 	 
	TH1  = 0xfd; //刷新显示周期（300us或25us） 	刷新率 										   
	TL1  = 0Xa8; //
	G++;						  	
	if(G>=17) G=1;
	switch(G)								     //往点阵屏填充一列的数据
	{		
		case 1: layer_cs(tablew[(LED_TAB[1] )%8]);hang1();break;	 //(LED_TAB[1] )%8取八的余数
		case 2: layer_cs(tablew[(LED_TAB[2] )%8]);hang1();break;
		case 3: layer_cs(tablew[(LED_TAB[3] )%8]);hang2();break;
		case 4: layer_cs(tablew[(LED_TAB[4] )%8]);hang2();break;
		case 5: layer_cs(tablew[(LED_TAB[5] )%8]);hang3();break;
		case 6: layer_cs(tablew[(LED_TAB[6] )%8]);hang3();break;
		case 7: layer_cs(tablew[(LED_TAB[7] )%8]);hang4();break;
		case 8: layer_cs(tablew[(LED_TAB[8] )%8]);hang4();break;
		case 9: layer_cs(tablew[(LED_TAB[9] )%8]);hang5();break;
		case 10:layer_cs(tablew[(LED_TAB[10])%8]);hang5();break;
		case 11:layer_cs(tablew[(LED_TAB[11])%8]);hang6();break;
		case 12:layer_cs(tablew[(LED_TAB[12])%8]);hang6();break;
		case 13:layer_cs(tablew[(LED_TAB[13])%8]);hang7();break;
		case 14:layer_cs(tablew[(LED_TAB[14])%8]);hang7();break;
		case 15:layer_cs(tablew[(LED_TAB[15])%8]);hang8();break;
		case 16:layer_cs(tablew[(LED_TAB[15])%8]);hang8();break;
	}		 
}		

//ADC转换完成中断
void ADC_Finish() interrupt 5
{	   
    ADC_CONTR &= !ADC_FLAG; 
    Fft_Real[LIST_TAB[ADC_Count]] = (int)((ADC_RES)<<1)+(ADC_RESL>>1)*2;     //按LIST_TAB表里的顺序，进行存储采样值
    
    if(ADC_Count<=127)
    { 
        ADC_Count++;  
    }
    else
    { 
        EADC=0;
        TR0=0;
    }	  													 
}

void interrupt_print() interrupt 1					 
{	
	uchar i;
	static uchar layer=0;	 			
	if(flag_fft_s == 1)
	{ 
        x1=0; x2=0;x3=0;x4=0;x5=0;x6=0;x7=0;x8=0;
		for(i=0;i<8;i++)
		{
            P2 = 1<<i;
            P0 = display[layer][i];            			
		}
        layer_cs(0x01<<layer);

		if(layer<7) 
        {
            layer++;
        }
		else
        {
            layer=0; 
        }
		TH0=0xFA;
		TL0=0xF0;  	
	}
	//控制采样速率
	else
	{
		ADC_CONTR = ADC_POWER | ADC_SPEEDHH| ADC_START | channel; //开始AD采集
	}
} 