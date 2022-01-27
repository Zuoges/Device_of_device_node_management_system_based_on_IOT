#ifndef _MY_UART_H_
#define _MY_UART_H_

#define ECHO_TXD1  GPIO_NUM_13
#define ECHO_RXD1  GPIO_NUM_14
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define UART1_BUF_SIZE 1024

//extern char uart1_receive_data[50];

void uart1_init();
/* void uart1_rec();
void my_uart1_init(); */

#endif