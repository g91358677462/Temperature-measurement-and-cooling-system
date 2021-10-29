#include <regx52.h>
#include <intrins.h>

#define uchar unsigned char

#define uint unsigned int
sbit fan=P2^0;           //定義風扇界面
sbit DS=P2^1;           //定義DS18B20界面
sbit set=P2^2;           //定義設定溫度模式界面
sbit tmpup=P2^3;           //定義增加設定溫度界面
sbit tmpdon=P2^4;           //定義減少設定溫度界面

int temp;   
int setdata[3]={0,0,0};          

uchar flag1;            

void display(unsigned char *lp,unsigned char lc);//數字的顯示函數；lp為指向數組的位址，lc為顯示的個數

void delay();//延時子函數，5個空指令

code unsigned char table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x98,0xbf,0x7f,0xff};
			//七段顯示器 0-9 - . 空 表

code unsigned char displaytable[]={0xf7,0xfb,0xfd,0xfd,0xfe};
                        //當前溫度顯示順序

code unsigned char setdisplaytable[]={0xfb,0xfd,0xfe};
                        //設定溫度顯示順序

unsigned char l_tmpdate[8]={0,0,10,0,0,0,0,0};//定義數組變量，並給予1，2，3，4，5，6，7，8，就是本程式顯示的八個數
int tmp(void);
void tmpchange(void);
void tmpwritebyte(uchar dat);
uchar tmpread(void);
bit tmpreadbit(void);
void dsreset(void);
void delayb(uint count);
void setdisplay(int t);

void main()               //程式開始
{
 uchar i;
 int l_tmp,settmp;
 unsigned int setmod;
 fan=1;
 set=1;
 tmpup=1;
 tmpdon=1;
 setmod=0;
 settmp=300;

  while(1)
  {
   if(set==0) 
   {
    fan=1;
    setmod=1;
    delayb(3);
    while(set==0);
   }
   
   while(setmod)
   {
    setdisplay(settmp);
    if(set==0) 
    {
     setmod=0;
     delayb(3);
     while(set==0);
    }   
    else if(tmpup==0) 
      {
       if(settmp>=1000)  settmp=1000;
       else settmp=settmp+10;
       delayb(3);
       while(tmpup==0);
      }	 

    else if(tmpdon==0)
      {
       if(settmp<=-200)  settmp=-200;
       else settmp=settmp-10;
       delayb(3);
       while(tmpdon==0);
       }
    }
   
   if(setmod==0)
   {
       tmpchange();      //溫度轉換
       l_tmp=tmp();
       if(l_tmp>settmp) fan=0;
       if(l_tmp<settmp) fan=1;
       if(l_tmp<0)  l_tmpdate[0]=10;	//判斷溫度為負溫度，前面加"-"		
       else
	{
	l_tmpdate[0]=temp/1000;	//顯示百位，這裡用1000，是因為我們之前乖以10位了
	if(l_tmpdate[0]==0)		
	l_tmpdate[0]=12;//判斷溫度為正溫度且沒有上百，前面不顯示
	}

    l_tmp=temp%1000;
    l_tmpdate[1]=l_tmp/100;//截取十位
    l_tmp=l_tmp%100;
    l_tmpdate[2]=l_tmp/10;//截取個位
    l_tmpdate[3]=11;	
    l_tmpdate[4]=l_tmp%10;//截取小數第一位

     for(i=0;i<10;i++){		//循環輸出10次，提升亮度
      display(l_tmpdate,5);   
      }
    }
                  
  }
  

}

void setdisplay(int t)
{
  int i,j;
  t=t/10;
  if(t<0)
   {
    t=t*-1;
    setdata[0]=10;
   }
  else
  {
   setdata[0]=t/100;
   if(setdata[0]==0) setdata[0]=12; 
  }
 t=t%100;
 setdata[1]=t/10;
 setdata[2]=t=t%10;

 P3=0xff;
 for(j=0;j<100;j++)
 {
  for(i=0;i<3;i++)
  {
   P1=setdisplaytable[i];
   P3=table[setdata[i]];
   P3=0xff; 
  }
 }
}

void display(unsigned char *lp,unsigned char lc)//顯示
{
	unsigned char i;		//定義變量
	P3=0xff;			//P3為輸出
	for(i=0;i<lc;i++){		//循環顯示
        P1=displaytable[i];             //顯示位數順序
	P3=table[lp[i]];		//查表法得到要顯示數字
	delay();			//延時5個空指令	
	if(i==7)  break;		//檢測顯示完8位否，完成直接退出，不讓P1再加1，否則進位影響到第四位資料
	P3=0xff;			//清除為0，準備顯示下一位數
	}
}
void delay(void)								//空5個指令
{
_nop_();_nop_();_nop_();_nop_();_nop_();
}

void delayb(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}


void dsreset(void)       //DS18B20初始化
{
  uint i;                
  DS=0;
  i=103;
  while(i>0)i--;
  DS=1;
  i=4;
  while(i>0)i--;
}

 

bit tmpreadbit(void)       // 讀一位
{
   uint i;
   bit dat;
   DS=0;i++;          //小延時一下
   DS=1;i++;i++;
   dat=DS;
   i=8;while(i>0)i--;
   return (dat);
}

 

uchar tmpread(void)   //讀一個位元組
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   //讀出的資料最低位在最前面，這樣剛好//一個位元組在DAT裡
  }
 return(dat);             //將一個位元組資料返回
}

 

void tmpwritebyte(uchar dat)   
{                           //寫一個位元組到DS18B20裡
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     // 寫1部分
    {
      DS=0;
      i++;i++;
      DS=1;
      i=8;while(i>0)i--;
    }
    else
    {
      DS=0;       //寫0部分
      i=8;while(i>0)i--;
      DS=1;
      i++;i++;
    }
  }
}

 

void tmpchange(void)  //發送溫度轉換命令
{
  dsreset();             //初始化DS18B20
  delayb(1);             //延時
  tmpwritebyte(0xcc);  // 跳過序列號命令
  tmpwritebyte(0x44);  //發送溫度轉換命令
}

int tmp()               //獲得溫度
{
  float tt;
  uchar a,b;
  dsreset();
  delayb(1);
  tmpwritebyte(0xcc);   
  tmpwritebyte(0xbe);    //發送讀取資料命令
  a=tmpread();          //連續讀兩個位元組資料
  b=tmpread();
  temp=b;
  temp<<=8;             
  temp=temp|a;         //兩位元組合成一個整型變量。
  tt=temp*0.0625;       //得到真實十進製溫度值，因為DS18B20
//可以精確到0.0625度，所以讀回資料的最低位代表的是
//0.0625度。

  temp=tt*10+0.5;   //放大十倍，這樣做的目的將小數點後第一位
//也轉換為可顯示數字，同時進行一個四捨五入操作。

  return temp;     //返回溫度值
}

void delay10ms()            
  {
    uchar a,b;
    for(a=10;a>0;a--)
      for(b=60;b>0;b--);
   }
