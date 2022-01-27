//Zuoge 2020-12-04
//软件I2C设置
//设置引脚 GPIO18-->SDA
//         GPIO19-->SCL


#include <stdio.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"
#include "my_soft_i2c.h"

void i2c_start(void)
{
	I2C_SDA_H();
	//ets_delay_us(1);
	I2C_SCL_H();
	//ets_delay_us(1);
	I2C_SDA_L();
	//ets_delay_us(1);
	I2C_SCL_L();
}

void i2c_stop(void)
{
    I2C_SCL_L();
    //ets_delay_us(1);
    I2C_SDA_L();
	//ets_delay_us(1);
	I2C_SCL_H();
	//ets_delay_us(1);
	I2C_SDA_H();
}

void transmit_ack(unsigned char ack)
{
	if (ack) {
		I2C_SDA_L();
	} else {
		I2C_SDA_H();
	}
	//ets_delay_us(1);
	I2C_SCL_H();
	//ets_delay_us(1);
	I2C_SCL_L();
}

unsigned char receive_ack(void)
{
	unsigned char temp;

	I2C_SDA_H();
	//ets_delay_us(1);
	I2C_SCL_H();
	temp = I2C_SDA_IN();
	//ets_delay_us(1);
	I2C_SCL_L();

	return temp;
}

void transmit_char(unsigned char data)
{
	unsigned char i=0;
	for (i=8; i>0; i--) {
		if (data & 0x80) {
			I2C_SDA_H();
		} else {
			I2C_SDA_L();
		}
		data <<= 1;
		//ets_delay_us(1);
		I2C_SCL_H();
		//ets_delay_us(1);
		I2C_SCL_L();
	}
	receive_ack();
}

unsigned char receive_char(unsigned char ack)
{
	unsigned char i=0;
	unsigned char temp=0;
	I2C_SDA_H();
	for (i=8; i>0; i--) {
		//ets_delay_us(1);
		I2C_SCL_H();
		temp = temp<<1;
		if (I2C_SDA_IN()) {
			temp |= 0x01;
		}
		//ets_delay_us(1);
		I2C_SCL_L();
	}
	transmit_ack(ack);
	return temp;
}


void soft_i2c_init()
{
    I2C_SDA_CLR();
    gpio_set_level(GPIO_NUM_18, 0);
    I2C_SCL_CLR();
    gpio_set_level(GPIO_NUM_19, 0);
}