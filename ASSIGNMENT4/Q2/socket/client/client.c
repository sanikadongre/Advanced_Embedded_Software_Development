#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#define PORT (5000)

typedef enum
{
	off_led=0,
	on_led=1,
	bright_led=2,
	broken_led=3,
	dim_led=4
}led_enum;

int i=0;
pid_t temp;
uint32_t getrand;
struct timespec currtime;
uint8_t j,k,l; 
int len;
useconds_t usec = 10000;
struct sockaddr_in client_addr;
char *serv_host = "localhost";
struct hostent *hptr;
int buff_size = 0;
int clientfd, client_f, clilen, port;
char string[100];
int flag = 0;
FILE *fptr;
char *stringarray[5] = {"hey", "sky", "snow", "purple", "orchid"};

struct data
{
	char *string;
	int led;
}object;

void handler(int signo, siginfo_t *info, void *extra) 
{
	clock_gettime(CLOCK_REALTIME, &currtime);
	fptr = fopen("socketlog.txt", "a");
    	fprintf(fptr,"currtime : %ld Thread EXITING \n",currtime.tv_nsec/(1000));
    	fclose(fptr);
	close(clientfd);
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
		exit(1);
	} 
}
int main(int argc, char* argv[])
{

	sigfunc();
	object.led = off_led;
    	int randno = 0;
	hptr = gethostbyname(serv_host); 
	
  	if(hptr->h_addrtype !=  AF_INET) 
	{
		perror("\n address is unknown\n");
		exit(1);
	}
	bzero((char *) &client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = ((struct in_addr *)hptr->h_addr_list[0])->s_addr;
	client_addr.sin_port = htons(PORT);
	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("\nsocket opening error\n");
		exit(1);
	}
    	if(connect(clientfd, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) 		
	{
		perror("\nsocket connection error\n");
		exit(1);
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
        		for(int j=0; j<100000; j++); 
			clock_gettime(CLOCK_REALTIME, &currtime);
			fptr = fopen("socketlog.txt", "a");
			temp=getpid();
        		fprintf(fptr,"PID=%d\tcurrtime is %ld in microseconds value which is being sent from client to server %s\n",temp,currtime.tv_nsec/(1000), object.string);
        		fclose(fptr);
       			write(clientfd, object.string, 50);
			usleep(usec);
        }
	for(int i=0; i<10; i++)
	{
		len = read(clientfd, string, 100);	
		if (len < 0)
		{
			printf("\nReceiving Read failed\n");
		}
		clock_gettime(CLOCK_REALTIME, &currtime);
		fptr = fopen("socketlog.txt", "a");
        	fprintf(fptr,"PID=%d\tcurrtime is %ld in microseconds value which is being received  from  client: %s\n",temp,currtime.tv_nsec/(1000), string);
        	fclose(fptr);
		printf("%s\n", string);
	}
	close(clientfd);
}
