#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
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
	char str[50];
	int len;
	int led;
}object;

struct timespec currtime;
char *stringarray[5] = {"hey", "sky", "snow", "purple", "orchid"};
pid_t mainpid;
pid_t temp;
pid_t childpid;
pthread_mutex_t pmutex;
uint32_t getrand;
int i=0;
char readdata[4] = {0};

int main(int argc, char* argv[])
{
	  if(argc<2)
	  {
		printf("\n<usage> ./name_file <outputlogfile>\n");
		exit(1);
	  }
	  //pid_t parentid = getppid();
	  int client_socket = 0, nbytes;
	  struct sockaddr_in serv_addr = {0};
	  const char* string = "Message from Client";
	  size_t sizestr;
	  //strcpy(object.str,string);
	  memcpy(object.str,string,strlen(string)+1);
	  object.len = strlen(object.str);
	  object.led = 1;

	  if((client_socket = socket(AF_INET, SOCK_STREAM, 0))<0) //creating socket
	  {
		perror("socket creation error\n");
	    	exit(1);
	  }
	  else
	  {
		printf("socket created successfully\n");
	  }
	  serv_addr.sin_family = AF_INET;
	  serv_addr.sin_port = htons(5000);
	  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	  {
		perror("\nerror");
	  }
	  if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	  {
	    	perror("socket connection failure\n");
	  }
	  /*FILE *outputfile = fopen(argv[1], "a");
	  if (outputfile != NULL)
	  {
		fprintf(outputfile, "\n data is written to pipe1\t\r and the method used is IPC \t\rparent pid number is \n");
				
	  }
	  clock_gettime(CLOCK_REALTIME,&currtime);
	  fprintf(outputfile,"\nThe operation_led is write\n currtime is %ld seconds and %ld microseconds value\n The string sent is %s\t\n, the string length is %d\t\n and the led status is %d\n", currtime.tv_sec, currtime.tv_nsec/(1000),object.str, object.len, object.led);*/
	  sizestr = sizeof(object);
	  nbytes = send(client_socket,&sizestr,sizeof(size_t), 0);
	  printf("The size of the string is %ld\n", sizestr);
	  nbytes = send(client_socket , (char*)&object , sizeof(object), 0 ); //sending data
	  if(nbytes < sizeof(object))
	  {
	    	perror("entire data is not being sent yet\n");
	  }
	  temp=getpid();
	  printf("PID=%d\tMessage sent from Client is \n string is %s\n string length is %d\n USR LED is %d\n\n",temp,
		                   object.str, object.len, object.led); //data sent*/
	  read(client_socket, readdata, 4); //reading data from server
	  printf("Message received from Server: %s\n",readdata);
	  close(client_socket); //closing the socket
	  return 0;
}
