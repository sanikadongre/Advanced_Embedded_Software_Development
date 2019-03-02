#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>

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
	int len;
	int led;
}object;

char *stringarray[5] = {"hey", "sky", "snow", "purple", "orchid"};
pthread_mutex_t pmutex;
struct timespec curtime;
ssize_t mq_error_cond;
uint32_t randno;
int send_data;
mqd_t qid1,qid2;
struct data obj_recv;
struct mq_attr attrqueue;
FILE* outputfile=NULL;
char* fname;
int i=0;

void send_command()
{
	 send(sockfdaccept, "Hi!" ,4, 0);
}

void recv_command()
{
	  obtbytes = read(sockfdaccept, ptr->str, 100); //reading message size
	  if(obtbytes<0)
	  {
		printf("couldn't read data %d\n", ptr->len);
	  }	
}

void sendmsg()
{
	outputfile = fopen(fname, "a");
	if (outputfile != NULL)
	{
		clock_gettime(CLOCK_REALTIME,&curtime);
		fprintf(outputfile,"\nThe operation is send\n curtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n", curtime.tv_nsec/(1000),object.str, object.len, object.led);
	}
	fclose(outputfile);
}

void getmsg()
{
	outputfile = fopen(fname, "a");
	if (outputfile != NULL)
	{
		clock_gettime(CLOCK_REALTIME,&curtime);
		fprintf(outputfile,"\nThe operation is receive\n curtime is %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n", curtime.tv_nsec/(1000),object.str, object.len, object.led);	
	}
	fclose(outputfile);
} 

int main(int argc, char *argv[])
{	
	if(argc<2)
	{
		printf("\n<usage> ./name_file <outputlogfile>\n");
		exit(1);
	}
	struct sockaddr_in addr, clientaddr;
	int addr_len = sizeof(clientaddr);
	struct data *ptr;
	int obtbytes;	
	//srand(time(0));
	//attrqueue.mq_flags=0;
	//attrqueue.mq_maxmsg=10;
	//attrqueue.mq_msgsize=100;
	//attrqueue.mq_curmsgs=0;
	pid_t parentpid = getpid();
	pthread_mutex_lock(&pmutex);
	fname=argv[1];
	outputfile = fopen(argv[1], "a");
	if (outputfile != NULL)
	{
		fprintf(outputfile,"\n\t\rThe method used is IPC queues\t\rparent pid number is  %d\n", parentpid);
	}
	fclose(outputfile);
	pthread_mutex_unlock(&pmutex);
	//qid1 = mq_open("/mymq1", O_RDWR, 0644, &attrqueue);
	//qid2 = mq_open("/mymq2", O_RDWR, 0644, &attrqueue);
	//if(qid1<0)
	//{
		//perror("\nmessage queue open\n");
	//}
	//if(qid2<0)
	//{
		//perror("\nmessage queue open\n");
	//}
	if((server_socket = socket(AF_INET,SOCK_STREAM,0)) == 0) //socket creation
	  {
	   	perror("socket creation error\n");
	    	exit(1);
	  }
	  else
	  {
	   	printf("socket created successfully\n");
	  }
	  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(opt), sizeof(opt)))
	  {
	   	perror("error\n");
	    	exit(1);
	  }
	  addr.sin_family = AF_INET;
	  addr.sin_addr.s_addr = INADDR_ANY;	
	  addr.sin_port = htons(5000);
	  if((bind(server_socket,(struct sockaddr*)&addr, sizeof(addr))) < 0) //socket bind
	  {
	    	perror("socket binding error\n");
	    	exit(1);
	  }
	  else
	  {
	    	printf("socket binded Successfully\n");
	  }
	  if(listen(server_socket,8) < 0) //listening for connections
	  {
	    	perror("socket listening error\n");
	    	exit(1);
	  }
	  sockfdaccept = accept(server_socket, (struct sockaddr*)&clientaddr,(socklen_t*)&addr_len); //socket accept
	  if(sockfdaccept < 0) 
	  {
	    	perror("socket accepting error\n");
	    	exit(1);
	  }
	  
	for(i=0;i<10;i++)
	{
		randno = rand()%(10);
		if(randno==0)
		{	
			object.str = stringarray[randno];
			object.len = strlen(stringarray[randno]);
				
		}
		else if(randno==1)
		{
			object.led = on_led;
			object.str=stringarray[randno];
			object.len=strlen(stringarray[randno]);
	 	}
	 	else if(randno==2)
	 	{
			object.led = off_led;
			object.str=stringarray[randno];
			object.len=strlen(stringarray[randno]);
	 	}
	 	else if(randno==3)
	 	{				
			object.str=stringarray[randno];
			object.len=strlen(stringarray[randno]);
				
	  	}
	 	else if(randno==4)
	  	{
			object.led = on_led;
			object.str=stringarray[randno];
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
		send_command();
		sleep(1);
		pthread_mutex_lock(&pmutex);
		sendmsg();
		pthread_mutex_unlock(&pmutex);

	}
	for(i=0;i<10;i++)
	{
		//recv_command();
		while((obtbytes = read(sockfdaccept, readdata+i, 1024)) < ptr->len)
	  	{
	    		i+=obtbytes;	
	  	}
	 	 ptr= (struct data*)readdata;
	  	printf("Message received from Client is \n string is %s\n string length is %d\n USR LED is %d\n",
		                                ptr->str, ptr->len, ptr->led);
		pthread_mutex_lock(&pmutex);
		getmsg();
		pthread_mutex_unlock(&pmutex);

	}
	return 0;
}

