#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<getopt.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

char const * s_perm(mode_t mode) {
    static char local_buff[16] = {0};
	int i = 0;
	//special file designations
	if(S_ISREG(mode)) local_buff[i] = '-';
	else if(S_ISDIR(mode)) local_buff[i] = 'd';
	i++;
	//user permissions
	if ((mode & S_IRUSR) == S_IRUSR) local_buff[i] = 'r';
	else local_buff[i] = '-';
	i++;
	if ((mode & S_IWUSR) == S_IWUSR) local_buff[i] = 'w';
	else local_buff[i] = '-';
	i++;
	if ((mode & S_IXUSR) == S_IXUSR) local_buff[i] = 'x';
	else local_buff[i] = '-';
	i++;
 	//group permissions
	if ((mode & S_IRGRP) == S_IRGRP) local_buff[i] = 'r';
	else local_buff[i] = '-';
	i++;
	if ((mode & S_IWGRP) == S_IWGRP) local_buff[i] = 'w';
	else local_buff[i] = '-';
	i++;
	if ((mode & S_IXGRP) == S_IXGRP) local_buff[i] = 'x';
	else local_buff[i] = '-';
	i++;
	// other permissions
	if ((mode & S_IROTH) == S_IROTH) local_buff[i] = 'r';
	else local_buff[i] = '-';
	i++;
	if ((mode & S_IWOTH) == S_IWOTH) local_buff[i] = 'w';
	else local_buff[i] = '-';
	i++;
	if ((mode & S_IXOTH) == S_IXOTH) local_buff[i] = 'x';
	else local_buff[i] = '-';
	return local_buff;
}

void reverse_array(struct dirent **arr, int start, int end){
	struct dirent *temp;
   	if (start >= end){
		return;
	}
	temp = arr[start];
	arr[start] = arr[end];
	arr[end] = temp;
	reverse_array(arr, start+1, end-1);
}

int main(int argc, char** argv){

    int *arrForArgs[8] = {0};
    int file_counter = 0;

    char* dirname;
	DIR *current_dir;
    int sarg;

    while (1) {

        static struct option full_arg[] =   //building a struct to house all of the written out command args
    	{
        	{"all",                        0, 0, 'a'},
    	    {"directory",                  0, 0, 'd'},
    	    {"group-directories-first",    0, 0, 'g'},
	        {"reverse",                    0, 0, 'r'},
    	    {"recursive",                  0, 0, 'R'},
    	    {NULL, 0, NULL, 0}
    	};

        int option_index = 0;

        sarg = getopt_long(argc, argv, "adlrRSt:", full_arg, &option_index);

        if (sarg == -1) {
            break;
        }

        switch (sarg)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (full_arg[option_index].flag != 0)
                    break;
                printf ("option %s", full_arg[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'a':
                arrForArgs[0] = 1;
                puts ("option -a\n");
                break;

            case 'd':
                arrForArgs[1] = 1;
                puts ("option -d\n");
                break;

            case 'l':
                arrForArgs[2] = 1;
                puts ("option -l\n");
                break;

            case 'r':
                arrForArgs[3] = 1;
                puts ("option -r\n");
                break;

            case 'R':
                arrForArgs[4] = 1;
                puts ("option -R\n");
                break;

            case 'S':
                arrForArgs[5] = 1;
                puts ("option -S\n");
                break;

            case 't':
                arrForArgs[6] = 1;
                puts ("option -t\n");
                break;

            case 'g':
                arrForArgs[7] = 1;
                puts ("option -g\n");
                break;

            case '?':
                break;

        }
    }

    printf("%d",arrForArgs[0]);
    printf("%d",arrForArgs[1]);
    printf("%d",arrForArgs[2]);
    printf("%d",arrForArgs[3]);
    printf("%d",arrForArgs[4]);
    printf("%d",arrForArgs[5]);
    printf("%d",arrForArgs[6]);
    printf("%d\n",arrForArgs[7]);


    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        putchar ('\n');
    }

    if (arrForArgs[1] == 1) { // -d
        printf(".\n");
        return 0;
    }

    dirname = getenv("PWD");
    current_dir = opendir(dirname);

    struct dirent *current_file;

    rewinddir(current_dir);
    struct dirent *entries[file_counter];
    int counter = 0;
    while(counter < file_counter){
        current_file = readdir(current_dir);
        entries[counter] = current_file;
        counter++;
    }

    if (arrForArgs[0] == 1){ // -a
        struct dirent *current_file;
        while ((current_file = readdir(current_dir)) != NULL){
            printf("%s  \n", current_file->d_name);
        }
    } else {
        struct dirent *current_file;
        while ((current_file = readdir(current_dir)) != NULL){
            printf("%s  \n", current_file->d_name);
        }
    }

    if (arrForArgs[2] == 1) { // -l

    } else if (arrForArgs[3] == 1) { // -r

    } else if (arrForArgs[5] == 1) { // -S

    } else if (arrForArgs[6] == 1) { // -t

    } else if (arrForArgs[7] == 1) { // --group-directories-first

    } else if (arrForArgs[4] == 1) { // -R

    }
	return 0;
}
