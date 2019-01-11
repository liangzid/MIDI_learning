#include"STC12C5A60S2.H" //芯片头文件
typedef unsigned char uchar;
typedef unsigned int  uint; 
#define MAX_CHAR    14 //开机依次要显示几个字母

extern unsigned char xdata display[8][8];
extern unsigned char xdata display2[8][8];
extern unsigned char xdata uart_buffer[65];
extern void delay(unsigned int i);
extern void clear(char le);
void con(void);
code char ch[MAX_CHAR][64] ={
/*取模的后的第一个字母*/


{0x00, 0xFF,0x00,0x7E, 0x81,0x81,0x81,0x7E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },//10

{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x79, 0x89, 0x89, 0x89, 0x89, 0x7E, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},//9 

{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x36, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},//8

{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },//7


{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0E, 0x11, 0x31, 0x51, 0x91, 0x0E, 0x00,                         
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },//6

{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xF9, 0x89, 0x89, 0x89, 0x8F, 0x00,                         
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },//5

{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xF8, 0x08, 0x08, 0x08, 0xFF, 0x08, 0x00,                              
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },//4

{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x89, 0x89, 0x89, 0x89, 0x89, 0xFF, 0x00,  },//3            


{0x00, 0x9F, 0x91, 0x91, 0x91, 0x91, 0xF1, 0x00,                         
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },//2

{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,                              
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },//1

{0x00, 0xFF, 0x00, 0x0E, 0x11, 0x21, 0x51, 0x8E, //16
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
},
/*取模的后的第二个字母*/
{
0x3E, 0x41, 0x41, 0x3E, 0x30, 0x49, 0x49, 0x3E, //09
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
},
//取模的后的第三个字母
//{
//0x27, 0x45, 0x49, 0x31, 0x3E, 0x41, 0x41, 0x3E, //20
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//},
//取模的后的第四个字母
//{	   
//0x00, 0xFF, 0x00, 0x00, 0x71, 0x89, 0x89, 0x7E, //19
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//},
/*取模的后的第五个字母*/
//{
//0x01, 0x00, 0x7F, 0x01, 0x00, 0x79, 0x49, 0x7F, //.1.9
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//},
//{
//0x18, 0x18, 0x24, 0xA2, 0xE2, 0xAA, 0x22, 0x1C, //jingyu
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//},
//{
//0x38, 0x7C, 0x66, 0x33, 0x66, 0x7C, 0x38, 0x00, //xin
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//},
//{
//0x67, 0x8E, 0x6E, 0x87, 0x87, 0x67, 0x8E, 0x6E, //3SHAN
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//},
};

void flash_diy(void)
{
    unsigned char i;
    unsigned char j;

    for(j=0;j<MAX_CHAR;j++)
    {
        /*三个字母之间的时间间隔可以适当调整延时delay的参数最大值是 65535*/
        delay(10000);
        delay(10000);
        delay(10000);
		delay(10000);
        for(i=0;i<64;i++)
        {
            uart_buffer[i+1] = ch[j][i];
        }
        con();//转换数据并更新换缓冲区 
    }
}


void con(void)
{
    unsigned char i;
    unsigned char j;

    /*清空数据*/
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
    clear(0);
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
