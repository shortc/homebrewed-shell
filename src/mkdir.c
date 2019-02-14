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

int parent(int argc, char**argv, char* dirname)
{
  printf("We used only the parent argument\n");
        int j = 0;
        int i = 0;
        char str[40];
        strcpy(str, argv[optind]);
        char *file_ptr = strtok(str, "/");
        char *array[5];

        while (file_ptr != NULL)
        {
            array[i++] = file_ptr;
            file_ptr = strtok (NULL, "/");
        }
        char* OG_dir = dirname;
        char slash[] = "/";
        DIR *current_dir;

        while (j < i)
          {
            printf("We are in the loop.\n");
            mkdir(array[j], S_IRWXU | S_IRWXG | S_IRWXO);
        //    printf("Hopefully we made some directories\n");

            strcat(dirname, slash);
            printf("%s\n", dirname);
            strcat(dirname, array[j]);
            printf("%s\n", dirname);

            chdir(dirname);

            current_dir = opendir(dirname);                   //going one directory down in theory
            j++;
          }
    return 0;
}


int orphan(int argc, char** argv)
{
  printf("We have not used any argument\n");
  if (optind < argc)
      printf("In theory, these will be our files: \n");
      while (optind < argc)
      {
          printf ("%s\n", argv[optind]);
          mkdir(argv[optind], S_IRWXU | S_IRWXG | S_IRWXO);
          optind++;
      }
    return 0;
}



int loud(int argc, char** argv)
{
  printf("We used only the verbose argument\n");

    if (optind < argc)
      printf("In theory, these will be our files: \n");
    while (optind < argc)
    {
      printf ("%s\n", argv[optind]);
      mkdir(argv[optind], S_IRWXU | S_IRWXG | S_IRWXO);
      printf("mkdir: created directory '%s' \n", argv[optind]);
      optind++;
    }
  return 0;
}



int main(int argc, char** argv)
{
    int command_flag[2];                                   //argument flag
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
                    puts ("option -p\n");
                    break;

                case 'v':                                  //-v --verbose
                    command_flag[1] = 1;
                    puts ("option -v\n");
                    break;
            }
    }

    /*Tell me if we used either argument*/
    printf("%d\n",command_flag[0]);
    printf("%d\n",command_flag[1]);

    if(command_flag[0] == 0 && command_flag[1] == 0)
    {
      orphan(argc, argv);
    }

    if(command_flag[0] == 1 && command_flag[1] == 0)
    {
      parent(argc, argv, dirname);
    }

    if (command_flag[0] == 0 && command_flag[1] == 1)
    {
       loud(argc,  argv);
    }

    return 0;
}
                                        	    
