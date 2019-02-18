#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, uint8_t* argv[])
{
	uint8_t i=0,next=0; 
        uint8_t val=0;
        uint8_t char_read=0;
        uint8_t* check = (uint8_t*)malloc(100);
	uint8_t* string_in = (uint8_t*)malloc(100);
	uint8_t *file_name,*string2;
        FILE* fptr;
	for(i=1;i<argc;i++)
	{
		next=0;
		while(*(argv[i]+next)!=0)	
		{
			*(string_in+val++)=*(argv[i]+next++);
		}	
		*(string_in+val++)=32;
	}
        string2="Hello this is me";
        file_name= "sanika.txt";
        fptr = fopen(file_name, "w");
        fwrite(string_in, 1, strlen(string_in), fptr);
        fclose(fptr);
        sprintf(check,"chmod %d %s",222,file_name);
	system(check);
	free(check);	
        fptr=fopen(file_name, "a");
	fwrite(string2, 1, strlen(string2), fptr);
	fclose(fptr);
        sprintf(check,"chmod %d %s",444,file_name);
	system(check);
	free(check);
	fptr=fopen(file_name, "r");
	char_read=(uint8_t)fgetc(fptr);
	printf("output char is = %c\n",char_read);	
	string_in=fgets(string_in,100,fptr);
	printf("output string is = %s\n",string_in);
	sprintf(check,"chmod %d %s",666,file_name);
	system(check);
	free(check);
        fprintf(fptr, "yolo");
	fflush(fptr);
        printf("flush complete\n");
	fclose(fptr);
	free(string_in);
}
