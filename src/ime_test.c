/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : ime_test [ifname] [cycletime]
 * ifname is NIC interface, f.e. eth0
 * cycletime in us, f.e. 500
 *
 * This test is specifically build for the IME EtherCAT Box.
 *
 * (c)Arthur Ketels 2008 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <postgresql/libpq-fe.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatcoe.h"
#include "ethercatconfig.h"
#include "ethercatdc.h"

#define NSEC_PER_SEC 1000000000

typedef struct PACKED
{
	uint8	   status1;
	int16	   invalue1;
	uint8	   status2;
	int16	   invalue2;
} in_EL3102t;	

typedef struct PACKED
{
	int16	   outvalue1;
	int16	   outvalue2;
} out_EL4132t;

typedef struct PACKED
{
	uint8	   inbits;
} in_EL1018t;	

typedef struct PACKED
{
	uint8	   outbits;
} out_EL2008t;	

typedef struct PACKED
{
	uint8	   control;
	int16	   outvalue;
} out_EL5101t;	

typedef struct PACKED
{
	uint8	   status;
	int16	   invalue;
	int16	   latch;
} in_EL5101t;	

struct sched_param schedp;
char IOmap[4096];
pthread_t thread1;
struct timeval tv,t1,t2;
int dorun = 0;
int deltat, tmax=0;
int64 toff;
int DCdiff;
int os;
uint8 ob;
uint16 ob2;
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;
int64 integral=0;
in_EL3102t		*in_EL3102;
out_EL4132t		*out_EL4132;
in_EL1018t		*in_EL1018;
out_EL2008t		*out_EL2008;
out_EL5101t		*out_EL5101_1;
out_EL5101t		*out_EL5101_2;
in_EL5101t		*in_EL5101_1;
in_EL5101t		*in_EL5101_2;

in_EL5101t		test = { .status = 4, .latch = -10};

