#ifndef SRC_MY_SOFT_SPI_H_
#define SRC_MY_SOFT_SPI_H_

#define SPI_MISO_DIR_IN() gpio_set_direction(GPIO_NUM_22, GPIO_MODE_INPUT)
#define SPI_MISO_GET() gpio_get_level(GPIO_NUM_22)
#define SPI_MOSI_DIR_OUT() gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT)
#define SPI_MOSI_HIGH() gpio_set_level(GPIO_NUM_18, 1)
#define SPI_MOSI_LOW() gpio_set_level(GPIO_NUM_18, 0)
#define SPI_SCK_DIR_OUT() gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT)
#define SPI_SCK_HIGH() gpio_set_level(GPIO_NUM_19, 1)
#define SPI_SCK_LOW() gpio_set_level(GPIO_NUM_19, 0)

void spi_mode(int mode);
void soft_spi_init();
void soft_spi_write_byte(unsigned char dat);
unsigned char soft_spi_read_byte();

#endif /* SRC_MY_SOFT_SPI_H_ */