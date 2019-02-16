#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<getopt.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>
#include<errno.h>


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


int reverse_array(struct dirent **arr, int start, int end){
    struct dirent *temp;
    if (start >= end){
        return 0;
    }
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverse_array(arr, start+1, end-1);
    return 0;
}


int insertionSortSize(struct dirent **entries, int n, char *inputDir){
    int i, j;
    struct dirent *key;
    for (i = 1; i < n; i++){
        struct stat statbuf;
        struct stat statbufminus;

        char fullPathA[500] = "";
        char fullPathB[500] = "";
        if(inputDir[0] != 0){
            strcat(fullPathA, inputDir);
            strcat(fullPathA, "/");
        }
        strcat(fullPathA, entries[i]->d_name);
        if(inputDir[0] != 0){
            strcat(fullPathB, inputDir);
            strcat(fullPathB, "/");
        }
        strcat(fullPathB, entries[i - 1]->d_name);

        stat(fullPathA, &statbuf);
        stat(fullPathB, &statbufminus);

        key = entries[i];
        j = i - 1;

        while (j >= 0 && statbufminus.st_size > statbuf.st_size){
            entries[j + 1] = entries[j];
            j = j - 1;
            char fullPathC[500] = "";
            if(inputDir[0] != 0){
                strcat(fullPathC, inputDir);
                strcat(fullPathC, "/");
            }
            strcat(fullPathC, entries[j]->d_name);
            stat(fullPathC, &statbufminus);
        }
        entries[j + 1] = key;
    }
    return 0;
}


int insertionSortTime(struct dirent **entries, int n, char *inputDir){
    int i, j;
    struct dirent *key;
    for (i = 1; i < n; i++){
        struct stat statbuf;
        struct stat statbufminus;

        char fullPathA[500] = "";
        char fullPathB[500] = "";
        if(inputDir[0] != 0){
            strcat(fullPathA, inputDir);
            strcat(fullPathA, "/");
        }
        strcat(fullPathA, entries[i]->d_name);
        if(inputDir[0] != 0){
            strcat(fullPathB, inputDir);
            strcat(fullPathB, "/");
        }
        strcat(fullPathB, entries[i - 1]->d_name);

        stat(fullPathA, &statbuf);
        stat(fullPathB, &statbufminus);

        key = entries[i];
        j = i - 1;

        while (j >= 0 && statbufminus.st_mtime > statbuf.st_mtime){
            entries[j + 1] = entries[j];
            j = j - 1;
            char fullPathC[500] = "";
            if(inputDir[0] != 0){
                strcat(fullPathC, inputDir);
                strcat(fullPathC, "/");
            }
            strcat(fullPathC, entries[j]->d_name);
            stat(fullPathC, &statbufminus);
        }
        entries[j + 1] = key;
    }
    return 0;
}

