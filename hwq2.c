

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdint.h>

void create_and_write_file(char* string_input)
{
    char* file_name = "sanika.txt";
    FILE *fptr;
    fptr = fopen(file_name, "w+");
    if(fptr==NULL)
    {
        printf("file creation error", file_name);
    }
    else
    {
        printf("%s\nfile has been created", file_name);
        fwrite(string_input,1,strlen(string_input),fptr);
    }
    fclose(fptr);
}
int main(int argc, char* argv[])
{ 
    int i,next=0,val=0;
    char* string_in = (char*)malloc(sizeof(80));
    if(argc==1)
    {
        printf("Enter a string\n");
        scanf("%s", string_in);
    }
    if(argc>1)
    {
        for(i=1; i<argc; i++)
        {
            while(*(argv[i]+ next)!=0)
            {
                *(string_in+val++) = *(argv[i]+next++);
            }
        }
    }
    printf("The interesting string is %s\n", string_in);
    free(string_in);
    create_and_write_file(string_in);
}
