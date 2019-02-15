#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>

int orphan(int argc, char** argv, char* dirname)
{
  int i = 0;
  char* OG_dir = strdup(dirname);
  DIR *current_dir;
  char slash[] = "/";
  struct dirent *current_file;

  if (optind < argc)
      while (optind < argc)
      {
        strcat(dirname, slash);
        strcat(dirname, argv[optind]);
        if(chdir(dirname) == -1 )
        {
            printf("rmdir: failed to remove '%s': No such file or directory\n", argv[optind]);
        }

        else
        {
            chdir(dirname);
            current_dir = opendir(dirname);                   //going one directory down in theory
   
            int count = 0;
            while((current_file = readdir(current_dir)) != NULL)
            {
                count++;
            }

            if (count <= 2)
            {
                rmdir(dirname);
            }
           else
            {
                printf("rmdir: failed to remove '%s': Directory not empty\n", argv[optind]);
            }
        } 
        i++;
        optind++;
        dirname = strdup(OG_dir);
        chdir(OG_dir);
        current_dir = opendir(OG_dir);
        
      }
  return 0;
}


int parent(int argc, char**argv, char* dirname)
{
     int j = 0;
     int i = 0;
     char tok[256];
     char*  temp;
     strcpy(tok, argv[optind]);
     char *file_ptr = strtok(tok, "/");
     char *array[256];
     char* OG_dir = strdup(dirname);
     char slash[] = "/";
     DIR *current_dir;
     struct dirent *current_file;

     while (file_ptr != NULL)
     {
         array[i++] = file_ptr;
         file_ptr = strtok (NULL, "/");
     }
     i--;
     while(i > -1)          
     {
         for(j = 0; j <= i ; j++)
         { 
                 strcat(dirname, slash);
                 strcat(dirname, array[j]);
                 chdir(dirname);
         }         
                 current_dir = opendir(dirname);                   //going one directory down in theory
  
                 int count = 0;
                 while((current_file = readdir(current_dir)) != NULL)
                 {
                   count++;
		 }
                 if (count <= 2)
                 {
                   rmdir(dirname);
                 }
                 else
                 {
                     printf("rmdir: failed to remove '%s': Directory not empty\n", argv[optind]);
                 }
                 optind++;
                 dirname = strdup(OG_dir);
                 chdir(OG_dir);
                 current_dir = opendir(OG_dir);
         i--;       
      }   
     return 0;
}    


int main(int argc, char** argv)
{
    int command_flag[1]= {0};                                   //argument flag
    int command_index = 0;
    int sarg;

    char* dirname = getenv("PWD");                         //find the directories pathname
    DIR *current_dir = opendir(dirname);                   //opening our current directory (this will be our default)
    struct dirent *dir_to_list  = readdir(current_dir);    //next file in directory, will tell us if dir already exists?

    while (1)
    {
    	static struct option full_arg[] =              //building a struct to house all of the written out command args
    	{
    	    {"parents", 0, 0, 'p'},
	    {NULL, 0, NULL, 0}
  	};

	sarg = getopt_long(argc, argv, "p", full_arg, &command_index);
	    if (sarg == -1)                                //if there are no/no more arguments
	        break;

	    switch (sarg)
	    {
		case 0:
		    break;

		case 'p':                                  //-p --parent
        	    command_flag[0] = 1;
               	    break;

                default:
                    printf("mkdir: unrecognizable option");
                    break;
	    }
    }

    /*Tell me if we used either argument*/
   // printf("%d\n",command_flag[0]);

    if(command_flag[0] == 0)
    {
      orphan(argc, argv, dirname);
    }

    if(command_flag[0] == 1)
    {
      parent(argc, argv, dirname);
    }

    return 0;
}
