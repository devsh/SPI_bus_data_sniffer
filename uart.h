/*
 * uart.h
 *
 *  Created on: 06.01.2019
 *  Author: Denis Shashunkin
 */


#ifndef UART_H_
#define UART_H_


int init_uarts();
void SendByte( unsigned char byte);
void SendBuffer( unsigned char* buff, int len );
void SendDbgMsg( char* str );


#endif /* UART_H_ */
