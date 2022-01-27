#ifndef SRC_MY_SOFT_I2C_H_
#define SRC_MY_SOFT_I2C_H_

#define I2C_SDA_SET() gpio_set_direction(GPIO_NUM_18, GPIO_MODE_INPUT)
#define I2C_SDA_CLR() gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT)
#define I2C_SDA_IN() gpio_get_level(GPIO_NUM_18)
#define I2C_SCL_SET() gpio_set_direction(GPIO_NUM_19, GPIO_MODE_INPUT)
#define I2C_SCL_CLR() gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT)
#define I2C_SCL_H() gpio_set_level(GPIO_NUM_19,1)
#define I2C_SCL_L() gpio_set_level(GPIO_NUM_19,0)
#define I2C_SDA_H() gpio_set_level(GPIO_NUM_18,1)
#define I2C_SDA_L() gpio_set_level(GPIO_NUM_18,0)

void i2c_start();
void i2c_stop();
void transmit_ack(unsigned char ack);
unsigned char receive_ack();
void transmit_char(unsigned char data);
unsigned char receive_char(unsigned char ack);
void soft_i2c_init();




#endif /* SRC_MY_SOFT_I2C_H_ */