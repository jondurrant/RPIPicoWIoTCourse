/*
 * Transport.cpp
 *
 *  Created on: 19 Mar 2023
 *      Author: jondurrant
 */

#include "Transport.h"
#include <stdlib.h>
#include "pico/stdlib.h"
#include <errno.h>

#include <stdio.h>
#define DEBUG_LINE 25

Transport::Transport() {
}

Transport::~Transport() {
}



/***
 * Required by CoreMQTT returns time in ms
 * @return
 */
uint32_t Transport::getCurrentTime(){
	return to_ms_since_boot(get_absolute_time ());
}


/***
 * Static function to send data through socket from buffer
 * @param pNetworkContext - Used to locate the TCPTransport object to use
 * @param pBuffer - Buffer of data to send
 * @param bytesToSend - number of bytes to send
 * @return number of bytes sent
 */
int32_t Transport::staticSend(NetworkContext_t * pNetworkContext, const void * pBuffer, size_t bytesToSend){
	Transport *t = (Transport *)pNetworkContext->tcpTransport;
	return t->transSend(pNetworkContext, pBuffer, bytesToSend);
}


/***
 * Read data from network socket. Non blocking returns 0 if no data
 * @param pNetworkContext - Used to locate the TCPTransport object to use
 * @param pBuffer - Buffer to read into
 * @param bytesToRecv - Maximum number of bytes to read
 * @return number of bytes read. May be 0 as non blocking
 * Negative number indicates error
 */
int32_t Transport::staticRead(NetworkContext_t * pNetworkContext, void * pBuffer, size_t bytesToRecv){
	Transport *t = (Transport *)pNetworkContext->tcpTransport;
	int32_t res = t->transRead(pNetworkContext, pBuffer, bytesToRecv);
	return res;
}

/***
 * Print the buffer in hex and plain text for debugging
 */
void Transport::debugPrintBuffer(const char *title, const void * pBuffer, size_t bytes){
	size_t count =0;
	size_t lineEnd=0;
	const uint8_t *pBuf = (uint8_t *)pBuffer;

	printf("DEBUG: %s of size %d\n", title, bytes);

	while (count < bytes){
		lineEnd = count + DEBUG_LINE;
		if (lineEnd > bytes){
			lineEnd = bytes;
		}

		//Print HEX DUMP
		for (size_t i=count; i < lineEnd; i++){
			if (pBuf[i] <= 0x0F){
				printf("0%X ", pBuf[i]);
			} else {
				printf("%X ", pBuf[i]);
			}
		}

		//Pad for short lines
		size_t pad = (DEBUG_LINE - (lineEnd - count)) * 3;
		for (size_t i=0; i < pad; i++){
			printf(" ");
		}

		//Print Plain Text
		for (size_t i=count; i < lineEnd; i++){
			if ((pBuf[i] >= 0x20) && (pBuf[i] <= 0x7e)){
				printf("%c", pBuf[i]);
			} else {
				printf(".");
			}
		}

		printf("\n");

		count = lineEnd;

	}
}