int sortAlpha(struct dirent **entries, int n){
    struct dirent *temp;
    int i, j;
    for (i = 0; i < n ; i++){
        for (j = i + 1; j < n; j++){
            if (strcmp(entries[i]->d_name, entries[j]->d_name) > 0){
                temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }
     return 0;
   }

int groupDirsFirst(struct dirent **entries, int n){
    struct dirent *new_entries[n];
    struct dirent *dir_entries[n];
    struct dirent *file_entries[n];
    int dir_count = 0;
    int file_count = 0;
    int i, x, y, z;
    for(i = 0; i < n; i++){
        if(entries[i]->d_type == DT_DIR){
            dir_entries[dir_count] = entries[i];
            dir_count++;
        } else {
            file_entries[file_count] = entries[i];
            file_count++;
        }
    }
    for(x = 0; x < dir_count; x++){
        new_entries[x] = dir_entries[x];
    }
    for(y = 0; y < file_count; y++){
        new_entries[y + dir_count] = file_entries[y];
    }
    for(z = 0; z < n; z++){
        entries[z] = new_entries[z];
    }
    return 0;
}


int listLong(struct dirent **entries, int counter, char *inputDir) {
    int i;
    for(i = 0; i < counter; i++){
        struct stat statbuf;
        char timebuf[15];
        char fullPath[500] = "";
        if(inputDir[0] != 0){
            strcat(fullPath, inputDir);
            strcat(fullPath, "/");
        }
        strcat(fullPath, entries[i]->d_name);
        if (stat(fullPath, &statbuf) == -1){
            printf("errno %d\n", errno);
            printf("Error in stat\n");
            continue;
        }
        strftime(timebuf, 14, "%b %d %H:%M", localtime(&statbuf.st_mtime));
        printf("%-10.10s ", s_perm(statbuf.st_mode));
        printf("%3ld ", statbuf.st_nlink);
        printf("%-6s ", getpwuid(statbuf.st_uid)->pw_name);
        printf("%-1s ", getgrgid(statbuf.st_gid)->gr_name);
        printf("%*ld ", 10, statbuf.st_size);
        printf("%s ", timebuf);
        printf("%-1s\n", entries[i]->d_name);
    }
    return 0;
}


int ls(int *arrForArgs, DIR *current_dir, char *inputDir){
    int file_counter = 0;

    if (arrForArgs[1] == 1) { // -d
        if(inputDir[0] != 0){
            printf(".\n");
        } else {
            printf("%s", inputDir);
        }
        return 0;
    }

    struct dirent *current_file;
    int counter = 0;

    while((current_file = readdir(current_dir)) != NULL){
        file_counter++;
    }

    struct dirent *entries[file_counter];

    rewinddir(current_dir);

    if (arrForArgs[0] == 1){ // -a
        while ((current_file = readdir(current_dir)) != NULL){
            entries[counter] = current_file;
            counter++;
        }
        sortAlpha(entries, counter);
    } else {
        while ((current_file = readdir(current_dir)) != NULL){
            if(current_file->d_name[0] != '.'){
                entries[counter] = current_file;
                counter++;
            }
        }
        sortAlpha(entries, counter);
    }

    if (arrForArgs[5] == 1) { // -S
        insertionSortSize(entries, counter, inputDir);
        reverse_array(entries, 0, counter - 1);
    }
    if (arrForArgs[6] == 1) { // -t
        insertionSortTime(entries, counter, inputDir);
        reverse_array(entries, 0, counter - 1);
    }
    if (arrForArgs[3] == 1) { // -r
        reverse_array(entries, 0, counter - 1);
    }
    if (arrForArgs[7] == 1) { // --group-directories-first
        groupDirsFirst(entries, counter);
    }
    if (arrForArgs[2] == 1 && arrForArgs[4] != 1) { // -l
        listLong(entries, counter, inputDir);
        return 0;
    }

    if (arrForArgs[4] == 1) { // -R
        printf("%s:\n", inputDir);
        int x;
        for (x = 0; x < counter; x++) {
            if (arrForArgs[2] == 1) { // -l
                listLong(entries, counter, inputDir);
            } else {
                printf("%s\n", entries[x]->d_name);
            }
        }
        printf("\n");
        for (int x = 0; x < counter; x++) {
            if (entries[x]->d_type == DT_DIR && strcmp(entries[x]->d_name, ".") != 0 && strcmp(entries[x]->d_name, "..") != 0) {
                char recInputDir[500] = {0};
                DIR *rec_Current_dir = {NULL};
                strcat(recInputDir, inputDir);
                strcat(recInputDir, "/");
                strcat(recInputDir, entries[x]->d_name);
                rec_Current_dir = opendir(recInputDir);
                ls(arrForArgs, rec_Current_dir, recInputDir);
            }
        }
    }
    else {
        for (int x = 0; x < counter; x++) {
            printf("%s\n", entries[x]->d_name);
        }
    }

    return 0;
}

int main(int argc, char** argv){

    int arrForArgs[8] = {0};
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

        sarg = getopt_long(argc, argv, "adlrRSt", full_arg, &option_index);

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
                break;

            case 'd':
                arrForArgs[1] = 1;
                break;

            case 'l':
                arrForArgs[2] = 1;
                break;

            case 'r':
                arrForArgs[3] = 1;
                break;

            case 'R':
                arrForArgs[4] = 1;
                break;

            case 'S':
                arrForArgs[5] = 1;
                if (arrForArgs[6] == 1) {
                    arrForArgs[6] = 0;
                }
                break;

            case 't':
                arrForArgs[6] = 1;
                if (arrForArgs[5] == 1) {
                    arrForArgs[5] = 0;
                }
                break;

            case 'g':
                arrForArgs[7] = 1;
                break;

            case '?':
                return 0;
                break;

        }
    }

    char inputDir[200] = {0};
    int inputCount = 0;

    // If a directory was given then store it
    if (optind < argc) {
        while (optind < argc) {
            char inputDir[200] = {0};
            strcat(inputDir, argv[optind++]);
            inputCount++;
            printf("%s:\n", inputDir);
            current_dir = opendir(inputDir);
            ls(arrForArgs, current_dir, inputDir);
        }
        // Tests to see if file exists. If stat fails then file does not exist.
        struct stat statbuf;
        if (stat(inputDir, &statbuf) == -1){
            printf("errno %d\n", errno);
            //printf("Error in stat\n");
            return -1;
        }

        current_dir = opendir(inputDir);

    } else {
        dirname = getenv("PWD");
        current_dir = opendir(dirname);
        ls(arrForArgs, current_dir, inputDir);
    }

	return 0;
}
