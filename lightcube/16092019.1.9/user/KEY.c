#include"STC12C5A60S2.H" //оƬͷ�ļ�
typedef unsigned char uchar;
typedef unsigned int  uint; 
//���尴���ܽ�
sbit key_left = P1^5;
sbit key_middle = P1^6;
sbit key_right = P1^7;

//ȫ�ֱ���
unsigned char key_flag = 0;
extern void delay(uint i);
//����ɨ�躯��
void key_scan()
{	
	if(key_left == 0)
    {
       delay(10);
       while(!key_left)
       key_flag = 1;

    }
    else if(key_middle == 0)
    {
       delay(10);
       while(!key_middle)
       key_flag = 2;        
    }
    else if(key_right == 0)
    {
       delay(10);
       while(!key_right)
       key_flag = 3;        
    }
    else
    {
        key_flag = 0;
    } 

}