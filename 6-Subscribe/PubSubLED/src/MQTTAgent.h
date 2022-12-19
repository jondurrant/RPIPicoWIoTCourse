/*
 * MQTTAgent.h
 *
 * MQTT Agent to manage MQTT connection and messaging
 *
 *  Created on: 15 Nov 2021
 *      Author: jondurrant
 */

#ifndef MQTTAGENT_H_
#define MQTTAGENT_H_

#include "MQTTConfig.h"
#include "FreeRTOS.h"
#include "core_mqtt.h"
#include "core_mqtt_agent.h"
#include "MQTTTopicHelper.h"
#include "TCPTransport.h"
#include "MQTTAgentObserver.h"
#include "MQTTInterface.h"
#include "MQTTRouter.h"

extern "C" {
#include "freertos_agent_message.h"
#include "freertos_command_pool.h"
}

#ifndef MQTT_AGENT_NETWORK_BUFFER_SIZE
#define MQTT_AGENT_NETWORK_BUFFER_SIZE 512
#endif

#ifndef MAXSUBS
#define MAXSUBS 12
#endif

#ifndef MQTTKEEPALIVETIME
#define MQTTKEEPALIVETIME 10
#endif

#ifndef MQTT_RECON_DELAY
#define MQTT_RECON_DELAY 10
#endif


// Enumerator used to control the state machine at centre of agent
enum MQTTState {  Offline, TCPReq, TCPConned, MQTTReq, MQTTConned, MQTTRecon, Online};

class MQTTAgent: public MQTTInterface{
public:
	/***
	 * Constructor
	 */
	MQTTAgent();

	/***
	 * Distructor
	 */
	virtual ~MQTTAgent();

	/***
	 * Set credentials
	 * @param user - string pointer. Not copied so pointer must remain valid
	 * @param passwd - string pointer. Not copied so pointer must remain valid
	 * @param id - string pointer. Not copied so pointer must remain valid. I
	 * f not provide ID will be user
	 * @return lwespOK if succeeds
	 */
	void credentials(const char * user, const char * passwd, const char * id = NULL );

	/***
	 * Connect to mqtt server - Actual connection is done in the state machine so task must be running
	 * @param target - hostname or ip address, Not copied so pointer must remain valid
	 * @param port - port number
	 * @param ssl - unused
	 * @return
	 */
	 bool mqttConnect(const char * target, uint16_t  port, bool recon=false);

	/***
	 * Start the task running
	 * @param priority - priority to run within FreeRTOS
	 */
	void start(UBaseType_t priority = tskIDLE_PRIORITY);

	/***
	 * Stop task
	 * @return
	 */
	void stop();

	/***
	 * Returns the id of the MQTT client
	 * @return
	 */
	virtual const char * getId();

	/***
	 * Close connection
	 */
	virtual void close();

	/***
	 * Set a single observer to get call back on state changes
	 * @param obs
	 */
	virtual void setObserver(MQTTAgentObserver *obs);


	/***
	 * Get the FreeRTOS task being used
	 * @return
	 */
	virtual TaskHandle_t getTask();

	/***
	 * Get high water for stack
	 * @return close to zero means overflow risk
	 */
	virtual unsigned int getStakHighWater();

	/***
	 * Publish message to topic
	 * @param topic - zero terminated string. Copied by function
	 * @param payload - payload as pointer to memory block
	 * @param payloadLen - length of memory block
	 * @param QoS - quality of service - 0, 1 or 2
	 * @param retain - ask broker to retain message
	 */
	virtual bool pubToTopic(const char * topic,  const void * payload,
			size_t payloadLen, const uint8_t QoS=0, bool retain = false);

	/***
	 * Subscribe to a topic, mesg will be sent to router object
	 * @param topic
	 * @param QoS
	 * @return
	 */
	virtual bool subToTopic(const char * topic, const uint8_t QoS=0);

	/***
	 * Get the router object handling all received messages
	 * @return
	 */
	MQTTRouter* getRouter() ;


	/***
	 * Set the router object
	 * Router objects handle any message arriving
	 * @param pRouter
	 */
	void setRouter( MQTTRouter *pRouter = NULL);

	/***
	 * Route a message to the router object
	 * @param topic - non zero terminated string
	 * @param topicLen - topic length
	 * @param payload - raw memory
	 * @param payloadLen - payload length
	 */
	virtual void route(const char * topic, size_t topicLen, const void * payload, size_t payloadLen);

private:
	/***
	 * Initialisation code
	 * @return
	 */
	MQTTStatus_t init();

	/***
	 * Task object running to manage MQTT interface
	 * @param pvParameters
	 */
	static void vTask( void * pvParameters );

	/***
	 * Call back function on connect
	 * @param pCmdCallbackContext
	 * @param pReturnInfo
	 */
	static void connectCmdCallback( MQTTAgentCommandContext_t * pCmdCallbackContext,
            MQTTAgentReturnInfo_t * pReturnInfo );

	/***
	 * Callback on when new data is received
	 * @param pMqttAgentContext
	 * @param packetId
	 * @param pxPublishInfo
	 */
	static void incomingPublishCallback( MQTTAgentContext_t * pMqttAgentContext,
	                                        uint16_t packetId,
	                                        MQTTPublishInfo_t * pxPublishInfo );

	/***
	 * Call back function when Publish completes
	 * @param pCmdCallbackContext
	 * @param pReturnInfo
	 */
	static void publishCmdCompleteCb( MQTTAgentCommandContext_t * pCmdCallbackContext,
            MQTTAgentReturnInfo_t * pReturnInfo );

	/***
	 * Call back function nwhen subscribe completes
	 * @param pCmdCallbackContext
	 * @param pReturnInfo
	 */
	static void subscribeCmdCompleteCb( MQTTAgentCommandContext_t * pCmdCallbackContext,
		                             MQTTAgentReturnInfo_t * pReturnInfo );


	/***
	 * Run loop for the task
	 */
	void run();

	/***
	 * Connect to MQTT hub
	 * @return
	 */
	MQTTStatus_t MQTTconn();

	/***
	 * Perform TCP Connection
	 * @return true if succeeds
	 */
	bool TCPconn();

	/***
	 * Set the connection state variable
	 * @param s
	 */
	void setConnState(MQTTState s);


	NetworkContext_t xNetworkContext;
	TCPTransport xTcpTrans;

	//MQTT Server and credentials
	const char * pUser;
	const char * pPasswd;
	const char * pId;
	const char * pTarget = NULL;
	char xMacStr[14];
	uint16_t xPort = 1883 ;
	bool xRecon = false;

	//MQTT Will object
	char *pWillTopic = NULL;
	static const char * WILLPAYLOAD;
	MQTTPublishInfo_t xWillInfo;

	//Topics and payload for connection
	static const char * ONLINEPAYLOAD;
	char *pOnlineTopic = NULL;

	//Router object to handle all sub messages
	MQTTRouter * pRouter = NULL;

	// Buffers and queues
	uint8_t xNetworkBuffer[ MQTT_AGENT_NETWORK_BUFFER_SIZE ];
	uint8_t xStaticQueueStorageArea[ MQTT_AGENT_COMMAND_QUEUE_LENGTH * sizeof( MQTTAgentCommand_t * ) ];
	StaticQueue_t xStaticQueueStructure;
	MQTTAgentMessageContext_t xCommandQueue;
	MQTTAgentContext_t xGlobalMqttAgentContext;
	TaskHandle_t xHandle = NULL;

	//State machine state
	MQTTState xConnState = Offline;


	//Single Observer
	MQTTAgentObserver *pObserver = NULL;

};

#endif /* MQTTAGENT_H_ */