void imetest(char *ifname)
{
	int cnt, i;
	
	printf("Starting IME test\n");
	
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
			
			/* configure DC options for every DC capable slave found in the list */
			ec_configdc();
			
			/* check configuration */
			if (( ec_slavecount >= 7 ) &&
				 (strcmp(ec_slave[2].name,"EL3102") == 0) &&
				 (strcmp(ec_slave[3].name,"EL4132") == 0) &&
				 (strcmp(ec_slave[4].name,"EL1018") == 0) &&
				 (strcmp(ec_slave[5].name,"EL2008") == 0) &&
				 (strcmp(ec_slave[6].name,"EL5101") == 0) &&
				 (strcmp(ec_slave[7].name,"EL5101") == 0)
			   )
			{
				printf("Correct configuration for IME Box\n");
			/* connect struct pointers to slave I/O pointers */
			in_EL3102 = (in_EL3102t*) ec_slave[2].inputs;
			out_EL4132 = (out_EL4132t*) ec_slave[3].outputs;
			in_EL1018 = (in_EL1018t*) ec_slave[4].inputs;
			out_EL2008 = (out_EL2008t*) ec_slave[5].outputs;
			out_EL5101_1 = (out_EL5101t*) ec_slave[6].outputs;
			in_EL5101_1 = (in_EL5101t*) ec_slave[6].inputs;
			out_EL5101_2 = (out_EL5101t*) ec_slave[7].outputs;
			in_EL5101_2 = (in_EL5101t*) ec_slave[7].inputs;

			/* set filter on for EL3102 Index 0x4061:03 = 1 */
			os=sizeof(ob); ob=1;
			ec_SDOwrite(2,0x4061,03,FALSE,os,&ob,EC_TIMEOUTRXM);
			
			/* set filter to IIR level 8 */
			os=sizeof(ob2); ob2=9;
			ec_SDOwrite(2,0x4065,00,FALSE,os,&ob2,EC_TIMEOUTRXM);

			/* read filter value */
			os=sizeof(ob2); ob2=0;
			ec_SDOread(2,0x4065,00,FALSE,&os,&ob2,EC_TIMEOUTRXM);
			
			/* read indevidual slave state and store in ec_slave[] */
			ec_readstate();
			for(cnt = 1; cnt <= ec_slavecount ; cnt++)
			{
				printf("Slave:%d Name:%s Output size:%3dbits Input size:%3dbits State:%2d delay:%d.%d\n",
					   cnt, ec_slave[cnt].name, ec_slave[cnt].Obits, ec_slave[cnt].Ibits,
					   ec_slave[cnt].state, (int)ec_slave[cnt].pdelay, ec_slave[cnt].hasdc);
			}
			printf("Request operational state for all slaves\n");
			ec_slave[0].state = EC_STATE_OPERATIONAL;
			/* request OP state for all slaves */
			ec_writestate(0);
			/* wait for all slaves to reach OP state */
			ec_statecheck(0, EC_STATE_OPERATIONAL,  EC_TIMEOUTSTATE);
			if (ec_slave[0].state == EC_STATE_OPERATIONAL )
			{
				printf("Operational state reached for all slaves.\n");
				dorun = 1;
				/* acyclic loop 20ms */
				for(i = 1; i <= 500; i++)
				{
					/* read DC difference register for slave 5 = EL2008 */
					ec_FPRD(ec_slave[5].configadr, ECT_REG_DCSYSDIFF, sizeof(DCdiff), &DCdiff, EC_TIMEOUTRET);
					if(DCdiff<0) { DCdiff = - (int32)((uint32)DCdiff & 0x7ffffff); }
					printf("Processdata cycle %5d , DCtime %12lld, Offsettime %5lld, Diff slave 5 %7d\n",
						   dorun, ec_DCtime, toff, DCdiff);
					printf("   AD:%6d,%6d  DigIn:%4d  Count:%6d,%6d\n",
						   in_EL3102->invalue1, in_EL3102->invalue2, in_EL1018->inbits, in_EL5101_1->invalue, out_EL4132->outvalue2);
					usleep(20000);
				}
				dorun = 0;
			}
			else
			{
				printf("Not all slaves reached operational state.\n");
			}			
			}
			else
			{
				printf("IME box not found in slave configuration.\n");
			}	
			printf("Request safe operational state for all slaves\n");
			ec_slave[0].state = EC_STATE_SAFE_OP;
			/* request SAFE_OP state for all slaves */
			ec_writestate(0);
		}
		else
		{
			printf("No slaves found!\n");
		}
		printf("End ime_test, close socket\n");
		/* stop SOEM, close socket */
		ec_close();
	}
	else
	{
		printf("No socket connection on %s\nExcecute as root\n",ifname);
	}	
}	

/* add ns to timespec */
void add_timespec(struct timespec *ts, int64 addtime)
{
	int64 sec, nsec;
	
	nsec = addtime % NSEC_PER_SEC;
	sec = (addtime - nsec) / NSEC_PER_SEC;
	ts->tv_sec += sec;
	ts->tv_nsec += nsec;
	if ( ts->tv_nsec > NSEC_PER_SEC ) 
	{ 
		nsec = ts->tv_nsec % NSEC_PER_SEC;
		ts->tv_sec += (ts->tv_nsec - nsec) / NSEC_PER_SEC;
		ts->tv_nsec = nsec;
	}	
}	

/* PI calculation to get linux time synced to DC time */
void ec_sync(int64 reftime, int64 cycletime , int64 *offsettime)
{
	int64 delta;
	/* set linux sync point 50us later than DC sync, just as example */
	delta = (reftime - 50000) % cycletime;
	if(delta> (cycletime /2)) { delta= delta - cycletime; }
	if(delta>0){ integral++; }
	if(delta<0){ integral--; }
	*offsettime = -(delta / 100) - (integral /20);
}	

