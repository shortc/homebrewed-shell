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

/*In this function we use the strtok() function
 to parse each string and check for a forward slash*/
int checkfortoken(char** argv)
{ 
     int i = 0;

     int count = 0;
     char tok[200];
     strcpy(tok, argv[optind]);
     char *file_ptr = strtok(tok, "/");
     char *array[200];

     while (file_ptr != NULL)
     {
         array[i++] = file_ptr;
         file_ptr = strtok (NULL, "/");
         count ++;
     }
  
     if (count == 0)             //we have a single forward slash
     {
         return 1;
     }
     else if (count > 1)         //we have multiple forward slashes and multiple files
     {
         return 2;
     }

     else
     {
        return 0;                //we have nothing
     }     
}

/*In this functin, we check each of the
files in a directory to make sure there
are no other directories with the same 
name of the one we are trying to add.*/
int file_parse(int argc, char** argv, char* dirname, DIR *current_dir, struct dirent *next_file)
{
  while (next_file != NULL)
    {
        if(strcmp(next_file->d_name, argv[optind])== 0 && next_file->d_type == DT_DIR)               //comparing all file names & types
        {
                  return 1;
        }

        next_file = readdir(current_dir);
    }
    return 0;
}

/*Function for no passed options*/
int orphan(int argc, char** argv, char* dirname, DIR *current_dir, struct dirent *next_file)
{ int tok_flag = 0; 
  int file_flag = 0;

  if (optind < argc)
  {
      while (optind < argc)
      {
          tok_flag = checkfortoken(argv);                                                    //flag for finding a /
          file_flag = file_parse(argc, argv, dirname, current_dir, next_file);               //flag for finding a duplicate  
          
          if (tok_flag == 1)                                                                 //if we have one /
          { 
              printf("mkdir: cannot create directory ‘/’: File exists\n");
          }
          else if (tok_flag == 2)                                                            //if we have several /'s
          {
              printf("mkdir: cannot create directory '%s': No such file or directory\n",argv[optind]);
          }
          else if (*argv[optind]== 47)                                                       //if our input starts with a /
          {
              printf("mkdir: cannot create directory '%s': No such file or directory\n",argv[optind]);
          }
 
          else if (strcmp(argv[optind], argv[optind-1]) == 0 | file_flag == 1)               //if we found a duplicate
          { 
              printf("mkdir: cannot create directory '%s': File exists\n", argv[optind]);
          }
          
          else
          {
              mkdir(argv[optind], S_IRWXU | S_IRWXG | S_IRWXO);                              //make the directory
          }
        optind++;
      }
  } 
  else
  {
      printf("mkdir: missing operand\n");                                                    //if we received no input
      printf("Try 'mkdir --help' for more information.\n");
  }

    return 0;
}


/*Function for the verbose option*/
int loud(int argc, char** argv, char* dirname,DIR *current_dir, struct dirent *next_file)
{
    int tok_flag = 0;
    int file_flag = 0;
    if (optind < argc)
    {
        while (optind < argc)
        {
            tok_flag = checkfortoken(argv);                                                  //check for forward /
            file_flag = file_parse(argc, argv, dirname, current_dir, next_file);             //check for duplicate

        //    printf("%d\n", tok_flag);
            if (tok_flag == 1)                                                              //if we have a single /
            {
                printf("mkdir: cannot create directory ‘/’: File exists\n");
            }
            else if (tok_flag == 2)                                                         //if we have multiple /'s
            {
                printf("mkdir: cannot create directory '%s': No such file or directory\n",argv[optind]);
            }
            else if (*argv[optind]== 47)                                                    //if our input starts with a /
            { 
             printf("mkdir: cannot create directory '%s': No such file or directory\n",argv[optind]);
            }

            else if (strcmp(argv[optind], argv[optind-1]) == 0 | file_flag == 1)             //if we found a duplicate
            {
                printf("mkdir: cannot create directory '%s': File exists\n", argv[optind]);
            }

            else
            {
                mkdir(argv[optind], S_IRWXU | S_IRWXG | S_IRWXO);                            //make the directory
                printf("mkdir: created directory '%s' \n", argv[optind]);                    //print out make  message
            }
            optind++;
        }
    }
    else
    {
        printf("mkdir: missing operand\n");
        printf("Try 'mkdir --help' for more information.\n");                                //if we received no input

    }
    
  return 0;
}


