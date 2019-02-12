#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/stat.h>
#include<sys/types.h>
	

int main(int argc, char** argv)
{
	
	
	static int verbose_flag = 0;                                 //argument flags
	static int parent_flag = 0;		
        int no_of_files = 0;
        int sarg;

	char* dirname = getenv("PWD");                              //find the directories pathname 
	DIR *current_dir = opendir(dirname);                        //opening our current directory (this will be our default)
	struct dirent *dir_to_list  = readdir(current_dir);         //next file in directory, will tell us if dir already exists?
				
	while (1)
	{
		static struct option full_arg[] =                   //building a struct to house all of the written out command args
		{
			{"parent", 0, 0, 'p'}
			{"verbose", 0, 0, 'v'},
			{NULL, 0, NULL, 0}
		};

		sarg = getopt_long(argc, argv, "p:v", full_arg, &parent_flag, &verbose_flag);

		if (sarg == -1)                                    //if there are no/no more arguments 
		{
			break;
		}
	
		switch (sarg)
		{
			case 0:
				break;                             
            		case 'p':                                  //-p --parent
            			parent_flag = 1;
                		puts ("option -p\n");
                		break;
            		case 'v':                                  //-v --verbose
                		verbose_flag = 1;
                		puts ("option -v\n");
                		break;	
		}
	}
	printf("%d",parent_flag);
	printf("%d",verbose_flag);
	
	if (parent_flag == 1)
	{
		print("We used the parent argument")
	}
	
	if (verbose_flag == 1)
	{
		print("We used the verbose argument")
	}
	
	return 0;
}
