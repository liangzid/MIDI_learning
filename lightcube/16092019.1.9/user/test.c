#include <intrins.h>	 //
#include"STC12C5A60S2.H" //оƬͷ�ļ�
#include"STC12C5A60S2.H" //оƬͷ�ļ�
#define channel      0x01		  //����ADͨ��Ϊ P1.0
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
extern void FFT(); //��2fft
extern void flash_diy(void);

extern code  char LIST_TAB[128] ;
extern int   xdata Fft_Real[128]; 				//FFT��ʵ��
extern int   xdata Fft_Image[128];             //FFT���鲿 
extern uchar xdata LED_TAB[16];				//��¼��ɫ��״
extern uchar key_flag ;
extern uchar xdata LED_TAB[16];				//��¼��ɫ��״  
uchar flag_fft_s;
uchar ADC_Count=0,G=0,T;
uchar code tablew[]={0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE}; 
#define     NEW     1
#if         NEW
sbit x1 = P1^2;         //����P1.2��Ϊ��һ��Ĳ�ѡ�ź�
sbit x2 = P1^3;         //����P1.3��Ϊ�ڶ���Ĳ�ѡ�ź� 
sbit x3 = P1^4;         //����P1.4��Ϊ������Ĳ�ѡ�ź�
sbit x4 = P3^3;         //����P3.3��Ϊ���ĵڵĲ�ѡ�ź� 
sbit x5 = P3^4;         //����P3.4��Ϊ�����Ĳ�ѡ�ź�
sbit x6 = P3^5;         //����P3.5��Ϊ�����ڵĲ�ѡ�ź� 
sbit x7 = P3^7;         //����P3.7��Ϊ���߲�Ĳ�ѡ�ź�
sbit x8 = P3^6;         //����P3.6��Ϊ�ڰ˲�Ĳ�ѡ�ź�
#else
sbit x1 = P1^4;         //����P1.4��Ϊ��һ��Ĳ�ѡ�ź�
sbit x2 = P1^2;         //����P1.2��Ϊ�ڶ���Ĳ�ѡ�ź� 
sbit x3 = P3^2;         //����P3.2��Ϊ������Ĳ�ѡ�ź�
sbit x4 = P3^3;         //����P3.3��Ϊ���ĵڶ���ѡ�ź� 
sbit x5 = P3^4;         //����P3.4��Ϊ�����Ĳ�ѡ�ź�
sbit x6 = P3^5;         //����P3.5��Ϊ�����ڶ���ѡ�ź� 
sbit x7 = P3^6;         //����P3.6��Ϊ���߲�Ĳ�ѡ�ź�
sbit x8 = P3^7;         //����P3.7��Ϊ�ڰ˲�Ĳ�ѡ�ź�
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
//���ܳ�ʼ��
void Init()
{  
	P0 = 0x00; //74HC573���ݿ�
	P2 = 0x00; //74HC573λѡ��
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
    REN = 1;            //�����ڽ���
    ES = 1;             //�����ж�
    EA = 1;    
}
/******** ������ ***************************************************************************************/

