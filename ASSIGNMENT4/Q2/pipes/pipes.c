#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

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
int pipe1[2];	
int pipe2[2];
char readdata[sizeof(struct data)];

void send_data(int pipeno[])
{
	write(pipeno[1], &object, sizeof(struct data));
}

void receive_data(int pipeno[])
{
	read(pipeno[0], readdata, sizeof(readdata));
}

int main(int argc, char *argv[])
{		
	object.led = bright_led;
	struct data *ptr = (struct data *)readdata;
	if(argc<2)
	{
		printf("\n<usage> ./name_file <outputlogfile>\n");
		exit(1);
	}
	if(pipe(pipe1))
	{
		perror("pipe creation_led failed\n");
		exit(1);
	}
	if(pipe(pipe2))
	{
		perror("pipe creation_led failed\n");
		exit(1);
	}
	mainpid = fork();
	if(mainpid==-1)
	{
		perror("error in fork\n");
		exit(1);
	}
	else if(mainpid > 0)		 
	{
		pid_t parentpipepid = getpid();
		childpid = mainpid;
		close(pipe1[0]);
		close(pipe2[1]);
		pthread_mutex_lock(&pmutex);
		FILE *outputfile = fopen(argv[1], "a");
		if (outputfile != NULL)
		{
			temp=getpid();
			fprintf(outputfile, "\nmessage is written to pipe1\t\r and the method used is IPC pipes \t\r pid = %d\n", temp);
			
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
			send_data(pipe1);
			clock_gettime(CLOCK_REALTIME,&curtime);
			temp=getpid();
			fprintf(outputfile, "\nmessage is read from pipe 1\t\r and the method used is IPC pipes \t\r pid = %d",temp);
			fprintf(outputfile,"\nThe operation is write\n curtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n", curtime.tv_nsec/(1000),object.str, object.len, object.led );
		}
		fclose(outputfile);
		outputfile = fopen(argv[1], "a");
		if (outputfile != NULL)
		{
			temp=getpid();
			fprintf(outputfile, "\nmessage is read from pipe 2\t\r and the method used is IPC pipes \t\rpid = %d\n",temp);
		}
		for(i=0; i<10;i++)
		{
			receive_data(pipe2);
			clock_gettime(CLOCK_REALTIME,&curtime);
			temp=getpid();
			fprintf(outputfile, "\nmessage is read from pipe 1\t\r and the method used is IPC pipes \t\r pid = %d",temp);
			fprintf(outputfile,"\n The operation is read\n curtime  is %ld microseconds\n the string sent is %s\t\n and its length is %d \t\n and the led status is %d\n", curtime.tv_nsec/(1000),ptr->str, ptr->len, ptr->led);

		}
		fclose(outputfile);
		pthread_mutex_unlock(&pmutex);
	}
	else if(mainpid == 0)	
	{
		close(pipe1[1]);
		close(pipe2[0]);
		pthread_mutex_lock(&pmutex);
		FILE *outputfile = fopen(argv[1], "a");
		if (outputfile != NULL)
		{
			temp=getpid();
			fprintf(outputfile, "\nmessage is read from pipe 1\t\r and the method used is IPC pipes \t\r pid = %d\n",temp);
		}
		for(i=0;i<10;i++)
		{
			receive_data(pipe1);
			clock_gettime(CLOCK_REALTIME,&curtime);
			temp=getpid();
			fprintf(outputfile, "\nmessage is read from pipe 1\t\r and the method used is IPC pipes \t\r pid = %d",temp);
			fprintf(outputfile,"\n The operation is read\n curtime  is %ld microseconds\n the string sent is %s\t\n and its length is %d \t\n and the led status is %d\n",curtime.tv_nsec/(1000),ptr->str, ptr->len, ptr->led);
		}
		fclose(outputfile);
		outputfile = fopen(argv[1], "a");
		if (outputfile != NULL)
		{
			fprintf(outputfile, "\nmessage is written to pipe 2 by the pid %d\t\r and the method used is IPC pipes \t\n", childpid);
		}
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
			send_data(pipe2);
			clock_gettime(CLOCK_REALTIME,&curtime);
			temp=getpid();
			fprintf(outputfile, "\nmessage is read from pipe 1\t\r and the method used is IPC pipes \t\r pid = %d",temp);
			fprintf(outputfile,"\nThe operation is write\n curtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n",curtime.tv_nsec/(1000),object.str, object.len, object.led );
		}
		fclose(outputfile);
		pthread_mutex_unlock(&pmutex);
	}	
	return 0;
}

