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

/*Function to pare through a string to search for forward slashes*/
int checkfortoken(char** argv)
{
     int count = 0;
     char tok[200];
     strcpy(tok, argv[optind]);                                     //spits the string
     char *file_ptr = strtok(tok, "/");

     while (file_ptr != NULL)
     {
         file_ptr = strtok (NULL, "/");
         count ++;                                                  //counts # of times we go through a loop
     }
   //  printf("%d\n", count);
     if (count == 0 || count > 1)                                    //if we have process any /, return a 1
     {
        return 1;
     }
  return 0;
}



/*function to remove directories with no options*/
int orphan(int argc, char** argv, char* dirname)                
{
    int flag = 0;
    int i = 0;
    char* OG_dir = strdup(dirname);
    DIR *current_dir;
    char slash[] = "/";
    struct dirent *current_file;

    if (optind < argc)
    {
        while (optind < argc)
        {
            flag = checkfortoken(argv);                            //check if we have been given a full pathname

            if(flag == 1 )                                         //if we have, then print out the message and return
            {
                printf("rmdir: failed to remove '%s': No such file or directory\n", argv[optind]);
                return 0;
            }

            strcat(dirname, slash);
            strcat(dirname, argv[optind]);                         //build out our path 
     //       printf("%s\n", dirname); 
            if(chdir(dirname) == -1 )                              //if the directory doesn't exist in out home, then raise an error
            {
                printf("rmdir: failed to remove '%s': No such file or directory\n", argv[optind]);
                break;
            }

            else
            {
                chdir(dirname);                                    //move into the new directory
                current_dir = opendir(dirname);                   

                int count = 0;
                while((current_file = readdir(current_dir)) != NULL)  ///parse through the directoy
                {
                    count++;
                }

                if (count <= 2)                                       //if we encounter no files
                {
                    rmdir(dirname);                                     //remove the directory
                }
                else
                {
                    printf("rmdir: failed to remove '%s': Directory not empty\n", argv[optind]);  //if not raise an error message that it's not empty
                }
            }
                 optind++;
                 dirname = strdup(OG_dir);
                 chdir(OG_dir);
                 current_dir = opendir(OG_dir);
                 i--;
        }
    }
    else
    {
        printf("mkdir: missing operand\n");                             //if we do not receive any operand
        printf("Try 'mkdir --help' for more information.\n");
    }
    return 0;
}

