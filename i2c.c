#include "r_cg_macrodriver.h"
#include "var_define.h"
#include "i2c.h"
#include "r_cg_port.h"

/************PARAM*************/
extern unsigned char UC_C_Read_eeprom_Data[150];
unsigned char UC_I2C_LENGTH=150;


void eeprom_read(unsigned char page)
{
    unsigned int i;
    i2c_start();
    i2c_write(0xa0);        // 写入器件地址
    if(page==0)
    {
    	i2c_write(0x00);    // 写入存储地址
    }
    i2c_start();
    i2c_write(0xa1);    // 写入读取命令
    change_pin_In();
    for(i=0;i<UC_I2C_LENGTH;i++)
    {
	 UC_C_Read_eeprom_Data[i]=i2c_read();
	 ACK();								//连续接受需发送应答信号后才能接受下一个数据
    }
    i2c_stop();
}

void delay_us(unsigned int count)
{
    unsigned int i;
    for (i = 0; i < count; i++);
}

void i2c_start(void)
{
    SDA = 1;
    SCL = 1;
    delay(1);
    SDA = 0;
    delay(1);
    SCL = 0;
}

void i2c_stop(void)
{
    SDA = 0;
    SCL = 1;
    delay(1);
    SDA = 1;
    delay(1);
}

void i2c_write(unsigned char dat)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
	if(dat&0x80)
	{
		 SDA = 1;
	}
	else
	{
		 SDA = 0;
	}
	dat <<= 1;
	delay(1);
        SCL = 1;
	delay(1);
        SCL = 0;
	delay(1);
    }
    ACK();
    //delay_us(5000);
}


unsigned char Waite_ASK(void)		//等待应答信号
{
	SCL=0;
	change_pin_In();
	delay(1);
	SCL=1;
	delay(1);
	if(SDA)
	{
		SCL=0;
		change_pin_out();
		SDA=0;
		SCL=0;
		return 0;		//无应答，返回0
	}
	else
	{
		SCL=0;
		change_pin_out();
		SDA=0;
		SCL=0;
		return 1;		//拉低，有应答返回1
	}
}


/*********每成功地传送一个字节数据后 接收器都必须产生一个应答信号 应答的器件在第 9 个时钟周期时将 SDA 线拉低 表示其已收到一个 8 位数据。*/
void ACK(void)		//发送应答信号
{
	SCL=0;
	delay(1);
	change_pin_out();
	SDA=0;
	SCL=1;
	delay(1);
	SCL=0;
}

unsigned char i2c_read(void)
{
    unsigned char i, dat = 0;
    change_pin_In();
    for(i = 0; i < 8; i++)
    {
	SCL=0;
	delay(1);
	SCL = 1;
        dat <<= 1;
	dat |= SDA;
    }
    SCL = 0;
    return dat;
}

void delay(unsigned int t) // 延时函数
{
   while (t--)
    {
        NOP();
        NOP();
        NOP();
        NOP();
    }
}


void change_pin_In(void)		//中文
{
    P2 = _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn5_OUTPUT_0;
    PM2 = _01_PMn0_NOT_USE | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _08_PMn3_MODE_INPUT | _10_PMn4_MODE_INPUT |
    _00_PMn5_MODE_OUTPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
}

void change_pin_out(void)		//数据口更改为输出模式
{
    P2 = _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn5_OUTPUT_0;
    PM2 = _01_PMn0_NOT_USE | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _10_PMn4_MODE_INPUT |
    _00_PMn5_MODE_OUTPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
}