#ifndef _I2C_H
#define _I2C_H

void delay(unsigned int t);
void ACK(void);
void change_pin_In(void);
void change_pin_out(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(unsigned char dat);
void delay_us(unsigned int count);
void eeprom_read(unsigned char page);
unsigned char i2c_read(void);

#endif
