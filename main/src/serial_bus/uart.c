#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "string.h"
#include "uart.h"

//char uart1_receive_data[50]="";

TaskHandle_t uart1_task_handler;

void uart1_init()
{
    uart_config_t uart_config = {
        .baud_rate = 4800,                      //���ò�����
        .data_bits = UART_DATA_8_BITS,          //���ݳ���
        .parity    = UART_PARITY_DISABLE,//UART_PARITY_EVEN,          //��żУ��
        .stop_bits = UART_STOP_BITS_1,          //ֹͣλ
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE  
    };
    uart_param_config(UART_NUM_1, &uart_config);    //Ӧ������
    uart_set_pin(UART_NUM_1, ECHO_TXD1, ECHO_RXD1, ECHO_TEST_RTS, ECHO_TEST_CTS);   //����UART����
    //��װUART����
    uart_driver_install(UART_NUM_1,             //UART���              
                        UART1_BUF_SIZE * 2,     //UART RX��������С
                        0,                      //UART TX��������С,�������Ϊ0���������򽫲���ʹ��TX��������TX��������������ֱ���������ݶ������ͳ�ȥ��
                        0,                      //UART�¼����д�С/���
                        NULL,                   //UART�¼����о��(out����)������ɹ������������дһ���µĶ��о�������ṩ��UART�¼��ķ��ʡ��������ΪNULL���������򽫲�ʹ���¼����С�
                        0);                     //���ڷ����жϵı�־
}

/* void uart1_rec()
{
    unsigned char *data = (unsigned char*)malloc(UART1_BUF_SIZE+1);     //Ϊ��ȡ�����ݷ���ռ�
    while(1)
    {
        int len = uart_read_bytes(UART_NUM_1 , data , UART1_BUF_SIZE , 10 / portTICK_RATE_MS);
        //sprintf(uart1_receive_data,"%s",data);
        uart_write_bytes(UART_NUM_1, (const char *) data, len);      
    }
}

void my_uart1_init()
{
    uart1_init();
    xTaskCreate(uart1_rec, "uart1_rec_task", 2048, NULL, 10, uart1_task_handler);
} */