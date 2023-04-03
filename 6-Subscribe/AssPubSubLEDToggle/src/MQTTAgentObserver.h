/*
 * MQTTAgentObserver.h
 * Observer abstract class to pick up state information on MQTT Connection
 *
 *  Created on: 23 Nov 2021
 *      Author: jondurrant
 */

#ifndef SRC_MQTTAGENTOBSERVER_H_
#define SRC_MQTTAGENTOBSERVER_H_

class MQTTAgentObserver {
public:
	MQTTAgentObserver();
	virtual ~MQTTAgentObserver();

	virtual void MQTTOffline();

	virtual void MQTTOnline();

	virtual void MQTTSend();

	virtual void MQTTRecv();
};

#endif /* SRC_MQTTAGENTOBSERVER_H_ */
