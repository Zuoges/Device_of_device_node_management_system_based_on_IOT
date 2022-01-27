#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "uart.h"
#include "Euclidean.h"
#include "im1281b.h"

unsigned char  Tx_Buffer[8]; 
unsigned char  Rx_Buffer[40];
unsigned char  read_enable = 1 , receive_finished = 0 , reveive_number; 
unsigned long  Voltage_data,Current_data,Power_data,Energy_data,Pf_data,CO2_data; 
unsigned char Read_ID = 0x01;

im1281b_data_t *im1281b_data;

void im1281b_send_data(unsigned char num)
{
    uart_write_bytes(UART_NUM_1 , Tx_Buffer , num);
}

void im1281b_rec_data()
{
    int len = uart_read_bytes(UART_NUM_1 , Rx_Buffer , 40 , 10 / portTICK_RATE_MS);
    //printf("len:%d\n" , len);
}

unsigned int im1281b_calc_crc(unsigned char crcbuf,unsigned int crc)
{
    unsigned char i;  
    unsigned char chk; 
    crc=crc ^ crcbuf;  
    for(i=0;i<8;i++)  
    {   
        chk=( unsigned char)(crc&1); 
        crc=crc>>1;  
        crc=crc&0x7fff;   
        if (chk==1) 
            crc=crc^0xa001;   
        crc=crc&0xffff;   
    } 
    return crc;
}

unsigned int im1281b_chk_crc(unsigned char *buf,unsigned char len)
{
    unsigned char hi,lo; 
    unsigned int i;   
    unsigned int crc;   
    crc=0xFFFF;  
    for(i=0;i<len;i++)  
    {  
        crc=im1281b_calc_crc(*buf,crc); 
        buf++;  
    }
    hi=( unsigned char)(crc%256);  
    lo=( unsigned char)(crc/256); 
    crc=(((unsigned int)(hi))<<8)|lo; 
    return crc;
}

void im1281b_read_data(void) 
{ 
    union  crcdata 
    { 
        unsigned int word16; 
        unsigned char   byte[2]; 
    }crcnow; 
    if(read_enable==1)    // 到时间抄读模块，抄读间隔1秒钟(或其他) 
    { 
        read_enable=0; 
        Tx_Buffer[0]=Read_ID;    //模块的ID号，默认ID为0x01 
        Tx_Buffer[1]=0x03; 
        Tx_Buffer[2]=0x00; 
        Tx_Buffer[3]=0x48; 
        Tx_Buffer[4]=0x00; 
        Tx_Buffer[5]=0x06; 
        crcnow.word16=im1281b_chk_crc(Tx_Buffer,6); 
        Tx_Buffer[6]=crcnow.byte[1];     //CRC效验低字节在前 
        Tx_Buffer[7]=crcnow.byte[0]; 
        im1281b_send_data(8);         //发送8个数据，请根据单片机类型自己编程 
    } 
} 

void im1281b_analysis_data(void) 
{ 
    unsigned char i; 
    union  crcdata 
    { 
        unsigned int word16; 
        unsigned char   byte[2]; 
    }crcnow; 
    if(receive_finished==1)   //接收完成 
    { 
        receive_finished=0; 
        // for(i = 0 ; i < 24 ; i++)
        // {
        //     printf("%c" , Rx_Buffer[i]);
        // }
        if(Rx_Buffer[0]==Read_ID)   //确认ID正确 
        { 
            crcnow.word16=im1281b_chk_crc(Rx_Buffer,reveive_number-2);   //reveive_numbe是接收数据总长度 
            //if((crcnow.byte[0]==Rx_Buffer[reveive_number-1])&&(crcnow.byte[1]==Rx_Buffer[reveive_number-2]))  //确认CRC校验正确 
            //{ 
                //printf("crc ok\n");
                Voltage_data=(((unsigned long)(Rx_Buffer[3]))<<24)|(((unsigned long)(Rx_Buffer[4]))<<16)|(((unsigned long)(Rx_Buffer[5]))<<8)|Rx_Buffer[6]; 
                Current_data=(((unsigned long)(Rx_Buffer[7]))<<24)|(((unsigned long)(Rx_Buffer[8]))<<16)|(((unsigned long)(Rx_Buffer[9]))<<8)|Rx_Buffer[10]; 
                Power_data=(((unsigned long)(Rx_Buffer[11]))<<24)|(((unsigned long)(Rx_Buffer[12]))<<16)|(((unsigned long)(Rx_Buffer[13]))<<8)|Rx_Buffer[14]; 
                Energy_data=(((unsigned long)(Rx_Buffer[15]))<<24)|(((unsigned  long)(Rx_Buffer[16]))<<16)|(((unsigned long)(Rx_Buffer[17]))<<8)|Rx_Buffer[18];  
                Pf_data=(((unsigned long)(Rx_Buffer[19]))<<24)|(((unsigned long)(Rx_Buffer[20]))<<16)|(((unsigned long)(Rx_Buffer[21]))<<8)|Rx_Buffer[22];  
                CO2_data=(((unsigned long)(Rx_Buffer[23]))<<24)|(((unsigned long)(Rx_Buffer[24]))<<16)|(((unsigned long)(Rx_Buffer[25]))<<8)|Rx_Buffer[26];  
            //} 
            //printf("%ld\n" , Voltage_data);
        } 
    } 
}

void im1281b_parse_data()
{
    im1281b_data->voltage = Voltage_data * 0.0001;
    im1281b_data->current = Current_data * 0.0001;
    im1281b_data->power = Power_data * 0.0001;
    im1281b_data->electricity = Energy_data * 0.0001;
    im1281b_data->power_factor = Pf_data * 0.001;
    im1281b_data->co2 = CO2_data * 0.0001;
}

void im1281b_task()
{
    while(1)
    {
        read_enable = 1;
        im1281b_read_data(); 
        vTaskDelay(10 / portTICK_PERIOD_MS);
        im1281b_rec_data();
        receive_finished = 1;
        im1281b_analysis_data(); 
        im1281b_parse_data();
        // vTaskDelay(100 / portTICK_PERIOD_MS);
        // printf("V:%f\r\n A:%f\r\n P:%f\r\n" , im1281b_data->voltage ,im1281b_data->current ,im1281b_data->power);
        // status_parse(im1281b_data);
    }
}


void im1281b_init()
{
    //分配空间
    im1281b_data = (im1281b_data_t*)malloc(sizeof(im1281b_data_t));

    uart1_init();
    xTaskCreate(im1281b_task, "im1281b_task", 1024*2, NULL, 10, NULL);
    printf("im1281b task init finished\n");
}