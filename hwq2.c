#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>

#define string_size (200)

void create_write_append_file(char* string_input, char* file_name)
{
    
    char* string2 = " adding output";
    FILE *fptr;
    fptr = fopen(file_name, "w");
    if(fptr==NULL)
    {
        printf("%s file creation error", file_name);
    }
    else
    {
        printf("%s\nfile has been created\n", file_name);
        fwrite(string_input,1,strlen(string_input),fptr);
    }
    fclose(fptr);
    fptr= fopen(file_name, "a");
    fwrite(string2,1,strlen(string2),fptr);
    fclose(fptr);
}

char* permission_func(char* file_name)
{
    struct stat curr_status;
    char* perm_type = malloc(11*sizeof(char));
    if(stat(file_name, &curr_status) == 0)
    {
        mode_t perm = curr_status.st_mode;
	perm_type[0] = (perm & S_IRUSR) ? 'r' : '-';
	perm_type[1] = (perm & S_IWUSR) ? 'w' : '-';
	perm_type[2] = (perm & S_IXUSR) ? 'x' : '-';
	perm_type[3] = (perm & S_IRGRP) ? 'r' : '-';
	perm_type[4] = (perm & S_IWGRP) ? 'w' : '-';
	perm_type[5] = (perm & S_IXGRP) ? 'x' : '-';
	perm_type[6] = (perm & S_IROTH) ? 'r' : '-';
	perm_type[7] = (perm & S_IWOTH) ? 'w' : '-';
	perm_type[8] = (perm & S_IXOTH) ? 'x' : '-';
	perm_type[9] = '\0';
        return perm_type;
     }
 
}

void permissionmodification(char* file_name)
{
     int select_perm,output_sel, group_sel, others_sel;
     char* perm_present;
     perm_present = permission_func(file_name);
     printf("%s is the file permission\n", perm_present);
     printf("\nTo change the file permission out of: r, w,rw,no choose them as '1', '2', '3' or '4' options respectively\n");
     scanf("%d", &output_sel);
     switch(output_sel)
     {
	case 1: select_perm = select_perm | S_IRUSR;
		  break;
	case 2: select_perm = select_perm | S_IWUSR;
		  break;
	case 3: select_perm = select_perm | S_IRUSR | S_IWUSR;
		  break;
	case 4: select_perm = select_perm | 1;
		  break;
	default: printf("\nInvalid request\n");
	          break;
      }
      printf("To change the file permission for groups to r, w, wr choose them as '1', '2', '3' or '4' options respectively\n");
      scanf("%d", &group_sel);
      switch(group_sel)
      {
	 case 1: select_perm = select_perm | S_IRGRP;
		   break;
	 case 2: select_perm = select_perm | S_IWGRP;
		   break;
	 case 3: select_perm = select_perm | S_IRGRP | S_IWGRP;
		   break;
	 case 4: select_perm = select_perm | 1;
	           break;
	 default: printf("\nInvalid request\n");
	           break;
      }
      printf("\nTo change the permission for others\n");
      scanf("%d", &others_sel);
      switch(others_sel)
      {
	  case 1: select_perm = select_perm | S_IROTH;
	            break;
	  case 2: select_perm = select_perm | S_IWOTH;
	            break;
          case 3: select_perm = select_perm | S_IROTH | S_IWOTH;
                    break;
          case 4: select_perm = select_perm | 1;

	  default: printf("\nInvalid request\n");
	            break;
	}
	chmod(file_name, select_perm);
	printf("\nAfter changing the permission is now %s\n", permission_func(file_name));
	free(file_name);
        
}

void flushfileoutput(char* file_name)
{
	FILE *fptr;
        fptr = fopen(file_name, "a");
        if(fptr==NULL)
        {
		printf("%s file is not present/found", file_name);
	}
	else
	{
		fprintf(fptr, " HI I am sanika");
		fflush(fptr);
		printf("File output flush complete\n");
	}
      
}

int main(int argc, char* argv[])
{ 
    int i,next=0,val=0;
    int operation;
    char* file_name = "sanika.txt";
    FILE *fptr = fopen(file_name,"r");
    uint8_t char_read;
    char* string_in = (char*)malloc((string_size));
    while(1)
    {
    if(argc==1)
    {
        printf("\nEnter a string\n");
        scanf("%s", string_in);
	*(string_in+strlen(string_in)) = 32;
    }
    if(argc>1)
    {
        for(i=1; i<argc; i++)
        {
            while(*(argv[i]+ next)!=0)
            {
                *(string_in+val++) = *(argv[i]+next++);
            }
	    *(string_in+val++) = 32;
        }
    }
    printf("\nThe interesting string is %s\n", string_in);
    create_write_append_file(string_in, file_name);
    printf("\nEnter the operation to be performed: 1.Modify permission of file, 2.flush file, 3.reading character, 4.reading string\n");
    scanf("%d", &operation);
    switch(operation)
    {
         case 1: permissionmodification(file_name);
	          break;
	 case 2: flushfileoutput(file_name);
		  break;
         case 3: char_read=fgetc(fptr);
		 printf("The character read is %c", char_read);
                  break;
         case 4: fgets(string_in, string_size, fptr);
		 printf("%s is string output", string_in);
                  break;    
	 default: printf("\nInvalid request\n");
	          break;
    }
     
   }
   free(string_in);

}