/***************************************
�������ƣ��жϺ���
�����������
�� �� ֵ����
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
	uchar i = 0; //forѭ������
    uchar j = 0; //forѭ������
	Init();
	while(1)
	{					
		clear(0);	//���ˢ����ʾ������ 
		key_scan();	//ɨ�谴��
		
		
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
            flag_fft_s = 1;  //��ʱ��0���ܿ���
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
                /*PC����ת��*/
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
                /*��ջ�����*/
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
            //IO��ģʽ��ʼ��
            P3M0 = 0xff; //ULN2803���������Ϊ�������
            P3M1 = 0x00;
            P0M0 = 0xff; //P0��P2���ڿ���74HC573,�������
            P0M1 = 0x00;
            P2M0 = 0xff;
            P2M1 = 0x00;           
            //----- ADC��ʼ�� ------------------------------------------------------------------------------
        	P1ASF  = 0x02;          //0000,0010, �� P1.1 �ó�ģ���
        	AUXR1 &=0xFB;           //1111,1011, �� ADRJ=0
        	EADC   =1;				//AD�жϴ�
        	ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | channel; //1110 1001   1�� A/D ��ADC_POWER��ת����Դ��11�ٶ�Ϊ90����һ�Σ�
        	    		                                               //0�жϱ�־���㣻1����adc(ADC_START);001ADͨ���򿪣�����ΪP1.1��;        
        	ET0 = 1;			  //��ʱ��0�ж� ����
        	TR0 = 0;			  //��ʱ��0     �ر�
        	ET1 = 1;			  //��ʱ��1�ж� ����
        	TR1 = 0;              //��ʱ��1     �ر�
        	PT1 = 0;			  //��ʱ��1 �����ȼ�
        	PT0 = 1;	 		  //��ʱ��0 �����ȼ�
        	IPH = PADCH;
        	IP  = PADC;			  //�ж����ȼ�
        	EA  = 1;			  //���жϴ� 
        	clear(0);			  //�����ʾ������
			TMOD = 0x12; //T0: 8λ�Զ���װ      T1: 16λģʽ
			TH0  = 0x60; //ADC�������ڣ�80us��6.66us��  ������ 12.5 KHz                              				  
			TL0  = 0x60; //
			TH1  = 0xfd; //ˢ����ʾ���ڣ�300us��25us�� 	ˢ���� 										   
			TL1  = 0Xa8; //
			while(1)
			{
				flag_fft_s = 0;  
				TR1 = 1; //ˢ����ʾ
				TR0 = 1; //���Ʋ�������
				//P0    = 0xaa; //P0 δ֪����1010 1010
				ADC_Count=0;				
				TR1  = 1;     //������ʱ��
				TR0  = 1;     //������ʱ���ж�0
				EADC = 1;	  //����ADC 
				while(ADC_Count<128); //ת��128��
				for(i=0;i<128;i++)	  //FFT�����鲿��ֵ
				{
					Fft_Image[i]=0;	  //ԭʼ���� �鲿��ֵΪ0
				}
				FFT();                //FFT���㲢ת��Ϊ��Ƶ�η�ֵ	

				//������ť������
				//key_scan();
				//if(key_flag != 2) break;
			}																																										
		}

		//˳��ѭ��
		if(key_flag == 3)
		{       
            TMOD=0x11;
            EA  = 0;			  //��ж��
            flag_fft_s = 1;  //��ʱ��0���ܿ���
            PT0 = 1;	 		  //��ʱ��0 �����ȼ�
            ET0 = 1;			  //��ʱ��0�ж� ����
            ES = 0;             //�����ж�
            TR0 = 1;	  //��������0 
            EA  = 1;			  //��ж��
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


//��ʱ��1�ж� ����ˢ����ʾ(ֻ��Ƶ����ʾʱʹ��)
void interrupt_fft_display() interrupt 3					 //�ж�һ����ʾһ��
{	 	 
	TH1  = 0xfd; //ˢ����ʾ���ڣ�300us��25us�� 	ˢ���� 										   
	TL1  = 0Xa8; //
	G++;						  	
	if(G>=17) G=1;
	switch(G)								     //�����������һ�е�����
	{		
		case 1: layer_cs(tablew[(LED_TAB[1] )%8]);hang1();break;	 //(LED_TAB[1] )%8ȡ�˵�����
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

//ADCת������ж�
void ADC_Finish() interrupt 5
{	   
    ADC_CONTR &= !ADC_FLAG; 
    Fft_Real[LIST_TAB[ADC_Count]] = (int)((ADC_RES)<<1)+(ADC_RESL>>1)*2;     //��LIST_TAB�����˳�򣬽��д洢����ֵ
    
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
	//���Ʋ�������
	else
	{
		ADC_CONTR = ADC_POWER | ADC_SPEEDHH| ADC_START | channel; //��ʼAD�ɼ�
	}
} 