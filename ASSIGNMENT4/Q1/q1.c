#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>	
		
#define period 100
#define msectonsec (uint32_t)1e6
#define nsecdividedbymicrosec	(1000)	

typedef struct 
{
	FILE *fileop;
	char *fname;
}fileparams;

struct itimerspec datetime;
struct sigevent signal_event;
pthread_mutex_t pmutex;
pid_t pidmainthread;
pthread_t parent_thread;
pthread_t thread1;
pthread_t thread2;
struct timespec timerexec;	
static char cond;

void *child_thread_1(void *strptr)
{	
	fileparams* fpointer = (fileparams*)strptr;
	pid_t thread1_pid = getpid();
	pid_t thread1_tid = syscall(SYS_gettid);
	pthread_t posixid_thread1 = pthread_self();
	char capitalletter;
	uint8_t charval,i,j;
	static uint32_t arrayprint[400];
	clock_gettime(CLOCK_REALTIME,&timerexec);
	printf("\nCurrent time is %ld microseconds\n",timerexec.tv_nsec/nsecdividedbymicrosec);	
	printf("\nchild thread1 gets created with PID %d\t\n posix thread id is %ld\t\n linux thread is %d\n", thread1_pid,posixid_thread1,thread1_tid);	
	printf("Letters whose count is less than 100 are\n");
	pthread_mutex_lock(&pmutex);
	(*fpointer).fileop = fopen("gdb.txt", "r");
	if ((*fpointer).fileop == NULL)
	{
		perror("\nerror in reading the file");
		exit(1);
	}	
	while((capitalletter=fgetc((*fpointer).fileop))!=EOF)
	{
		charval = (uint8_t)tolower(capitalletter);
		if(97<=charval<=122)
		{
			i = charval-97;
			arrayprint[i]++;
		}
	}	
	(*fpointer).fileop=fopen((*fpointer).fname, "a");
	fprintf((*fpointer).fileop,"\nCurrent time is %ld microseconds\n", timerexec.tv_nsec/nsecdividedbymicrosec);
	fprintf((*fpointer).fileop,"\nchild thread1 logged in file with PID %d\t\n posix thread id is %ld\t\n linux thread is %d\n", thread1_pid,posixid_thread1,thread1_tid);	
	fprintf((*fpointer).fileop,"Letters whose count is less than 100 are\n");
	for(j=0;j<26;j++)
	{
		if (arrayprint[j]<100)
		{
			fprintf((*fpointer).fileop,"Character %c\t count is %d\n", j+97, arrayprint[j]);
			printf("Character %c\t count is %d\n", j+97, arrayprint[j]);
		}
	}
	fprintf((*fpointer).fileop,"Thread1 exiting\n");
	pthread_mutex_unlock(&pmutex);
}

void clockcond(union sigval sv)
{
	fileparams* fpointer=sv.sival_ptr;
	static char child2arr[600];	
	pid_t thread2pid = getpid();
	pid_t thread2tid = syscall(SYS_gettid);
	pthread_t posixidthread2 = pthread_self();
	clock_gettime(CLOCK_REALTIME,&timerexec);
	printf("\nCurrent time is %ld microseconds\n",timerexec.tv_nsec/nsecdividedbymicrosec);
	printf("\nchild thread2 gets created with pid %d\t\n posix thread id is %ld\t\n linux thread id is %d\n",thread2pid,posixidthread2,thread2tid);
	pthread_mutex_lock(&pmutex);
	(*fpointer).fileop = popen("cat /proc/stat | grep cpu","r");
	if ((*fpointer).fileop == NULL)
	{
		perror("error in reading the file\n");
		exit(1);
	}
	fread(child2arr,sizeof(child2arr),1,(*fpointer).fileop);
	printf("CPU Utilization is \n");
	printf("%s\n", child2arr);	
	(*fpointer).fileop=fopen((*fpointer).fname, "a");
	fprintf((*fpointer).fileop,"\nCurrent time is %ld microseconds\n", timerexec.tv_nsec/nsecdividedbymicrosec);
	fprintf((*fpointer).fileop,"child thread2 gets created with pid %d\t\n posix thread id is %ld\t\n linux thread id is %d\n",thread2pid,posixidthread2,thread2tid);
	fprintf((*fpointer).fileop,"CPU Utilization is \n");
	fprintf((*fpointer).fileop,"%s\n", child2arr);
	pthread_mutex_unlock(&pmutex);	
}

