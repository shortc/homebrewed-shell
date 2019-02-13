#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include<string.h>
	

int main(int argc, char** argv)
{
	
	int command_flag[2];                                 //argument flag	
	int command_index = 0;
        int no_of_files = 0;
        int sarg;

	char* dirname = getenv("PWD");                              //find the directories pathname 
	DIR *current_dir = opendir(dirname);                        //opening our current directory (this will be our default)
	struct dirent *dir_to_list  = readdir(current_dir);         //next file in directory, will tell us if dir already exists?
				
	while (1)
	{
		static struct option full_arg[] =                   //building a struct to house all of the written out command args
		{
			{"parent", 0, 0, 'p'},
			{"verbose", 0, 0, 'v'},
			{NULL, 0, NULL, 0}
		};

		sarg = getopt_long(argc, argv, "p:v", full_arg, &command_flag);

		if (sarg == -1)                                    //if there are no/no more arguments 
		{
			break;
		}
	
		switch (sarg)
		{
			case 0
              			if (full_arg[option_index].flag != 0)
					break;
                		printf ("option %s", full_arg[option_index].name);
                		if (optarg)
					printf (" with arg %s", optarg);
                			printf ("\n");
               				 break;                            
            		case 'p':                                  //-p --parent
            			command_flag[0] = 1;
                		puts ("option -p\n");
                		break;
            		case 'v':                                  //-v --verbose
                		command_flag[1] = 1;
                		puts ("option -v\n");
                		break;	
		}
	}
	/*Tell me if we used either argument*/
	printf("%d",command_flag[0]);
	printf("%d",command_flag[1]);
	
	   
         /*This will be used to read out the file names*/
	char file_arr[255];
	int file_count = 0;
	int curr_optind;
	if (optind < argc) 
	{
        	printf ("In theory, these will be our files: ");

        	while (optind < argc)
		{
			curr_optind = optind;
            		printf ("%s ", argv[curr_optind]);
			file_arr[file_count] = curr_optind;
			optind++; 
			file_count ++;
		}
        	putchar ('\n');
   	 }
	
/*NEW STUFF*/
    int i = 0;
    char *s = strtok(file_arr, "/");
    char *file_list[255];

    while(s != NULL)
    {
        file_list[i++] = s;
        s = strtok(NULL, "/");
    }
    for (i = 0; i < 5; ++i) 
        printf("%s\n", file_list[i]);
/*NEW STUFF*/
	
	if (command_flag[0] == 1)
	{
		print("We used the parent argument");
	}
	
	if (command_flag[1] == 1)
	{
		print("We used the verbose argument");
			
	}
	
	return 0;
}
