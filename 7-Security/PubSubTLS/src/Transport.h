/*
 * Transport.h
 *
 *  Created on: 19 Mar 2023
 *      Author: jondurrant
 */

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include "MQTTConfig.h"
#include "core_mqtt.h"

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "lwip/ip4_addr.h"
#include "lwip/ip_addr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"

}

class Transport {
public:
	Transport();
	virtual ~Transport();

	/***
	 * Connect to remote TCP Socket
	 * @param host - Host address
	 * @param port - Port number
	 * @return true on success
	 */
	virtual bool transConnect(const char * host, uint16_t port)=0;

	/***
	 * Get status of the socket
	 * @return int <0 is error
	 */
	virtual int status() = 0;

	/***
	 * Close the socket
	 * @return true on success
	 */
	virtual bool transClose() = 0;


	/***
	 * Send bytes through socket
	 * @param pNetworkContext - Network context object from MQTT
	 * @param pBuffer - Buffer to send from
	 * @param bytesToSend - number of bytes to send
	 * @return number of bytes sent
	 */
	virtual int32_t transSend(NetworkContext_t * pNetworkContext, const void * pBuffer, size_t bytesToSend)=0;

	/***
	 * Send
	 * @param pNetworkContext
	 * @param pBuffer
	 * @param bytesToRecv
	 * @return
	 */
	virtual int32_t transRead(NetworkContext_t * pNetworkContext, void * pBuffer, size_t bytesToRecv) = 0;


	/***
	 * returns current time, as time in ms since boot
	 * Required for MQTT Agent library
	 * @return
	 */
	static uint32_t getCurrentTime();

	/***
	 * Static function to send data through socket from buffer
	 * @param pNetworkContext - Used to locate the TCPTransport object to use
	 * @param pBuffer - Buffer of data to send
	 * @param bytesToSend - number of bytes to send
	 * @return number of bytes sent
	 */
	static int32_t staticSend(NetworkContext_t * pNetworkContext, const void * pBuffer, size_t bytesToSend);

	/***
	 * Read data from network socket. Non blocking returns 0 if no data
	 * @param pNetworkContext - Used to locate the TCPTransport object to use
	 * @param pBuffer - Buffer to read into
	 * @param bytesToRecv - Maximum number of bytes to read
	 * @return number of bytes read. May be 0 as non blocking
	 * Negative number indicates error
	 */
	static int32_t staticRead(NetworkContext_t * pNetworkContext, void * pBuffer, size_t bytesToRecv);


	void debugPrintBuffer(const char *title, const void * pBuffer, size_t bytes);

};

#endif /* _TRANSPORT_H_ */
