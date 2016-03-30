#ifndef UART_H
#define UART_H

/*-Include----------------*/
#include<ioCC2530.h>
#include "stdio.h"



/*-function---------------*/
void Uart0_Init(void);
void Uart0_SendByte(uint8_t Data);
void Uart0_SendString(uint8_t *Data, uint8_t len);


#endif