/*Function to use if we choose the verbose function*/
int loud(int argc, char** argv, char* dirname)
{
    int flag = 0;
    int i = 0;
    char* OG_dir = strdup(dirname);
    DIR *current_dir;
    char slash[] = "/";
    struct dirent *current_file;

    if (optind < argc)
    {
        while (optind < argc)
        {
            strcat(dirname, slash);
            strcat(dirname, argv[optind]);
            flag = checkfortoken(argv);                                 //check if we have any file paths

            if(flag == 1 )                                              //print errr message if we do
            {
                printf("rmdir: failed to remove '%s': No such file or directory\n", argv[optind]);
                return 0;
            }

            if(chdir(dirname) == -1 )                                   //if our directory doesn't exist, print out an error   
            {
                printf("rmdir: failed to remove '%s': No such file or directory\n", argv[optind]);
                break;
            }
            else
            {
                chdir(dirname);                                    //going down into the new directory
                current_dir = opendir(dirname);                   //opening it

                int count = 0;
                while((current_file = readdir(current_dir)) != NULL)   //parsing through the directory for any files
                {
                    count++;
                }

                if (count <= 2)                                               //if we don't have any
                {
                    rmdir(dirname);                                           //remove the directory
                    printf("rmdir: removing directory, '%s'\n",argv[optind]);
                }
                else                                                           //if we do, print that we can't
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
    }
    else
    {
        printf("mkdir: missing operand\n");                                 //print message if we are missing an operand
        printf("Try 'mkdir --help' for more information.\n");
    }
    return 0;
}



int parent(int argc, char**argv, char* dirname)
{    if (optind < argc)
     {
         int j = 0;
         int i = 0;
         char tok[256];
         char tempdirname[256];
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
           //  printf("%s", array[i]);
             file_ptr = strtok (NULL, "/");                           //use the str tok function to divide up the string
         }
         i--;
         while(i > -1)
         {
          //   printf("%d\n",i);
             for(j = 0; j <= i ; j++)
             {
                 strcat(tempdirname, dirname);                        //build a temporary file path
                 strcat(tempdirname, slash);
                 strcat(tempdirname, array[j]);
               //  printf("%s\n", tempdirname);
                 if (chdir(tempdirname) == -1)                        //if our directory doesn't exist
                 {
                     printf("rmdir: failed to remove '%s': No such file or directory\n", array[i]);  //print message and leave
                     return 0;
                 }
                 else
                 {
                     tempdirname[0] = '\0';
                     strcat(dirname, slash);
                     strcat(dirname, array[j]);
                     chdir(dirname);                               //go into the directory
                 }
             }

                 current_dir = opendir(dirname);                   //opening the directory

                 int count = 0;
                 while((current_file = readdir(current_dir)) != NULL)  //parse and check segment 
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
    }
    else
    {
        printf("mkdir: missing operand\n");
        printf("Try 'mkdir --help' for more information.\n");
    }

    return 0;
}

/*function if we use both the -p and -v options
(exactly the same as above but with a printed message 
that we deleted the directory)*/
int duo(int argc, char**argv, char* dirname)
{    if (optind < argc)
     {
         int j = 0;
         int i = 0;
         char tok[256];
         char tempdirname[256];
         strcpy(tok, argv[optind]);
         char *file_ptr = strtok(tok, "/");                                     //divide input string on / token
         char *array[256];
         char* OG_dir = strdup(dirname);
         char slash[] = "/";
         DIR *current_dir;
         struct dirent *current_file;

         while (file_ptr != NULL)
         {
             array[i++] = file_ptr;                                             //put each item into an array
           //  printf("%s", array[i]);
             file_ptr = strtok (NULL, "/");
         }
         i--;
         while(i > -1)
         {
          //   printf("%d\n",i);
             for(j = 0; j <= i ; j++)
             {
                 strcat(tempdirname, dirname);                                   //build a temporary path name
                 strcat(tempdirname, slash);
                 strcat(tempdirname, array[j]);
               //  printf("%s\n", tempdirname);
                 if (chdir(tempdirname) == -1)                                   //if the directory doesn't eist
                 {
                     printf("rmdir: failed to remove '%s': No such file or directory\n", array[j]); //error and leave
                     return 0;
                 }
                 else
                 {
                     tempdirname[0] = '\0';                               //reset the temp pathname
                     strcat(dirname, slash);
                     strcat(dirname, array[j]);
                     chdir(dirname);                                     //enter the directory
                 }
             }

                 current_dir = opendir(dirname);                   //going one directory down in theory

                 int count = 0;
                 while((current_file = readdir(current_dir)) != NULL)  //parse through to determine if empty
                 {
                     count++;
                 }

                 if (count <= 2)
                 {
                     printf("rmdir: removing directory, '%s'\n",array[i]);         //print that we're removing the directory
                     rmdir(dirname);
                 }
                 else
                 {
                     printf("rmdir: failed to remove '%s': Directory not empty\n", argv[optind]);    //error, we can't
                 }
                 optind++;
                 dirname = strdup(OG_dir);
                 chdir(OG_dir);
                 current_dir = opendir(OG_dir);
                 i--;
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
    int invalid_flag = 0;
    int command_flag[2]= {0};                                   //argument flag
    int command_index = 0;
    int sarg;


    char* dirname = getenv("PWD");                         //find the directories pathname

    while (1)
    {
    	static struct option full_arg[] =              //building a struct to house all of the written out command args
    	{
    	    {"parents", 0, 0, 'p'},
            {"verbose", 0, 0, 'v'},
	    {NULL, 0, NULL, 0}
  	};

	sarg = getopt_long(argc, argv, "pv", full_arg, &command_index);
	    if (sarg == -1)                                //if there are no/no more arguments
	        break;

	    switch (sarg)
	    {
		case 0:
         	    break;

		case 'p':                                  //-p --parent
        	    command_flag[0] = 1;
               	    break;
                case 'v':
                    command_flag[1] = 1;                   //-v --verbose

                default:
                    if(command_flag[0] != 1 && command_flag[1] != 1)
                    {
                        invalid_flag = 1;
                    }
                    break;
	    }
    }

    /*Tell me if we used either argument*/
   // printf("%d\n",command_flag[0]);
    if(invalid_flag != 1)
    {
        if(command_flag[0] == 0 && command_flag[1] == 0)
        {
            orphan(argc, argv, dirname);
        }

        if(command_flag[0] == 1 && command_flag[1] == 0)
        {
            parent(argc, argv, dirname);
        }

        if(command_flag[0] == 0 && command_flag[1] == 1)
        {
            loud(argc, argv, dirname);
        }

        if(command_flag[0] == 1 && command_flag[1] == 1)
        {
            duo(argc, argv, dirname);
        }
    }
    return 0;
}
