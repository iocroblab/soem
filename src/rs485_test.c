/* * Usage : rs485_test [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 * (c)Arthur Ketels 2008/Gerard Harkema
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"

char IOmap[4096];
uint8 *Ch2InputData;
uint8 *Ch2OutputData;
boolean TxReady[2] = {0};
boolean RxSignal[2] = {0};


#define CHANNEL_1	0
#define CHANNEL_2	1

#define CONTROL_INDEX		0
#define STATUS_INDEX		0
#define LENGTH_INDEX		1

#define MAX_CHANNEL			1

typedef struct{
	uint8 *InputData;
	uint8 *OutputData;
	boolean TxReady;
	boolean RxSignal;
	uint8 ReqState;
	uint8 State;
	uint8 Istate;
	uint8 Tries;
}RS_485_COM_CONTROL;

RS_485_COM_CONTROL Rs485Control[2] = {{0},{0}};

#define RS485_ID		0x17863052
void rs485_test(char *ifname)
{
	int i, j, k, slave_op_cnt;

	uint16 RS485Index = 0;

	uint16 transmit_accepted;
	uint16 receive_request;
	uint16 init_accepted;
	uint16 buffer_full;
	uint16 parity_error;
	uint16 framing_error;
	uint16 overrun_error;
	uint16 input_length;
	uint8 baudrate;
	uint8 state = 0;
	
	printf("Starting simple rs485 test\n");
	
	/* initialise SOEM, bind socket to ifname */
	if (ec_init(ifname))
	{	
		printf("ec_init on %s succeeded.\n",ifname);
		/* find and auto-config slaves */
		if ( ec_config(FALSE, &IOmap) > 0 )
		{
			printf("%d slaves found and configured.\n",ec_slavecount);
			/* wait for all slaves to reach SAFE_OP state */
			ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);

			for(k = 1; k <= ec_slavecount; k++){
				printf("Slave %i\n", k);
				printf("  Name = %s\n", ec_slave[k].name);
				printf("  eep_id = 0x%04x\n", ec_slave[k].eep_id);
				printf("  ec_slave[%i].Obytes = %i\n", k, ec_slave[k].Obytes);
				printf("  ec_slave[%i].Ibytes = %i\n", k, ec_slave[k].Ibytes);
				if(ec_slave[k].eep_id == RS485_ID){
					printf("  rs485 Rs485Controller found !!!\n");
					RS485Index = k;
				}
			}
			if(RS485Index){
			    ec_slave[0].state = EC_STATE_PRE_OP;
				/* request OP state for all slaves */
				ec_writestate(0);
				/* wait for all slaves to reach OP state */
				ec_statecheck(0, EC_STATE_PRE_OP,  EC_TIMEOUTSTATE);
				baudrate = 6; // set baudratRTS_CTS 9600 Baude to
				ec_SDOwrite(RS485Index, 0x8000, 0x11, FALSE, sizeof(baudrate), &baudrate, EC_TIMEOUTRXM);
				ec_SDOwrite(RS485Index, 0x8010, 0x11, FALSE, sizeof(baudrate), &baudrate, EC_TIMEOUTRXM);
			    ec_slave[0].state = EC_STATE_SAFE_OP;
				/* request OP state for all slaves */
				ec_writestate(0);
				/* wait for all slaves to reach OP state */
				ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);
				


				Rs485Control[CHANNEL_1].OutputData = ec_slave[RS485Index].outputs;
				Rs485Control[CHANNEL_1].InputData = ec_slave[RS485Index].inputs;
				Rs485Control[CHANNEL_2].OutputData = ec_slave[RS485Index].outputs + (ec_slave[RS485Index].Obytes / 2);
				Rs485Control[CHANNEL_2].InputData = ec_slave[RS485Index].inputs + (ec_slave[RS485Index].Ibytes / 2);

				printf("Request operational state for all slaves\n");
			    ec_slave[0].state = EC_STATE_OPERATIONAL;
				/* request OP state for all slaves */
				ec_writestate(0);
				/* wait for all slaves to reach OP state */
				ec_statecheck(0, EC_STATE_OPERATIONAL,  EC_TIMEOUTSTATE);
				slave_op_cnt = 0;
				ec_readstate();
				for(k = 1; k <= ec_slavecount; k++){
					if (ec_slave[k].state == EC_STATE_OPERATIONAL ){
						printf("Operational state reached for slaves %i.\n", k);
						slave_op_cnt++;
					}
				}
				for(j = 0; j < 24; j++){
					Rs485Control[CHANNEL_1].OutputData[j] = 0x00; // data channel 1
					Rs485Control[CHANNEL_1].OutputData[j] = 0x00; // data channel 2
				}
				if (slave_op_cnt)
	//			if (ec_slave[STATUS_INDEX].state == EC_STATE_OPERATIONAL )
				{
	//				printf("Operational state reached for all slaves.\n");
					/* cyclic loop 10 times */
					for(i = 1; i <= 30; i++)
					{
						printf("### Processdata cycle %4d ###\n", i);

						if(i == 2){
							for(k = 0; k < MAX_CHANNEL; k++){
								// Goto init
							//	Rs485Control[k].ReqState = 1;
							//	Rs485Control[k].ReqState = 1;
							}
						}


	
						ec_send_processdata();
						ec_receive_processdata(EC_TIMEOUTRET);
	
						for(k = 0; k < MAX_CHANNEL; k++){

						Rs485Control[k].TxReady = (Rs485Control[k].InputData[STATUS_INDEX] & 0x01) == (Rs485Control[k].OutputData[CONTROL_INDEX] & 0x01);
						Rs485Control[k].RxSignal = (Rs485Control[k].InputData[STATUS_INDEX] & 0x02) != (Rs485Control[k].OutputData[CONTROL_INDEX] & 0x02);

							if(Rs485Control[k].TxReady){
								printf("**** Send data channel %i\n", k+1);
								for(j = 2; j < 24; j++){
									Rs485Control[k].OutputData[j] = 0x55; // data channel 1
								}
								Rs485Control[k].OutputData[CONTROL_INDEX] = Rs485Control[k].OutputData[CONTROL_INDEX] ^ 0x01; // Rs485Control channel 1, Transmit request
								Rs485Control[k].OutputData[LENGTH_INDEX] = 10; // datalength channel 1
							}
						}

						for(k = 0; k < MAX_CHANNEL; k++){
						// analyze status channel 1
							transmit_accepted   = Rs485Control[k].InputData[STATUS_INDEX] & 0x01; 
							receive_request     = Rs485Control[k].InputData[STATUS_INDEX] & 0x02;
							init_accepted       = Rs485Control[k].InputData[STATUS_INDEX] & 0x04;
							buffer_full         = Rs485Control[k].InputData[STATUS_INDEX] & 0x08;
							parity_error        = Rs485Control[k].InputData[STATUS_INDEX] & 0x10;
							framing_error       = Rs485Control[k].InputData[STATUS_INDEX] & 0x20;
							overrun_error       = Rs485Control[k].InputData[STATUS_INDEX] & 0x40;
							
							// Receive datalength channel 1
							input_length        = Rs485Control[k].InputData[LENGTH_INDEX];
	
							printf("------------ Channel %i --------------\n", k + 1);
							printf("transmit_accepted : %d\n",transmit_accepted);
							printf("receive_request : %d\n",receive_request);
							printf("init_accepted : %d\n",init_accepted);
							printf("buffer_full : %d\n",buffer_full);
							printf("parity_error : %d\n",parity_error);
							printf("framing_error : %d\n",framing_error);
							printf("overrun_error : %d\n",overrun_error);
							printf("input_length : %d\n",input_length);
						}	
						for(k = 0; k < MAX_CHANNEL; k++){
							printf("**** Control Channel %i\n", k + 1);
							printf("       ReqState %i\n", Rs485Control[k].ReqState);
							printf("     status     %i\n", Rs485Control[k].InputData[STATUS_INDEX]);
							printf("     command    %i\n", Rs485Control[k].OutputData[CONTROL_INDEX]);
							printf("       TxReady  %i\n", Rs485Control[k].TxReady);
							printf("       RxSignal %i\n", Rs485Control[k].RxSignal);
							printf("       input1   %i\n", *ec_slave[2].inputs);
							printf("       output1  %i\n", *ec_slave[2].outputs);
							if((Rs485Control[k].ReqState == 1) && (Rs485Control[k].State == 0)){
								switch(Rs485Control[k].Istate){
									case 0:
										for(j = 2; j < 24; j++){
											Rs485Control[k].OutputData[j] = 0x00; // data channel 1
										}
										printf("              Init Request channel %i\n", k + 1);
										Rs485Control[k].OutputData[CONTROL_INDEX] = 0x04;
										Rs485Control[k].OutputData[LENGTH_INDEX] = 0;
										Rs485Control[k].Istate = 1;
										Rs485Control[k].Tries = 0;
										break;
									case 1:
										Rs485Control[k].Tries++;
										if(Rs485Control[k].InputData[STATUS_INDEX] & 0x04){
											printf("              Init Request Accepted channel %i\n", k + 1);
											Rs485Control[k].Istate = 3;
										}
										if(Rs485Control[k].Tries > 20){
											printf("              Too many Tries channel %i\n", k + 1);
											Rs485Control[k].Istate = 2;
										}
										break;
									case 2:
										Rs485Control[k].OutputData[CONTROL_INDEX] = 0x00;
										Rs485Control[k].ReqState = 0;
										Rs485Control[k].State = 0;
										Rs485Control[k].Istate = 0;
										break;
									case 3:
										Rs485Control[k].ReqState = 2;
										Rs485Control[k].State = 1;
										Rs485Control[k].Istate = 0;
										Rs485Control[k].Tries = 0;
										break;
								}
							}
							if((Rs485Control[k].ReqState == 2) && (Rs485Control[k].State == 1)){
								switch(Rs485Control[k].Istate){
									case 0:
										printf("              Clear Init Request channel %i\n", k + 1);
										Rs485Control[k].OutputData[CONTROL_INDEX] = 0x00;
										Rs485Control[k].OutputData[LENGTH_INDEX] = 0;
										Rs485Control[k].Istate = 1;
										Rs485Control[k].Tries = 0;
										break;
									case 1:
										Rs485Control[k].Tries++;
										if(!(Rs485Control[k].InputData[STATUS_INDEX] & 0x04)){
											printf("              Init Request Accepted Released channel %i\n", k + 1);
											Rs485Control[k].Istate = 3;
										}
										if(Rs485Control[k].Tries > 20){
											printf("              Too many Tries channel %i\n", k + 1);
											Rs485Control[k].Istate = 2;
										}
										break;
									case 2:
										Rs485Control[k].ReqState = 1;
										Rs485Control[k].State = 1;
										Rs485Control[k].Istate = 0;
										break;
									case 3:
										Rs485Control[k].ReqState = 2;
										Rs485Control[k].State = 2;
										Rs485Control[k].Istate = 0;
										Rs485Control[k].Tries = 0;
										break;
								}
							}
						}
						usleep(1);
	
					}
				}
				else
				{
					printf("No slaves reached operational state.\n");
				}			
				printf("Request safe operational state for all slaves\n");
				for(k = 1; k <= ec_slavecount; k++){
					ec_slave[k].state = EC_STATE_SAFE_OP;
				}
				/* request SAFE_OP state for all slaves */
				ec_writestate(0);
			}
			else{
				printf("No RS485 Rs485Controllers found!\n");
			}
		}
		else
		{
			printf("No slaves found!\n");
		}
		printf("End simple test, close socket\n");
		/* stop SOEM, close socket */
		ec_close();
	}
	else
	{
		printf("No socket connection on %s\nExcecute as root\n",ifname);
	}	
}	

int main(int argc, char *argv[])
{
	printf("SOEM (Simple Open EtherCAT Master)\nSimple RS485 test\n");
	
	if (argc > 1)
	{		
		/* start cyclic part */
		rs485_test(argv[1]);
	}
	else
	{
		printf("Usage: rs485_test ifname1\nifname = eth0 for example\n");
	}	
	
	printf("End program\n");
	return (0);
}
