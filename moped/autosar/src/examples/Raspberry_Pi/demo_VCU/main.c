/*
 * main.c
 *
 *  Created on: 22 sep 2014
 *      Author: sse
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Os.h"
#include "Mcu.h"
#include "arc.h"
#include "EcuM.h"

#include "bcm2835.h"
#include "Uart.h"
#include "MOPED_DEBUG.h"

#include "mcp2515.h"

#include "Can.h"
#include "Can.h"
#include "CanIf.h"
#include "Com.h"
#include "CanTp.h"

extern boolean ComWrite;

extern int runSquawk(void);

void SquawkTask(void){
	printf("Squawk task\r\n");
	for (;;) {
		WaitEvent(EVENT_MASK_SquawkEvent);
		ClearEvent(EVENT_MASK_SquawkEvent);
#if RUN_SQUAWK
		runSquawk();
#endif
		printf("Squawk task loop\r\n");
	}
}

struct ecu_config {
  char *mac;
  char *conf;
  int *ptab;			/* not used */
  int steering_min, steering_max, steering_zero;
};

struct ecu_config configs[] = {
  // VCU car 1
  {"b827eb3510df", "servo -1", NULL, -100, 100, 0},
  // VCU car 1 (RPi 3 card)
  {"b827eb31395c", "servo -1", NULL, -100, 100, 0},
  // VCU car 2
  {"b827eb3aebfd", "servo 1", NULL, -100, 100, 0},
  // VCU car 6
  {"b827eb9864ee", "servo 1", NULL, -124, 100, -29},
  // VCU car 3
  {"b827ebc93d92", "servo 1", NULL, -125, 125, 0},
  // VCU car 4
  {"b827ebcbdd68", "servo 1", NULL, -120, 120, 0},
  // VCU car 5
  {"b827eb1f2271", "servo 1", NULL, -109, 70, -16},
};

#define DEFAULT_SERVO_DIRECTION (1)

int vcu_servo_direction = DEFAULT_SERVO_DIRECTION;
int steering_min = -100, steering_max = 100, steering_zero = 0;

static void parse_config(void) {
  int i, j;
  int matched = 0;

  for (i = 0; i < sizeof(configs)/sizeof(configs[0]); i++) {
    printf("checking %s\r\n", configs[i].mac);
    if (strcmp(bcm2835_mac_address, configs[i].mac) == 0) {
      matched = 1;
      printf("config: %s\r\n", configs[i].conf);
      vcu_servo_direction = atoi(&configs[i].conf[6]);
      printf("steering: %d %d %d\r\n",
	     configs[i].steering_min,
	     configs[i].steering_max,
	     configs[i].steering_zero);
      steering_min = configs[i].steering_min;
      steering_max = configs[i].steering_max;
      steering_zero = configs[i].steering_zero;
      if (vcu_servo_direction != 1 && vcu_servo_direction != -1) {
	printf("invalid servo direction %d\r\n",
	       vcu_servo_direction);
	vcu_servo_direction = DEFAULT_SERVO_DIRECTION;
      }
      break;
    }
  }
  if (!matched) {
    printf("No configuration info was found for this ECU\r\n");
  }
}


extern uint32 act_led_gpio;

//extern int saved_r2;

extern char mem1000[];

static void show_memory(void) {
  //int b = 0x1000;
  //char *p = (char *) b;
  char *p = (char *) mem1000;
  int i, k;

#if 0
  printf("saved_r2 = %d\r\n", saved_r2);

  for (k = 0; k < 64; k++) {
    for (i = 0; i < 16; i++) {
      printf(" %d", *p);
      if (*p >= 'A' && *p <= 'z')
	printf("(%c)", *p);
      p++;
    }
    printf("\r\n");
  }
#endif
}

void StartupTask( void ) {
    pi_printf("infor: start up\r\n");

    show_memory();

    bcm2835_read_mac_address();

    parse_config();

    printf("ACT LED on GPIO %d\r\n", act_led_gpio);
    printf("CAN bus frequency %d MHz\r\n", MCP2515_FREQUENCY_MHz);

    EcuM_StartupTwo();

	// Startup CanIf due to ComM is missing in this example
	CanIf_SetControllerMode(CANIF_CanIfCtrlCfg, CANIF_CS_STARTED);
	CanIf_SetPduMode(CANIF_CanIfCtrlCfg, CANIF_SET_ONLINE);

	 /** Setup Com stack with necessary parameters**/
	Com_IpduGroupVector groupVector;
	Com_ClearIpduGroupVector(groupVector);
	//Start the IPDU group
	Com_SetIpduGroup(groupVector, COM_PDU_GROUP_ID_TXIPDUGROUP, TRUE);
	Com_SetIpduGroup(groupVector, COM_PDU_GROUP_ID_RXIPDUGROUP, TRUE);
	Com_IpduGroupControl(groupVector, TRUE);

	TerminateTask();

}

void CanFunctionTask(void) {

    pi_printf("CanFunctionTask\r\n");

	for(;;){

		WaitEvent(EVENT_MASK_CanFunctionEvent);
		ClearEvent(EVENT_MASK_CanFunctionEvent);
#if !CAN_INTERRUPT
		Can_MainFunction_Read();
#endif
		Com_MainFunctionRx();

		if (ComWrite == true) {
		  Com_MainFunctionTx();
		  ComWrite = false;
		}
	}
}

void OsIdle( void ) {
	for(;;) {}
}
