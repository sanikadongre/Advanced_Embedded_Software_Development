#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MEMORY_SIZE 200
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
	char *str;
	int len;
	int led;
}object;

char *stringarray[5] = {"hey", "sky", "snow", "purple", "orchid"};
pid_t mainpid;
pid_t childpid;
pid_t temp;
pthread_mutex_t pmutex;
uint32_t getrand;
int i=0;
void sendfunc();
struct timespec curtime;
FILE* outputfile=NULL;
key_t key;
int32_t mem[2]={0,0};
struct data readdata;

void send_data(int mem_no)
{
	uint8_t* mem_ptr=shmat(mem[mem_no],(void*)0,0);
	memcpy(mem_ptr,&object, sizeof(struct data));
	shmdt(mem_ptr);
}

void receive_data(int mem_no)
{
	uint8_t* mem_ptr=shmat(mem[mem_no],(void*)0,0);	
	memcpy(mem_ptr,&readdata, sizeof(struct data));
	shmdt(mem_ptr);
}

int main(int argc, char *argv[])
{		
	object.led = bright_led;
	struct data *ptr = &readdata;
	if(argc<2)
	{
		printf("\n<usage> ./name_file <outputlogfile>\n");
		exit(1);
	}
	mem[0]=shmget(1,MEMORY_SIZE,0666|IPC_CREAT);	
	mem[1]=shmget(2,MEMORY_SIZE,0666|IPC_CREAT);
	mainpid = fork();
	if(mainpid==-1)
	{
		perror("error in fork\n");
		exit(1);
	}
	else if(mainpid > 0)		 
	{
		pthread_mutex_lock(&pmutex);
		pid_t parentpipepid = getpid();
		childpid = mainpid;
		FILE *outputfile = fopen(argv[1], "a");
		if(outputfile!=NULL)
		{
			clock_gettime(CLOCK_REALTIME,&curtime);
			fprintf(outputfile, "\nmessage is written to shared memory1\t\r and the method used is IPC shared memorys\n\t\rparent pid number is  %d\n", parentpipepid);
		}
		for (i=0;i<10;i++)
		{
			getrand = rand()%(10);
			if(getrand==0)
			{	
				object.str = stringarray[getrand];
				object.len = strlen(stringarray[getrand]);
				
			}
			else if(getrand==1)
			{
				object.led = on_led;
				object.str=stringarray[getrand];
				object.len=strlen(stringarray[getrand]);
	 		}
	 		else if(getrand==2)
	 		{
				object.led = off_led;
				object.str=stringarray[getrand];
				object.len=strlen(stringarray[getrand]);
	 		}
	 		else if(getrand==3)
	 		{				
				object.str=stringarray[getrand];
				object.len=strlen(stringarray[getrand]);
				
	  		}
	 		else if(getrand==4)
	  		{
				object.led = on_led;
				object.str=stringarray[getrand];
				object.len = strlen(stringarray[getrand]);
	  		}
	  		else if(getrand==5)
	  		{
				object.led = on_led;
	  		}
	  		else if(getrand==6)
	  		{
				object.led = bright_led;
	 		} 
	  		else if(getrand==7)
	  		{
				object.led = dim_led;
	  		}
	  		else if(getrand==8)
	  		{
				object.led = bright_led;
	 		}
	  		else if(getrand==9)
	  		{				
				object.led = off_led;
	 		}
			send_data(0);
			temp = getpid();
			fprintf(outputfile, "\nmessage is written to shared memory1\t\r and the method used is IPC shared memorys\n\t\rpid = %d",temp);
			fprintf(outputfile,"\nThe operation is write\n curtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n", curtime.tv_nsec/(1000),object.str, object.len, object.led );
		}
		fclose(outputfile);
		outputfile = fopen(argv[1], "a");
		for(i=0; i<10;i++)
		{
			receive_data(1);
			clock_gettime(CLOCK_REALTIME,&curtime);
			temp = getpid();			
			fprintf(outputfile, "\nmessage is written to shared memory 2\t\r and the method used is IPC shared memorys \t\rpid =  %d", temp);
			fprintf(outputfile,"\n The operation is read\n curtime  is %ld microseconds\n the string sent is %s\t\n and its length is %d \t\n and the led status is %d\n", curtime.tv_nsec/(1000),ptr->str, ptr->len, ptr->led);

		}
		fclose(outputfile);
		pthread_mutex_unlock(&pmutex);
	}
	else if(mainpid == 0)	
	{
		pthread_mutex_lock(&pmutex);
		FILE *outputfile = fopen(argv[1], "a");
		for(i=0;i<10;i++)
		{
			receive_data(0);
			clock_gettime(CLOCK_REALTIME,&curtime);
			temp = getpid();
			fprintf(outputfile, "\nmessage is written to shared memory 2\t\r and the method used is IPC shared memorys \t\rpid =  %d", temp);
			fprintf(outputfile,"\n The operation is read\n curtime  is %ld microseconds\n the string sent is %s\t\n and its length is %d \t\n and the led status is %d\n",curtime.tv_nsec/(1000),ptr->str, ptr->len, ptr->led);
		}	
		fclose(outputfile);
		outputfile = fopen(argv[1], "a");
		for(i=0;i<10;i++)
		{
			getrand = rand()%(10) ;
			if(getrand==0)
			{	
				object.led = bright_led;
				object.str = stringarray[getrand];
				object.len = strlen(stringarray[getrand]);
				
			}
			else if(getrand==1)
			{				
				object.str = stringarray[getrand];
				object.len= strlen(stringarray[getrand]);
				
	 		}
	 		else if(getrand==2)
	 		{
				object.led = broken_led;
				object.str=stringarray[getrand];
				object.len= strlen(stringarray[getrand]);
	 		}
	 		else if(getrand==3)
	 		{
				object.str=stringarray[getrand];
				object.len=strlen(stringarray[getrand]);
				
	  		}
	 		else if(getrand==4)
	  		{
				object.led = broken_led;
				object.str=stringarray[getrand];
				object.len = strlen(stringarray[getrand]);
	  		}
	  		else if(getrand==5)
	  		{
				object.led = dim_led;
	  		}
	  		else if(getrand==6)
	  		{
				object.led = off_led;
	 		} 
	  		else if(getrand==7)
	  		{
				object.led = on_led;
	  		}
	  		else if(getrand==8)
	  		{
				object.led = bright_led;
	 		}
	  		else if(getrand==9)
	  		{
				object.led = dim_led;
	 		}
			send_data(1);
			clock_gettime(CLOCK_REALTIME,&curtime);
			temp=getpid();
			fprintf(outputfile, "\nmessage is written to shared memory 2\t\r and the method used is IPC shared memorys \t\rpid =  %d", temp);
			fprintf(outputfile,"\nThe operation is write\n curtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n",curtime.tv_nsec/(1000),object.str, object.len, object.led );
		}
		fclose(outputfile);
		pthread_mutex_unlock(&pmutex);
	}	
	return 0;
}