/*Function for the parent option*/
int parent(int argc, char**argv, char* dirname, DIR *current_dir, struct dirent *next_file)
{
    int file_flag = 0;
    if (optind < argc)
    {
        file_flag = file_parse(argc, argv, dirname, current_dir, next_file);                 //checking for duplicates

        if(*argv[optind] == 47)                                                              //checking for a starting /
        {
            printf("mkdir: cannot create directory ‘%s’: File exists\n", argv[optind]);
        }
 
        else if(file_flag == 1)                                                              //if we have a duplicate
        {
            printf("mkdir: cannot create directory '%s': File exists\n", argv[optind]);
        }

        else
        {
            int j = 0;
            int i = 0;
            char str[256];
            strcpy(str, argv[optind]);
            char *file_ptr = strtok(str, "/");                                               //parse through the file path we have been given an separate on the /
            char *array[5];

            while (file_ptr != NULL)
            {
                array[i++] = file_ptr;                                                       //put all directory names into a single array
                file_ptr = strtok (NULL, "/");
            }
            char* OG_dir = dirname;                                                          //combine the names to build a full file path
            char slash[] = "/";
            DIR *current_dir;

            while (j < i)
            {
  
                mkdir(array[j], S_IRWXU | S_IRWXG | S_IRWXO);                                //build the directory
                strcat(dirname, slash);     
                strcat(dirname, array[j]);                                                   //updaate the file path with the newest directory
                chdir(dirname);

                current_dir = opendir(dirname);                                              //going one directory down in theory
                j++;
            }
        }
    }

    else
    {
        printf("mkdir: missing operand\n");
        printf("Try 'mkdir --help' for more information.\n");
    }
    return 0;
}

/*Function for both the parent and verbose options*/
int duo(int argc, char**argv, char* dirname, DIR *current_dir, struct dirent *next_file)
{
    int file_flag = 0; 
    int silent_flag = 0;
    if (optind < argc)
    {
        file_flag = file_parse(argc, argv, dirname, current_dir, next_file);                 //checking for duplicates

        if(*argv[optind] == 47)                                                              //if our input starts with a /
        {
            silent_flag = 1;
            printf("mkdir: cannot create directory ‘%s’: File exists\n", argv[optind]);
        }

        else if(file_flag == 1)                                                              //if we have found a duplicate
        {
            silent_flag = 1;
            printf("mkdir: cannot create directory '%s': File exists\n", argv[optind]);
        }
        else
        {
            int j = 0;
            int i = 0;
            char str[256];
            strcpy(str, argv[optind]);
            char *file_ptr = strtok(str, "/");                                                //parse through the file path we have been given an separate on the /
            char *array[5];

            while (file_ptr != NULL)
            {
                array[i++] = file_ptr;                                                       //put all input into its own array
                file_ptr = strtok (NULL, "/");
            }
            char* OG_dir = dirname;                                                         //build a file path using the input name
            char slash[] = "/";
            DIR *current_dir;

            while (j < i)
            {
                mkdir(array[j], S_IRWXU | S_IRWXG | S_IRWXO);                                //build the directory
                printf("mkdir: created directory '%s' \n", array[j]);
                strcat(dirname, slash);
                strcat(dirname, array[j]);                                                   //update the file path
                chdir(dirname);
                current_dir = opendir(dirname);                                             //going one directory down in theory 
                j++;
            }
        }
      
    }
    else
    {
        printf("mkdir: missing operand\n");
        printf("Try 'mkdir --help' for more information.\n");
    }
    return 0;
}


int main(int argc, char** argv)
{
    int command_flag[2];                                                                  //argument flag
    int command_index = 0;
    int sarg;
    char* dirname = getenv("PWD");                                                        //find the directories pathname
    DIR *current_dir = opendir(dirname);                                                  //opening our current directory
    struct dirent *next_file = readdir(current_dir);
    

    	static struct option full_arg[] =                                                 //building a struct to house all of the written out command args
    	{
    	    {"parents", 0, 0, 'p'},
    	    {"verbose", 0, 0, 'v'},
	    {NULL, 0, NULL, 0}
	};

        while (1)
        {
	sarg = getopt_long(argc, argv, "pv", full_arg, &command_index);
	    if (sarg == -1)                                                               //if there are no/no more arguments
	    {
                break;
            }

	    switch (sarg)
	    {
		case 0:
		    break;

		case 'p':                                                                 //-p --parent
        	    command_flag[0] = 1;
            	    break;

		case 'v':                                                                //-v --verbose
            	    command_flag[1] = 1;
            	    break;

                default:
                    printf("mkdir: unrecognizable option");
                    break;
	    }
        }  

/*Determine which function we call based on our input options*/
    if(command_flag[0] == 0 && command_flag[1] == 0)
    {
      orphan(argc, argv, dirname, current_dir, next_file);            //no option
    }

    if(command_flag[0] == 1 && command_flag[1] == 0)                  //parent
    {
      parent(argc, argv, dirname, current_dir, next_file);
    }

    if (command_flag[0] == 0 && command_flag[1] == 1)                //verbose
    {
       loud(argc,  argv, dirname, current_dir, next_file);
    }

    if (command_flag[0] == 1 && command_flag[1] == 1)                 //parent and verbose
    { 
       duo(argc, argv, dirname, current_dir, next_file);
    }

    return 0;
}
