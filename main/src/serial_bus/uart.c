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
        .baud_rate = 4800,                      //设置波特率
        .data_bits = UART_DATA_8_BITS,          //数据长度
        .parity    = UART_PARITY_DISABLE,//UART_PARITY_EVEN,          //奇偶校验
        .stop_bits = UART_STOP_BITS_1,          //停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE  
    };
    uart_param_config(UART_NUM_1, &uart_config);    //应用配置
    uart_set_pin(UART_NUM_1, ECHO_TXD1, ECHO_RXD1, ECHO_TEST_RTS, ECHO_TEST_CTS);   //设置UART引脚
    //安装UART驱动
    uart_driver_install(UART_NUM_1,             //UART编号              
                        UART1_BUF_SIZE * 2,     //UART RX缓冲区大小
                        0,                      //UART TX缓冲区大小,如果设置为0，驱动程序将不会使用TX缓冲区，TX函数将阻塞任务，直到所有数据都被发送出去。
                        0,                      //UART事件队列大小/深度
                        NULL,                   //UART事件队列句柄(out参数)，如果成功，将在这里编写一个新的队列句柄，以提供对UART事件的访问。如果设置为NULL，驱动程序将不使用事件队列。
                        0);                     //用于分配中断的标志
}

/* void uart1_rec()
{
    unsigned char *data = (unsigned char*)malloc(UART1_BUF_SIZE+1);     //为读取的数据分配空间
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