void *child_thread_2(void *strptr)
{
	fileparams* fpointer = (fileparams*)strptr;
	static timer_t countdec;
	pid_t thread2pid = getpid();
	pid_t thread2tid = syscall(SYS_gettid);
	pthread_t posixidthread2 = pthread_self();
	clock_gettime(CLOCK_REALTIME,&timerexec);
	printf("\nCurrent time is %ld microseconds\n", timerexec.tv_nsec/nsecdividedbymicrosec);
	printf("\nchild thread2 gets created with pid %d\t\n posix thread id is %ld\t\n linux thread id is %d\n",thread2pid,posixidthread2,thread2tid);
	(*fpointer).fileop=fopen((*fpointer).fname, "a");
	fprintf((*fpointer).fileop,"\nCurrent time is %ld microseconds\n", timerexec.tv_nsec/nsecdividedbymicrosec);
	fprintf((*fpointer).fileop,"\nchild thread2 gets created with pid %d\t\n posix thread id is %ld\t\n linux thread id is %d\n",thread2pid,posixidthread2,thread2tid);
	memset(&datetime, 0, sizeof(struct itimerspec));	
	memset(&signal_event, 0, sizeof(struct sigevent));
    	signal_event.sigev_notify= SIGEV_THREAD;
	signal_event.sigev_value.sival_ptr= fpointer;
    	signal_event.sigev_notify_function= &clockcond;
    	timer_create(CLOCK_REALTIME, &signal_event, &countdec);
	datetime.it_value.tv_sec= 0;
	datetime.it_value.tv_nsec = period*msectonsec;
	datetime.it_interval.tv_sec= datetime.it_value.tv_sec;
	datetime.it_interval.tv_nsec = datetime.it_value.tv_nsec;
	timer_settime(countdec, 0, &datetime, NULL);
	do
	{
		sleep(6);
	}while(1);
}

void sighandlingfunc(int signo, siginfo_t *info, void *extra)
{
	if(signo==12)
	{
		printf("Signal USR2 Exiting\n");
		cond = 2;
	}
	if(signo==10)
	{
		printf("Signal USR1 Exiting\n");
		cond = 1;
	}
	if(pthread_cancel(thread2))
	{
		printf("error in cancelling\n");
		exit(1);
	}

}

int main(int argc, char *argv[])
{	
	int32_t var=0;
	struct sigaction actsig;
	fileparams fobj;
	if(argc<2)
	{
		printf("\n<usage> ./filename <outputlogfile>\n");
		exit(1);
	}
		
	clock_gettime(CLOCK_REALTIME,&timerexec);
	printf("\nCurrent time is %ld microseconds\n", timerexec.tv_nsec/nsecdividedbymicrosec);
	fobj.fname = argv[1];
	actsig.sa_flags = SA_SIGINFO;
	actsig.sa_sigaction = &sighandlingfunc;
	var= pthread_mutex_init(&pmutex,NULL);
	if(var==1)
	{
		perror("error in initializing mutex");
		exit(1);
	}
	pidmainthread = getpid();
	(fobj).fileop	= fopen((fobj).fname, "a");
	fprintf(fobj.fileop,"\nCurrent time is %ld microseconds\n", timerexec.tv_nsec/nsecdividedbymicrosec);
	fprintf(fobj.fileop,"The main thread with pid is %d\n", pidmainthread);
	if(sigaction(SIGUSR1,&actsig,NULL))
	{
		perror("error in sigaction\n");
		exit(1);
	}
	if(sigaction(SIGUSR2,&actsig,NULL))
	{
		perror("error in sigaction\n");
		exit(1);
	}
	var= pthread_create(&thread1, NULL, child_thread_1, (void *)&fobj);
	if(var==1)
	{	
		perror("error child thread1 creation unsuccessful\n");
		exit(1);	
	}
	var= pthread_create(&thread2, NULL, child_thread_2, (void *)&fobj);
	if(var==1)
	{
		perror("error child thread2 creation unsuccessful\n");
		exit(1);		
	}
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	clock_gettime(CLOCK_REALTIME,&timerexec);
	printf("\nCurrent time is %ld microseconds\n",timerexec.tv_nsec/nsecdividedbymicrosec);
	printf("Exit\n");
	(fobj).fileop	= fopen((fobj).fname, "a");
	fprintf(fobj.fileop,"\nCurrent time is %ld microseconds\n", timerexec.tv_nsec/nsecdividedbymicrosec);
	if (cond==1)
	{
		fprintf(fobj.fileop,"Signal USR1 exiting\n");

	}
	else if (cond==2)
	{
		fprintf(fobj.fileop,"Signal USR2 exiting\n");

	}
	fprintf(fobj.fileop,"Exit\n");
	return 0;
}

