/*
 * TestTrans.cpp
 *
 *  Created on: 4 Sept 2022
 *      Author: jondurrant
 */

#include "TestTrans.h"
#include "WifiHelper.h"
#include "TCPTransport.h"

TestTrans::TestTrans() {
	// TODO Auto-generated constructor stub

}

TestTrans::~TestTrans() {
	// TODO Auto-generated destructor stub
}

void TestTrans::run(){

	test();

	printf("RUN %d TESTS, SUCESSFUL %d\n", xTests, xSuccessful);

	while (true) { // Loop forever
		vTaskDelay(5000);
	}

}


/***
* Get the static depth required in words
* @return - words
*/
configSTACK_DEPTH_TYPE TestTrans::getMaxStackSize(){
	return 5000;
}

void TestTrans::test(){

	xTests++;
	if (!testConnect()){
		printf("CONNECTIONED FAILED\n");
	} else {
		xSuccessful++;
	}

	xTests++;
	if (!testTrans()){
		printf("Trans FAILED\n");
	} else {
		xSuccessful++;
	}


}


bool TestTrans::testConnect(){
	if (WifiHelper::isJoined()) {
		return true;
	}
	if (WifiHelper::join(WIFI_SSID, WIFI_SSID)){
		if (WifiHelper::isJoined()) {
			return true;
		} else {
			printf("is Joined is false\n\r");
		}
	} else {
		printf("Connect failed\n\r");
	}

	return false;
}

bool TestTrans::testTrans(){
	uint16_t targetPort = 80;
	char targetHost[]="drjonea.co.uk";
	char message[]="GET / HTTP/1.1\r\n"
					"Host: drjonea.co.uk\r\n"
					"Connection: close\r\n"
					"\r\n";
	NetworkContext_t net;
	char buf[1024];
	int32_t retVal;

	TCPTransport sockTrans;

	if (!sockTrans.transConnect(targetHost, targetPort)){
		printf("Socket Connect Failed\r\n");
		return false;
	}

	//Check no data and nonblocking
	retVal = sockTrans.transRead(&net, buf, sizeof(buf));
	if (retVal != 0){
		printf("Read data initially\n\r");
		return false;
	}

	retVal = sockTrans.transSend(&net, message, strlen(message));
	if (retVal != strlen(message)){
		printf("Socket Send failed\n\r");
		return false;
	}

	retVal = 1;

	while (retVal >= 0){
		retVal = sockTrans.transRead(&net, buf, sizeof(buf));
		if (retVal > 0){
			sockTrans.debugPrintBuffer("READ:", buf, retVal);
		}
	}

	sockTrans.transClose();



	return true;
}

