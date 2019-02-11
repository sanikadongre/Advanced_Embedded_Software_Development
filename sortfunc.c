#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <time.h>

int main()
{    
    int i, size =256, check; 
    time_t tcheck;
    int *buffer = (int*)malloc(sizeof(int)*length);
    int *buffer_sorted = (int*)malloc(sizeof(int)*length);
    printf("Before sorting\n");
    srand((unsigned) time(&tcheck));
    for(i=0; i < length; i++)
    {
		  buffer[i]=48 + rand()%96;
      printf("%d, ", buffer[i]);
    }
    printf("\n");	
    check = syscall(398,buffer,size,buffer_sorted);
    if(check == 0) 
    {
	    printf("After sorting the array is \n");
	    for(i = 0; i <size; i++)
	    {
		    printf("%d, ", buffer_sorted[i]);
	    }
	    printf("\n");
	    printf("sorting gets done\n");		
    }
    else 
    {
         printf("System call failed:%d \n", check);
    }
    return (0);
}