```
//IO 方向设置
#define SDA_IN() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
```
STM32 的 IO 口可以由软件配置成如下 8 种模式：<br>
```
    1、输入浮空<br>
    2、输入上拉<br>
    3、输入下拉<br>
    4、模拟输入<br>
    5、开漏输出<br>
    6、推挽输出<br>
    7、推挽式复用功能<br>
    8、开漏复用功能<br>
```
每个 IO 口可以自由编程，但 IO 口寄存器必须要按 32 位字被访问。STM32 的很多 IO 口都是 5V 兼容的，这些 IO 口在与 5V 电平的外设连接的时候很有优势，具体哪些 IO 口是 5V 兼容的，可以从该芯片的数据手册管脚描述章节查到（I/O Level 标 FT 的就是 5V 电平兼容的）。STM32 的每个 IO 端口都有 7 个寄存器来控制。他们分别是：<br>
```
配置模式的 2 个 32 位的端口配置寄存器 CRL 和 CRH
2 个 32 位的数据寄存器 IDR 和 ODR
1 个 32 位的置位/复位寄存器BSRR
一个 16 位的复位寄存器 BRR
1 个 32 位的锁存寄存器 LCKR
```
我们常用的 IO 端口寄存器只有 4 个：CRL、CRH、IDR、ODR。CRL 和 CRH 控制着每个 IO 口的模式及输出速率。<br>
STM32 的 IO 口位配置表如表所示：<br>
![1](/插图/1.png)<br>
STM32 输出模式配置如表:<br>
![2](/插图/2.png)<br>
接下来我们看看端口低配置寄存器 **CRL** 的描述和端口高配置寄存器 **CRH** 的描述<br>
![3](/插图/3.png)<br>
![4](/插图/4.png)<br>
STM32 的 **CRL** 控制着每组 IO 端口（A-G）的低 8 位的模式。<br>
每个 IO 端口占用 **CRL** 的 4 个位，高两位为 CNF，低两位为 MODE。<br>
同理 **CRH** 控制着每组 IO 端口（A-G）的高 8 位的模式。<br>
换句话说我们要控制PA0,就只要这样写:<br>
```
GPIOA->CRL&=0XFFFFFFF0；
//GPIOA->CRL=GPIOA->CRL & 0b1111 1111 1111 1111 1111 1111 1111 0000；
```
把PA0设置为输入就要这样写：<br>
```
GPIOA->CRL|=8<<0;
//GPIOA->CRL=GPIOA->CRL|(0b1000)<<0;
```
合起来控制PA0为输入就是这样写：<br>
```
GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;
```

**下面是各个IO口输入输出模式的配置：**<br>
_输入模式的配置：_<br>
PA0为输入(上/下拉)<br>
```
GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;
```
PA1为输入(上/下拉)<br>
```
GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=8<<4;
```
PA2为输入(上/下拉)<br>
```
GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=8<<8;
```
PA3为输入(上/下拉)<br>
```
GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=8<<12;
```
PA4为输入(上/下拉)<br>
```
GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=8<<16;
```
PA5为输入(上/下拉)<br>
```
GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=8<<20;
```
PA6为输入(上/下拉)<br>
```
GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=8<<24;
```
PA7为输入(上/下拉)<br>
```
GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=8<<28;
```
PA8为输入(上/下拉)<br>
```
GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8<<0;
```
PA9为输入(上/下拉)<br>
```
GPIOA->CRH&=0XFFFFFF0F;GPIOA->CRH|=8<<4;
```
PA10为输入(上/下拉)<br>
```
GPIOA->CRH&=0XFFFFF0FF;GPIOA->CRH|=8<<8;
```
PA11为输入(上/下拉)<br>
```
GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;
```
PA12为输入(上/下拉)<br>
```
GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=8<<16;
```
PA13为输入(上/下拉)<br>
```
GPIOA->CRH&=0XFF0FFFFF;GPIOA->CRH|=8<<20;
```
PA14为输入(上/下拉)<br>
```
GPIOA->CRH&=0XF0FFFFFF;GPIOA->CRH|=8<<24;
```
PA15为输入(上/下拉)<br>
```
GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=8<<28;
```

_同理设置为输出就是:_<br>
PA0为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;
```
PA1为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=3<<4;
```
PA2为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=3<<8;
```
PA3为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=3<<12;
```
PA4为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=3<<16;
```
PA5为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=3<<20;
```
PA6为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=3<<24;
```
PA7为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=3<<28;
```
PA8为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=3<<0;
```
PA9为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0XFFFFFF0F;GPIOA->CRH|=3<<4;
```
PA10为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0XFFFFF0FF;GPIOA->CRH|=3<<8;
```
PA11为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;
```
PA12为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=3<<16;
```
PA13为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0XFF0FFFFF;GPIOA->CRH|=3<<20;
```
PA14为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0XF0FFFFFF;GPIOA->CRH|=3<<24;
```
PA15为输出（通用推挽输出50MHZ）<br>
```
GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=3<<28;
```
--------------------------------------------------------------------------------------------
```
GPIOC->CRL&=0XFFFFFFF0;
```
GPIOx->CRL,这句话表示要操作GPIOx的低8位，就是Px0 ~ Px7<br>
GPIOx->CRH,这句话表示要操作GPIOx的高8位，就是Px8 ~ Px15<br>
<br>
所以GPIOC->CRL,这句话表示要操作GPIOC，<br>
<br>
后面的0XFFFFFFF0,表示操作PC0;<br>
0XFFFFFF0F,表示操作PC1;<br>
0XFFFFF0FF,表示操作PC2;<br>
0XFFFF0FFF,表示操作PC3;<br>
0XFFF0FFFF,表示操作PC4;<br>
0XFF0FFFFF,表示操作PC5;<br>
0XF0FFFFFF,表示操作PC3;<br>
0X0FFFFFFF,表示操作PC7;<br>
<br>
合起来的意思就是：利用“与”运算，把这个位清0，同时不影响其他的位的设置。<br>
```
GPIOC->CRL|=8<<0;
```
意思就是将1000左移0位(不移位)，然后再与GPIOC->CRL进行“或”运算。<br>
<br>
再根据原子的寄存器开发手册可以知道CNF0[10]、MODEO[00],对应的就是设置为上拉/输入模式。<br>
<br>
