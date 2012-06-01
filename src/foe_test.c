/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : simple_test [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 * (c)Arthur Ketels 2010 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"

#define FWBUFSIZE (0x200000)
#define MODE_NONE		0
#define MODE_READBIN	1
#define MODE_READINTEL	2
#define MODE_WRITEBIN	3
#define MODE_WRITEINTEL	4

#define MAXSLENGTH		256

uint8 ob;
uint16 ow;
uint32 data;
char filename[256];
char filebuffer[FWBUFSIZE]; // 4MB buffer
int filesize;
int wkc;
uint16 argslave;
int slave;
struct timeval tstart,tend, tdif;
int wkc;
int mode;
char sline[MAXSLENGTH];


int input_bin(char *fname, int *length)
{
    FILE *fp;
 
	int cc = 0, c;

    fp = fopen(fname, "rb");
    if(fp == NULL) 
        return 0;
	while (((c = fgetc(fp)) != EOF) && (cc < FWBUFSIZE))
		filebuffer[cc++] = (uint8)c;
	*length = cc;
	fclose(fp);
	return 1;
}

int output_bin(char *fname, int length)
{
    FILE *fp;
 
	int cc;

    fp = fopen(fname, "wb");
    if(fp == NULL) 
        return 0;
	for (cc = 0 ; cc < length ; cc++)
		fputc( filebuffer[cc], fp);
	fclose(fp);
	return 1;
}

void firmwaretool(char *ifname, int slave, int mode, char *fname)
{
	int wkc, rc;
	
	/* initialise SOEM, bind socket to ifname */
	if (ec_init(ifname))
	{	
		printf("ec_init on %s succeeded.\n",ifname);

	    if ( ec_config_init(FALSE) > 0 )
	    {
			printf("%d slaves found.\n",ec_slavecount);
			if((ec_slavecount >= slave) && (slave > 0))
			{
				ec_statecheck(0, EC_STATE_PRE_OP,  EC_TIMEOUTSTATE * 4);
				if ((mode == MODE_READBIN))
				{
					rc =  gettimeofday(&tstart, NULL);

					filesize = 1484404;
					wkc = ec_FOEread(slave, fname, 0x00000000, &filesize, &filebuffer, EC_TIMEOUTSTATE);
					printf("Read result : %d %d :%s:\n", wkc, filesize, fname);
					if(wkc)
					{
						printf("Writing file %s\n", fname);
						output_bin (fname, filesize);
					}

					rc =  gettimeofday(&tend, NULL);
					timersub(&tend, &tstart, &tdif);

					printf("\nTotal firmware read time :%ldms\n", (tdif.tv_usec+(tdif.tv_sec*1000000L)) / 1000);
				}
				if ((mode == MODE_WRITEBIN))
				{
					filesize = 0;
					if (mode == MODE_WRITEBIN) 	 rc = input_bin(fname, &filesize);

					if (rc > 0)
					{					
						fflush(stdout);
						printf("Writing firmware....\n");
						rc =  gettimeofday(&tstart, NULL);
						wkc = ec_FOEwrite(slave, fname, 0x00000000, filesize, &filebuffer, EC_TIMEOUTSTATE);
						rc =  gettimeofday(&tend, NULL);					
						timersub(&tend, &tstart, &tdif);

						printf("\nTotal firmware write time :%ldms\n", (tdif.tv_usec+(tdif.tv_sec*1000000L)) / 1000);
					}
					else
						printf("Error reading file, abort.\n"); 
				}
				while (EcatError)
					printf("%s\n", ec_elist2string());
			}
			else
				printf("Slave number outside range.\n");

			printf("Request init state for all slaves\n");
			ec_slave[0].state = EC_STATE_INIT;
			/* request INIT state for all slaves */
			ec_writestate(0);

		}
		else
			printf("No slaves found!\n");
		printf("End, close socket\n");
		/* stop SOEM, close socket */
		ec_close();
	}
	else
		printf("No socket connection on %s\nExcecute as root\n",ifname);

}	

int main(int argc, char *argv[])
{
	printf("SOEM (Simple Open EtherCAT Master)\nFirmware tool\n");

	if (argc > 4)
	{		
		slave = atoi(argv[2]);
		mode = MODE_NONE;
		if ((strncmp(argv[3], "-r", sizeof("-r")) == 0))   mode = MODE_READBIN;
		if ((strncmp(argv[3], "-w", sizeof("-w")) == 0))   mode = MODE_WRITEBIN;
		
		/* start tool */
		firmwaretool(argv[1],slave,mode,argv[4]);
	}
	else
	{
		printf("Usage: firmwaretool ifname slave OPTION fname\n");
		printf("ifname = eth0 for example\n");
		printf("slave = slave number in EtherCAT order 1..n\n");
		printf("    -r      read firmware, output binary format\n");
		printf("    -w      write firmware, input binary format\n");
	}	
	
	printf("End program\n");
	return (0);
}
