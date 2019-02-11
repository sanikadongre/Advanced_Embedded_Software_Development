#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>

int main()
{    
   FILE* fptr;
   int i, size =256; 
   unsigned long check,check1,check2,check3,check4,check5;
   uint8_t* string_func = (uint8_t*)malloc(200);
   pid_t proc_id = getpid();
   uid_t user_id = getuid();
   time_t t= time(NULL);
   struct tm *tme_date = localtime(&t);
    time_t tcheck;
    int *buffer = (int*)malloc(sizeof(int)*size);
    int *buffer_sorted = (int*)malloc(sizeof(int)*size);
    srand((unsigned) time(&tcheck));
    printf("Before sorting\n");
    for(i=0; i <size; i++)
    {
		buffer[i]=48 + rand()%100;
      		printf("%d, ", buffer[i]);
    }
    printf("\n");	
    check = syscall(398,buffer,size,buffer_sorted);
    check1= syscall(398, NULL, size, buffer_sorted);
    if(check1!=0)
    {
	    perror("The sorted output is NULL");
    }
    check2 = syscall(398, buffer, -6748, buffer_sorted);
    if(check2!-0)
    {
	    perror("The length is less than 0");
	    
    }
    check3 = syscall(398, buffer, 200, buffer_sorted);
    if(check3!=0)
    {
	    perror("The length is less than 256");
	    
    }
    
    if(check == 0) 
    {
	    printf("After sorting the array is \n");
	    for(i=0; i<size; i++)
	    {
		    printf("%d, ", buffer_sorted[i]);
	    }
	    printf("\n");
	    printf("sorting gets done\n");	
	    sprintf(string_func, "Processid=%d\n Userid=%d\n Timeanddate =%s\n Output sorted= %d, ", proc_id, user_id, asctime(tme_date), buffer_sorted);
	    fptr = fopen("cron_log.txt","a");
	    fwrite(string_func,1,strlen(string_func),fptr);
	    for(i=0;i<size;i++)
	    {
		    sprintf(string_func,"%d ,", buffer_sorted[i]);
		    fwrite(string_func,1,strlen(string_func),fptr);
	    }
	    printf("\n\n");
	    fwrite(string_func,1,strlen(string_func),fptr);
	    free(string_func);
	    fclose(fptr);
    	}
    	else 
    	{
         	printf("System call has been failed %d\n", check);
    	}
    	return 0;
}
