/***
 * main.cpp - LED Control over MQTT
 * Jon Durrant
 * 4-Oct-2022
 *
 * Control an LED over MQTT using Topic
 * TNG/<<device>>/TPC/LED
 * with JSON payload as boolean state
 * e.g. {"on": True}
 *
 */

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>


#include "MQTTAgent.h"
#include <WifiHelper.h>
#include "BlinkAgent.h"
#include "MQTTRouterLED.h"

//Check these definitions where added from the makefile
#ifndef MQTT_CLIENT
#error "MQTT_CLIENT not defined"
#endif
#ifndef MQTT_USER
#error "MQTT_PASSWD not defined"
#endif
#ifndef MQTT_PASSWD
#error "MQTT_PASSWD not defined"
#endif
#ifndef MQTT_HOST
#error "MQTT_HOST not defined"
#endif
#ifndef MQTT_PORT
#error "MQTT_PORT not defined"
#endif
#ifndef WIFI_SSID
#error "WIFI_SSID not defined"
#endif
#ifndef WIFI_PASSWORD
#error "WIFI_PASSWORD not defined"
#endif

// LED PAD defintions
#define PULSE_LED  	   0
#define CONTROL_LED	  15


#define TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )


void runTimeStats(   ){
	//char pcWriteBuffer[120] = "";
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime;

   /* Make sure the write buffer does not contain a string. */
   //*pcWriteBuffer = 0x00;

   /* Take a snapshot of the number of tasks in case it changes while this
   function is executing. */
   uxArraySize = uxTaskGetNumberOfTasks();
   printf("Number of tasks %d\n", uxArraySize);

   /* Allocate a TaskStatus_t structure for each task.  An array could be
   allocated statically at compile time. */
   pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL ){
      /* Generate raw status information about each task. */
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                 uxArraySize,
                                 &ulTotalRunTime );



	 /* For each populated position in the pxTaskStatusArray array,
	 format the raw data as human readable ASCII data. */
	 for( x = 0; x < uxArraySize; x++ )
	 {
		 printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
				pxTaskStatusArray[ x ].xTaskNumber ,
				pxTaskStatusArray[ x ].uxCurrentPriority ,
				pxTaskStatusArray[ x ].uxBasePriority ,
				pxTaskStatusArray[ x ].usStackHighWaterMark ,
				pxTaskStatusArray[ x ].pcTaskName
				);
	 }


      /* The array is no longer needed, free the memory it consumes. */
      vPortFree( pxTaskStatusArray );
   } else {
	   printf("Failed to allocate space for stats\n");
   }

   HeapStats_t heapStats;
   vPortGetHeapStats(&heapStats);
   printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
		   heapStats.xAvailableHeapSpaceInBytes,
		   heapStats.xNumberOfFreeBlocks,
		   heapStats.xNumberOfSuccessfulAllocations,
		   heapStats.xNumberOfSuccessfulFrees
		   );

}


void main_task(void *params){

	printf("Main task started\n");

	//Start up pulse blink LED
	BlinkAgent blink(PULSE_LED);
	blink.start("Blink", TASK_PRIORITY);


	// Init Wifi Adapter
	int res = cyw43_arch_init();
	if (res) {
		printf("cyw42 init %d\n", res);
		printf("failed to initialise\n");
		return;
	}

	cyw43_wifi_pm(&cyw43_state ,CYW43_AGGRESSIVE_PM);

	cyw43_arch_enable_sta_mode();
	printf("Connecting to WiFi... %s \n", WIFI_SSID);

	//Loop trying to connect to Wifi
	int r =-1;
	while (r < 0){
		r = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 60000);
		printf("Connect r %d\n", r);
		if (r){
			printf("Failed to join AP.\n");
			 vTaskDelay(2000);
		}
	}

	//Print IP Address
	char ipStr[20];
	WifiHelper::getIPAddressStr(ipStr);
	printf("IP ADDRESS: %s\n", ipStr);


	// Setup for MQTT Connection
	char mqttTarget[] = MQTT_HOST;
	int mqttPort = MQTT_PORT;
	char mqttClient[] = MQTT_CLIENT;
	char mqttUser[] = MQTT_USER;
	char mqttPwd[] = MQTT_PASSWD;
	MQTTRouterLED mqttRouter(mqttClient, CONTROL_LED);
	MQTTAgent mqttAgent;


	mqttAgent.credentials(mqttUser, mqttPwd, mqttClient);
	mqttAgent.setRouter(&mqttRouter);

	printf("Connecting to: %s(%d)\n", mqttTarget, mqttPort);
	printf("Client id: %.4s...\n", mqttAgent.getId());
	printf("User id: %.4s...\n", mqttUser);

	mqttAgent.mqttConnect(mqttTarget, mqttPort, true);
	mqttAgent.start(tskIDLE_PRIORITY+1);



    while(true) {

    	runTimeStats();

        vTaskDelay(3000);



        if (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA) < 0){
        	printf("AP Link is down\n");
        	while (r < 0){
				r = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 60000);
				printf("Connect r %d\n", r);
				if (r){
					printf("Failed to join AP.\n");
					 vTaskDelay(2000);
				}
			}
        }



    }

    cyw43_arch_deinit();
}





void vLaunch( void) {
    TaskHandle_t task;

    xTaskCreate(main_task, "MainThread", 2048, NULL, TASK_PRIORITY, &task);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}


int main( void )
{
    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    /* Configure the hardware ready to run the demo. */
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();

    return 0;
}
