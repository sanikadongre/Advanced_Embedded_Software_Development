#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <time.h>


#define PORT (5000) 

typedef enum
{
	off_led=0,
	on_led=1,
	bright_led=2,
	broken_led=3,
	dim_led=4
}led_enum;

pid_t temp;
struct sockaddr_in server_addr, client_addr;
struct hostent *hptr;
int buff_size = 0;
int flag = 0;
int sockfd, acc, client_len;
char string[100];
FILE *fptr;
uint32_t getrand;


struct data
{
	char *string;
	int led;
}object;

char *stringarray[5] = {"hey", "sky", "snow", "purple", "orchid"};
pid_t mainpid;
pid_t childpid;
pthread_mutex_t pmutex;
uint32_t getrand;
int i=0;
struct timespec currtime;
FILE* outputfile=NULL;

int len;
char readdata[sizeof(struct data)];

void handler(int signo, siginfo_t *info, void *extra) 
{
	clock_gettime(CLOCK_REALTIME, &currtime);
	fptr = fopen("socketlog.txt", "a");
    	fprintf(fptr,"currtime is %ld and the thread exits \n",currtime.tv_nsec/(1000));
    	fclose(fptr);
	close(sockfd);
	exit(0);
}

void sigfunc(void)
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO; 
	action.sa_sigaction = handler;
	if (sigaction(SIGINT, &action, NULL) == -1)
	{ 
		perror("sigusr: sigaction");
		exit(0);
	} 
}


int main(int argc, char *argv[])
{		
	sigfunc();
	struct data object;
	object.led = bright_led;
	struct data *ptr = (struct data *)readdata;
	if(argc<2)
	{
		printf("\n<usage> ./name_file <outputlogfile>\n");
		exit(1);
	}
	fptr = fopen("socketlog.txt", "a");
	fclose(fptr);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if((sockfd) < 0)
	{
		perror("\nsocket creation failed\n");
		exit(1);
    	}
    	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    	server_addr.sin_port = htons(5000);
    	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
	{
		perror("\naddress can't be binded\n");
		exit(1);
    	}
	if(listen(sockfd,8) < 0) //listening for connections
	{
	    	perror("socket listening error\n");
	    	exit(1);
	}
    	client_len = sizeof(client_addr);
    	acc = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
	for(int i=0; i<10; i++)
	{
		len = read(acc, string, 100);		  
		if (len < 0)
		{
			printf("\nReceiving Read failed\n");
		}
		string[len] = 0;
		clock_gettime(CLOCK_REALTIME, &currtime);
		fptr = fopen("socketlog.txt", "a");
		temp=getpid();
        	fprintf(fptr,"PID=%d\tcurrtime is %ld microseconds value which is received via the client %s\n",temp,currtime.tv_nsec/(1000), string);
        	fclose(fptr);
		printf("%s\n", string);
	}
	for (i=0;i<10;i++)
		{
			getrand = rand()%(10);
			if(getrand==0)
			{	
				object.string = stringarray[getrand];
				
				
			}
			else if(getrand==1)
			{
				object.led = on_led;
				object.string=stringarray[getrand];
				
	 		}
	 		else if(getrand==2)
	 		{
				object.led = off_led;
				object.string =stringarray[getrand];
				
	 		}
	 		else if(getrand==3)
	 		{				
				object.string =stringarray[getrand];
				
				
	  		}
	 		else if(getrand==4)
	  		{
				object.led = on_led;
				object.string=stringarray[getrand];
				
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
        sleep(1);
	clock_gettime(CLOCK_REALTIME, &currtime);
	fptr = fopen("socketlog.txt", "a");
	temp=getpid();
        fprintf(fptr,"PID=%d\tcurrtime is %ld microseconds value which is being sent to client via server %s\n",temp,currtime.tv_nsec/(1000), object.string);
        fclose(fptr);
	write(acc, object.string, 10);
	usleep(1e4);
    }

}

