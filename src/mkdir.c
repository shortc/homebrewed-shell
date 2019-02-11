#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/stat.h>
#include<sys/types.h>

static int verbose_flag = 0;
static int parent_flag = 0;

mkdir() //here's the start
{
  char* dirname = getenv("PWD");
	DIR *current_dir = opendir(dirname);
	struct dirent *dir_to_list  = readdir(current_dir);
		
		
	while (1) {
        static struct option full_arg[] =   //building a struct to house all of the written out command args
    	{
        {"parent", 0, 0, 'p'}
    	{"verbose", 0, 0, 'v'},
    	{NULL, 0, NULL, 0}
    	};

        sarg = getopt_long(argc, argv, "p:v", full_arg, &parent_flag, &verbose_flag);

        if (sarg == -1) 
	{
            break;
        }


        switch (sarg)
        {
            case 0:
                break;

            case 'p':
                parent_flag = 1;
                puts ("option -p\n");
                break;

            case 'v':
                verbose_flag = 1;
                puts ("option -v\n");
                break;	
			
	
	
  so...let's first check for what arguments we have.
    no args?
    
    how many files we got?
    loop for the number of files:
  
      if the directory name we plug in, already exists, then no error, it's already been made
        return 0
  
        else we take the directory and add it to the preexisting file path
          then mkdir using that file path and all access
          return 0
          
     else
       which arg?
       {parents}
        how many files we got?
        loop for each file
          
        if the directory name we plug in, already exists, then no error, it's already been made
        return 0
          
        else we take the directory and add it to the preexisting file path -1 file
          then mkdir using that file path and all access
          return 0
          
        {verbose}  
        how many files we got?
        loop for each file
          
        if the directory name we plug in, already exists, then no error, it's already been made
        print that it already exists
        return 0
  
        else we take the directory and add it to the preexisting file path file
          then mkdir using that file path and all access
          print that we did it.
          return 0
  
} 





int main(int argc, char* argv){

	mkdir();

	return 0;

}
