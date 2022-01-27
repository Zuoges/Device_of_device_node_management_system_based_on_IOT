/****************************
Zuoge
2021年1月4日

SPI_SCK---------->D19
SPI_MOSI--------->D18
SPI_MISO--------->D22

CPOL为0时，空闲时钟在低电平
CPOL为1时，空闲时钟在高电平
CPHA为0时，从时钟第一个边沿进行采样
CPHA为1时，从时钟第二个边沿进行采样

MODE    CPOL    CPHA
0       0       0
1       0       1
2       1       0
3       1       1

*****************************/

#include <stdio.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"
#include "my_soft_spi.h"


unsigned char CPOL=0;
unsigned char CPHA=0;

void spi_mode(int mode)
{
    switch(mode)
    {
        case 0:             //模式零
        {
            CPOL=0;
            CPHA=0;
            break;
        }
        case 1:             //模式一
        {
            CPOL=0;
            CPHA=1;
            break;
        }
        case 2:             //模式二
        {
            CPOL=1;
            CPHA=0;
            break;
        }
        case 3:             //模式三
        {
            CPOL=1;
            CPHA=1;
            break;
        }
        default:break;
    }
}

void soft_spi_init()
{
    SPI_MISO_DIR_IN();
    SPI_MOSI_DIR_OUT();
    SPI_SCK_DIR_OUT();
    if(CPOL==0)
        SPI_SCK_LOW();
    else
        SPI_SCK_HIGH();
}

void soft_spi_write_byte(unsigned char dat)
{
    unsigned char i;
    if(CPOL==0&&CPHA==0)        //模式零
    {
        for(i=0;i<8;i++)
        {
            SPI_SCK_LOW();
            if(dat & 0x80)
                SPI_MOSI_HIGH();
            else
                SPI_MOSI_LOW();
            dat<<=1;
            SPI_SCK_HIGH();
        }
        SPI_SCK_LOW();
    }
    else if(CPOL==1&&CPHA==0)   //模式二
    {
        for(i=0;i<8;i++)
        {
            SPI_SCK_HIGH();
            if(dat & 0x80)
                SPI_MOSI_HIGH();
            else
                SPI_MOSI_LOW();
            dat<<=1;
            SPI_SCK_LOW();
        }
        SPI_SCK_HIGH();
    }
    else if(CPOL==0&&CPHA==1)   //模式一
    {
        SPI_SCK_LOW();
        for(i=0;i<8;i++)
        {
            SPI_SCK_HIGH();
            if(dat & 0x80)
                SPI_MOSI_HIGH();
            else
                SPI_MOSI_LOW();
            dat<<=1;
            SPI_SCK_LOW();
        }
    }
    else if(CPOL==1&&CPHA==1)   //模式三
    {
        SPI_SCK_HIGH();
        for(i=0;i<8;i++)
        {
            SPI_SCK_LOW();
            if(dat & 0x80)
                SPI_MOSI_HIGH();
            else
                SPI_MOSI_LOW();
            dat<<=1;
            SPI_SCK_HIGH();
        }       
    }
}

unsigned char soft_spi_read_byte()
{
    unsigned char i;
    unsigned char dat=0;
    if(CPOL==0&&CPHA==0)        //模式零
    {
        for(i=0;i<8;i++)
        {
            SPI_SCK_LOW();
            dat<<=1;
            if(SPI_MISO_GET())
                dat|=0x01;
            else
                dat&=0xFE;
            SPI_SCK_HIGH();
        }
        SPI_SCK_LOW();
        return dat;
    }
    else if(CPOL==1&&CPHA==0)   //模式二
    {
        for(i=0;i<8;i++)
        {
            SPI_SCK_HIGH();
            dat<<=1;
            if(SPI_MISO_GET())
                dat|=0x01;
            else
                dat&=0xFE;
            SPI_SCK_LOW();
        }
        SPI_SCK_HIGH();
        return dat;        
    }
    else if(CPOL==0&&CPHA==1)   //模式一
    {
        for(i=0;i<8;i++)
        {
            SPI_SCK_HIGH();
            dat<<=1;
            if(SPI_MISO_GET())
                dat|=0x01;
            else
                dat&=0xFE;
            SPI_SCK_LOW();
        }
        SPI_SCK_LOW();
        return dat;
    }
    else if(CPOL==1&&CPHA==1)   //模式三
    {
        for(i=0;i<8;i++)
        {
            SPI_SCK_LOW();
            dat<<=1;
            if(SPI_MISO_GET())
                dat|=0x01;
            else
                dat&=0xFE;
            SPI_SCK_HIGH();
        }
        SPI_SCK_HIGH();
        return dat;        
    }
    else
    {
        return 0;
    }
}

/* unsigned char soft_spi_rw(unsigned char dat)
{

} */
