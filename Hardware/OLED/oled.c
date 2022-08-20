//////////////////////////////////////////////////////////////////////////////////	 
//
//
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源（V1.2底板已接好3.3V）
//              SCL   接PA4（SCL）（V1.2底板已接好）
//              SDA   接PA5（SDA）（V1.2底板已接好）         
//              ----------------------------------------------------------------
//
//修改说明：修改引脚需要修改本文件的OLED_Init()函数，还有 oled.h 中的第28、29、31、32行
//////////////////////////////////////////////////////////////////////////////////

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "ccd.h"
#include "adc.h"	
#include "usart.h"
#include "string.h"

int i;
unsigned char OLED_GRAM[144][8];
unsigned char OLED_GRAM1[144][8];

 /*
			OLED使用须知：
			1、全中文、字体为“16 X 16 , 宋体 ”显示时最多可显示三行，每行　7　个字    OLED_ShowCHinese(u8 x,u8 y,u8 no)函数
	　　	x坐标的范围为 x>=0　＆＆　x<=113，相邻两字间建议相差16
				y坐标的范围为 y>=0　＆＆　y<=6，相邻两字符间建议相差16，注意，最多可容纳4行字
	
			2、英文、字体大小为“16”显示时最多可显示4行，每行16个字符                 OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)函数
				x坐标的范围为 x>=0　＆＆　x<=121
				y坐标的范围为 y>=0　＆＆　y<=6，相邻两字符间建议相差16，注意，最多可容纳4行字
				
			3、中文字库存放在 oledfont.h 中，需要用字库软件生成
	
	*/

// OLED显示函数，建议将显示的内容写到此处
void OLED_Show(void)
{
//	int t;
	OLED_Clear();
	// 版本号1
//	OLED_ShowChinese(0,0,18,16);   // 中
//	OLED_ShowChinese(16,0,17,16);  // 值
//	OLED_ShowChar(32,0,':',16);
//	OLED_ShowNum(48,0,CCD_Zhongzhi,2,16);
//	OLED_ShowChinese(0,16,16,16);   // 阈
//	OLED_ShowChinese(16,16,17,16);  // 值
//	OLED_ShowChar(32,16,':',16);
////	OLED_ShowNum(48,16,CCD_Yuzhi,2,16);
//	for(t=5;t<123;t++)
//	{
//		if(ccd_adc[t]<CCD_Yuzhi)
//			OLED_DrawPoint(t,36);
//	}
//	OLED_ShowString(0,0,"TIM2-CH1:",16);

//	OLED_ShowChinese(0,48,13,16);   // 版
//	OLED_ShowChinese(16,48,14,16);  // 本
//	OLED_ShowChinese(32,48,15,16);  // 号
//	OLED_ShowString(48,48,":V0.0.1",16); //显示字符串
//	OLED_ShowChinese(0,48,0,16);  // 电
//	OLED_ShowChinese(16,48,1,16);  // 子
//	OLED_ShowChinese(32,48,2,16);  // 工
//	OLED_ShowChinese(48,48,3,16);  // 程
//	OLED_ShowChinese(64,48,4,16);  // 学
//	OLED_ShowChinese(80,48,5,16);  // 院
  //delay_ms(5);
	// 版本号2
//	OLED_ShowString(48,32,":remake",16); //显示字符串
  //delay_ms(5);
//	OLED_Refresh();
}

//m^n
unsigned int OLED_Pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//起始信号
void I2C_Start(void)
{
	OLED_SDIN_Set();
	OLED_SCLK_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

//结束信号
void I2C_Stop(void)
{
	OLED_SCLK_Set();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}

//写入一个字节
void Send_Byte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();//将时钟信号设置为低电平
		if(dat&0x80)//将dat的8位从最高位依次写入
		{
			OLED_SDIN_Set();
    }
		else
		{
			OLED_SDIN_Clr();
    }
		OLED_SCLK_Set();//将时钟信号设置为高电平
		OLED_SCLK_Clr();//将时钟信号设置为低电平
		dat<<=1;
  }
}





