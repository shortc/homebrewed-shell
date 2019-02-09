#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<getopt.h>

int ls(){
	char* dirname = getenv("PWD");
	DIR *current_dir = opendir(dirname);
	struct dirent *dir_to_list  = readdir(current_dir);

	struct dirent *current_file;
	while ((current_file = readdir(current_dir)) != NULL){
		printf("  %s\n", current_file->d_name);
	}
}

int main(int argc, char* argv){
	
	static struct option full_arg[] =   //building a struct to house all of the written out command args 
	{
    	    {"all", required_argument, NULL, 'a'},
    	    {"directory", required_argument, NULL, 'd'},
	    {"group-directories-first", required_arugment, NULL, 'g'}
	    {"reverse", required_argument, NULL, 'r'}
	    {"recursive", required_argument, NULL, 'R'}
    	    {NULL, 0, NULL, 0}
	};
	
	while ((sarg = getopt (argc, argv, "a:d:l:r:R:S:t:full_arg")) != -1) {
		switch (sarg) 
		    {
		    case 'a':
		    break;
		    
                    case 'd':
		    break;
			
		    case 'l':
	            break;
			
		    case 'r':
		    break;
				
		    case 'R':
		    break;
				
		    case 'S':
		    break;
				
		    case 't':
		    break;
				
		    case 'g':
		    break;
		
		    case '?':
		    break;
				
		    default:
				
				
						
	
	ls();
	return 0;
}
