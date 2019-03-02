#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/stat.h>

typedef enum
{
	off_led=0,
	on_led=1,
	bright_led=2,
	broken_led=3,
	dim_led=4
}led_enum;

struct data
{
	char str[50];
	uint32_t len;
	int led;
}object;

int i=0;
int sending;
static int flag;
char *stringarray[5] = {"hey", "sky", "snow", "purple", "orchid"};
pthread_mutex_t pmutex;
struct timespec currtime;
struct data obj_recv;
uint32_t randno;
char* fname;
mqd_t qid;
FILE* outputfile=NULL;

void signal_handler(int signo, siginfo_t *info, void *extra)
{
	if(signo == 15)
	{
		printf("Signal SIGTERM is being interrupted\n");
		flag = 1;
	}
	
	
	printf("Exit\n");

}

void siginitfunc()
{
	struct sigaction act_sig;
	act_sig.sa_flags = SA_SIGINFO;
	act_sig.sa_sigaction = &signal_handler;
	if(sigaction(SIGUSR1, &act_sig, NULL))
	{
		perror("error in sigaction\n");
		exit(1);
	}
}



void send_msg()
{
	outputfile = fopen(fname, "a");
	if (outputfile != NULL)
	{
		clock_gettime(CLOCK_REALTIME,&currtime);
		fprintf(outputfile,"\nThe operation is write\n currtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n", currtime.tv_nsec/(1000),object.str, object.len, object.led);
	}		
	fclose(outputfile);
}

void get_msg()
{
	outputfile = fopen(fname, "a");
	if (outputfile != NULL)
	{
		clock_gettime(CLOCK_REALTIME,&currtime);
		fprintf(outputfile,"\nThe operation is read\n currtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n", currtime.tv_nsec/(1000),object.str, object.len, object.led);
	}
		fclose(outputfile);
}

void mq_receivecommand()
{
	mq_receive(qid,(char *)&obj_recv,100,NULL);
}

void mq_sendcommand()
{
	sending = mq_send(qid, (char*)&object, sizeof(struct data), 0);
	if (sending == -1)
	{
		perror("\nmqsend error\n");
	}
}
int main(int argc, char *argv[])
{	
	if(argc<2)
	{
		printf("\n<usage> ./name_file <outputlogfile>\n");
		exit(1);
	}
	siginitfunc();	
	srand(time(0));
	struct mq_attr attrqueue;
	attrqueue.mq_flags=0;
	attrqueue.mq_maxmsg=10;
	attrqueue.mq_msgsize=100;
	attrqueue.mq_curmsgs=0;
	pid_t parentpid = getpid();
	pthread_mutex_lock(&pmutex);
	fname = argv[1];
	outputfile = fopen(argv[1], "a");
	if (outputfile!=NULL)
	{
		fprintf(outputfile, "\n\t\rthe method used is IPC queue message is written to queue2\t\rparent pid number is  %d\n", parentpid);
	}
	fprintf(outputfile,"\nPID Number = %d\n", parentpid);
	fclose(outputfile);
	pthread_mutex_unlock(&pmutex);
	qid = mq_open("/mymq", O_RDWR|O_CREAT, 0644, &attrqueue);
	if(qid==-1)
	{
		perror("\ninvaild qid error\n");
		exit(1);
	}
	for (i=0;i<10;i++)
	{
		randno = rand()%(10);
		if(randno==0)
		{	
			strcpy(object.str,stringarray[randno]);
			object.len = strlen(stringarray[randno]);
				
		}
		else if(randno==1)
		{
			object.led = on_led;
			strcpy(object.str,stringarray[randno]);
			object.len=strlen(stringarray[randno]);
	 	}
	 	else if(randno==2)
	 	{
			object.led = off_led;
			strcpy(object.str,stringarray[randno]);
			object.len=strlen(stringarray[randno]);
	 	}
	 	else if(randno==3)
	 	{				
			strcpy(object.str,stringarray[randno]);
			object.len=strlen(stringarray[randno]);
				
	  	}
	 	else if(randno==4)
	  	{
			object.led = on_led;
			strcpy(object.str,stringarray[randno]);
			object.len = strlen(stringarray[randno]);
	  	}
	  	else if(randno==5)
	  	{
			object.led = on_led;
	  	}
	  	else if(randno==6)
	  	{
			object.led = bright_led;
	 	} 
	  	else if(randno==7)
	  	{
			object.led = dim_led;
	  	}
	  	else if(randno==8)
	  	{
			object.led = bright_led;
	 	}
	  	else if(randno==9)
	  	{				
			object.led = off_led;
	 	}
		mq_sendcommand();
		sleep(1);
		pthread_mutex_lock(&pmutex);
		send_msg();
		pthread_mutex_unlock(&pmutex);

	}
	for (i=0; i<10; i++)
	{
		pthread_mutex_lock(&pmutex);
		get_msg();
		pthread_mutex_unlock(&pmutex);
	}
	if(mq_unlink("/mymq"))
	{
		perror("\n mq unlink error \n");
		exit(1);
	}

	if(mq_close(qid))
	{
		perror("\n mq close error\n");
		exit(1);
	}
	return 0;

printf("\Exiting\n");

}