//反显函数
void OLED_ColorTurn1(unsigned char i)
{
	if(i==0)
		{
			OLED_WR_Byte1(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte1(0xA7,OLED_CMD);//反色显示
		}
}

//屏幕旋转180度
void OLED_DisplayTurn1(unsigned char i)
{
	if(i==0)
		{
			OLED_WR_Byte1(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte1(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte1(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte1(0xA0,OLED_CMD);
		}
}





//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte1(unsigned char dat,unsigned char mode)
{
	I2C_Start();
	Send_Byte(0x7A);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}


//开启OLED显示 
void OLED_DisPlay_On1(void)
{
	OLED_WR_Byte1(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte1(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte1(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off1(void)
{
	OLED_WR_Byte1(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte1(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte1(0xAF,OLED_CMD);//关闭屏幕
}

//更新显存到OLED	
void OLED_Refresh1(void)
{
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte1(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte1(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte1(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte1(OLED_GRAM1[n][i],OLED_DATA);
  }
}
//清屏函数
void OLED_Clear1(void)
{
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM1[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh1();//更新显示
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint1(unsigned char x,unsigned char y)
{
	unsigned char i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM1[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint1(unsigned char x,unsigned char y)
{
	unsigned char i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM1[x][i]=~OLED_GRAM1[x][i];
	OLED_GRAM1[x][i]|=n;
	OLED_GRAM1[x][i]=~OLED_GRAM1[x][i];
}


//画线
//x:0~128
//y:0~64
void OLED_DrawLine1(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
	unsigned char i,k,k1,k2;
	if((1)||(x2>128)||(1)||(y2>64)||(x1>x2)||(y1>y2))return;
	if(x1==x2)    //画竖线
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint1(x1,y1+i);
			}
  }
	else if(y1==y2)   //画横线
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint1(x1+i,y1);
			}
  }
	else      //画斜线
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
			{
			  OLED_DrawPoint1(x1+i,y1+i*k/10);
			}
	}
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle1(unsigned char x,unsigned char y,unsigned char r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint1(x + a, y - b);
        OLED_DrawPoint1(x - a, y - b);
        OLED_DrawPoint1(x - a, y + b);
        OLED_DrawPoint1(x + a, y + b);
 
        OLED_DrawPoint1(x + b, y + a);
        OLED_DrawPoint1(x + b, y - a);
        OLED_DrawPoint1(x - b, y - a);
        OLED_DrawPoint1(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
void OLED_ShowChar1(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1)
{
	unsigned char i,m,temp,size2,chr1;
	unsigned char y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
				for(m=0;m<8;m++)           //写入数据
				{
					if(temp&0x80)OLED_DrawPoint1(x,y);
					else OLED_ClearPoint1(x,y);
					temp<<=1;
					y++;
					if((y-y0)==size1)
					{
						y=y0;
						x++;
						break;
          }
				}
  }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString1(unsigned char x,unsigned char y,unsigned char *chr,unsigned char size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar1(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //换行
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}


////显示2个数字
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
void OLED_ShowNum1(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size1)
{
	unsigned char t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar1(x+(size1/2)*t,y,'0',size1);
      }
			else 
			{
			  OLED_ShowChar1(x+(size1/2)*t,y,temp+'0',size1);
			}
  }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
void OLED_ShowChinese1(unsigned char x,unsigned char y,unsigned char num,unsigned char size1)
{
	unsigned char i,m,n=0,temp,chr1;
	unsigned char x0=x,y0=y;
	unsigned char size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Hzk1[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=Hzk4[chr1][i];}//调用64*64字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint1(x,y);
								else OLED_ClearPoint1(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}

//num 显示汉字的个数
//space 每一遍显示的间隔
void OLED_ScrollDisplay1(unsigned char num,unsigned char space)
{
	unsigned char i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese1(128,24,t,16); //写入一个汉字保存在OLED_GRAM1[][]数组中
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=0;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM1[i-1][n]=OLED_GRAM1[i][n];
							}
						}
           OLED_Refresh1();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=0;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM1[i-1][n]=OLED_GRAM1[i][n];
			}
		}
		OLED_Refresh1();
	}
}

//配置写入数据的起始位置
void OLED_WR_BP1(unsigned char x,unsigned char y)
{
	OLED_WR_Byte1(0xb0+y,OLED_CMD);//设置行起始地址
	OLED_WR_Byte1(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte1((x&0x0f)|0x01,OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture1(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x=0,y=0;
	if(y%8==0)y=0;
	else y+=1;
	for(y=y0;y<y1;y++)
	 {
		 OLED_WR_BP1(x0,y);
		 for(x=x0;x<x1;x++)
		 {
			 OLED_WR_Byte1(BMP[j],OLED_DATA);
			 j++;
     }
	 }
}


//反显函数
void OLED_ColorTurn(unsigned char i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}

//屏幕旋转180度
void OLED_DisplayTurn(unsigned char i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}



//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(unsigned char dat,unsigned char mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}


//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//关闭屏幕
}

//更新显存到OLED	
void OLED_Refresh(void)
{
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
//清屏函数
void OLED_Clear(void)
{
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint(unsigned char x,unsigned char y)
{
	unsigned char i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(unsigned char x,unsigned char y)
{
	unsigned char i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}


//画线
//x:0~128
//y:0~64
void OLED_DrawLine(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
	unsigned char i,k,k1,k2;
	if((1)||(x2>128)||(1)||(y2>64)||(x1>x2)||(y1>y2))return;
	if(x1==x2)    //画竖线
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint(x1,y1+i);
			}
  }
	else if(y1==y2)   //画横线
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1);
			}
  }
	else      //画斜线
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
			{
			  OLED_DrawPoint(x1+i,y1+i*k/10);
			}
	}
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(unsigned char x,unsigned char y,unsigned char r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);
 
        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1)
{
	unsigned char i,m,temp,size2,chr1;
	unsigned char y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
				for(m=0;m<8;m++)           //写入数据
				{
					if(temp&0x80)OLED_DrawPoint(x,y);
					else OLED_ClearPoint(x,y);
					temp<<=1;
					y++;
					if((y-y0)==size1)
					{
						y=y0;
						x++;
						break;
          }
				}
  }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //换行
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}


////显示2个数字
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size1)
{
	unsigned char t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
			}
  }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char num,unsigned char size1)
{
	unsigned char i,m,n=0,temp,chr1;
	unsigned char x0=x,y0=y;
	unsigned char size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Hzk1[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=Hzk4[chr1][i];}//调用64*64字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}

//num 显示汉字的个数
//space 每一遍显示的间隔
void OLED_ScrollDisplay(unsigned char num,unsigned char space)
{
	unsigned char i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=0;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=0;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//配置写入数据的起始位置
void OLED_WR_BP(unsigned char x,unsigned char y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x=0,y=0;
	if(y%8==0)y=0;
	else y+=1;
	for(y=y0;y<y1;y++)
	 {
		 OLED_WR_BP(x0,y);
		 for(x=x0;x<x1;x++)
		 {
			 OLED_WR_Byte(BMP[j],OLED_DATA);
			 j++;
     }
	 }
}


//OLED的初始化
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOD4，5
 	GPIO_SetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9);	
//	GPIO_InitTypeDef  GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD4，5
// 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_4);	
	
	delay_ms(200);
	
	OLED_WR_Byte1(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte1(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte1(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte1(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte1(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte1(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte1(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte1(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte1(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte1(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte1(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte1(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte1(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte1(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte1(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte1(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte1(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte1(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte1(0x12,OLED_CMD);
	OLED_WR_Byte1(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte1(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte1(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte1(0x02,OLED_CMD);//
	OLED_WR_Byte1(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte1(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte1(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte1(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte1(0xAF,OLED_CMD);
	OLED_Clear1();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
	
}