/* RT EtherCAT thread */
void ecatthread( void *ptr )
{
	struct timespec   ts;
	struct timeval    tp;
	int rc;
	int ht;
	int64 cycletime;
	
    rc = pthread_mutex_lock(&mutex);
	rc =  gettimeofday(&tp, NULL);

    /* Convert from timeval to timespec */
    ts.tv_sec  = tp.tv_sec;
	ht = (tp.tv_usec / 1000) + 1; /* round to nearest ms */
    ts.tv_nsec = ht * 1000000;
	cycletime = *(int*)ptr * 1000; /* cycletime in ns */
	toff = 0;
	dorun = 0;
	while(1)
	{	
		/* calculate next cycle start */
		add_timespec(&ts, cycletime + toff);
		/* wait to cycle start */
		rc = pthread_cond_timedwait(&cond, &mutex, &ts);
		if (dorun>0)
		{
			rc =  gettimeofday(&tp, NULL);

			ec_send_processdata();

			ec_receive_processdata(EC_TIMEOUTRET);
			
			dorun++;
			
			out_EL2008->outbits = (uint8)(dorun / 5);
//			out_EL4132->outvalue1 = in_EL3102->invalue1;
			out_EL4132->outvalue1 = (int16)((double)sin(dorun/10.0)*0x7000);
			out_EL4132->outvalue2 = (int16)dorun;

			/* calulate toff to get linux time and DC synced */
			ec_sync(ec_DCtime, cycletime, &toff);
		}	
	}	 
}

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

void do_sql(void)
{
    const char *conninfo;
    PGconn     *conn;
    PGresult   *res;
    int         nFields;
    int         i,
                j;

     conninfo = "dbname = inventor";

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        exit_nicely(conn);
    }

    /*
     * Our test case here involves using a cursor, for which we must be inside
     * a transaction block.  We could do the whole thing with a single
     * PQexec() of "select * from pg_database", but that's too trivial to make
     * a good example.
     */

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)	
    {
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    /*
     * Should PQclear PGresult whenever it is no longer needed to avoid memory
     * leaks
     */
    PQclear(res);

    /*
     * Fetch rows from pg_database, the system catalog of databases
     */
    res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from pg_database");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);

    res = PQexec(conn, "FETCH ALL in myportal");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }		
    /* first, print out the attribute names */
    nFields = PQnfields(res);
    for (i = 0; i < nFields; i++)
        printf("%-15s", PQfname(res, i));
    printf("\n\n");

    /* next, print out the rows */
    for (i = 0; i < PQntuples(res); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(res, i, j));
        printf("\n");
    }

    PQclear(res);

    /* close the portal ... we don't bother to check for errors ... */
    res = PQexec(conn, "CLOSE myportal");
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

}

int main(int argc, char *argv[])
{
	int iret1;
    int ctime;
	struct sched_param    param;
	int                   policy = SCHED_OTHER;
	
	printf("SOEM (Simple Open EtherCAT Master)\nIME EtherCAT box test\n");
	
	memset(&schedp, 0, sizeof(schedp));
	/* do not set priority above 49, otherwise sockets are starved */
	schedp.sched_priority = 30;
	sched_setscheduler(0, SCHED_FIFO, &schedp);

	do
	{
		usleep(1000);
	}
	while (dorun);
	
	if (argc > 2)
	{		
		dorun = 1;
		ctime = atoi(argv[2]);
		/* create RT thread */
		iret1 = pthread_create( &thread1, NULL, (void *) &ecatthread, (void*) &ctime);	
		memset(&param, 0, sizeof(param));
		/* give it higher priority */
	    param.sched_priority = 40;
	    iret1 = pthread_setschedparam(thread1, policy, &param);

		do_sql();

		/* start acyclic part */
	//	imetest(argv[1]);
	}
	else
	{
//		printf("Usage: ime_test ifname cycletime\nifname = eth0 for example\ncycletime in us\n");
		do_sql();
	}	
	
	schedp.sched_priority = 0;
	sched_setscheduler(0, SCHED_OTHER, &schedp);

	printf("End program\n");
	return (0);
}
