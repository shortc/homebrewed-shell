#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
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

int ls(){
	char* dirname = getenv("PWD");
	DIR *current_dir = opendir(dirname);
	int file_counter = 0;

	struct dirent *current_file;
	while((current_file = readdir(current_dir)) != NULL){
		//if(current_file->d_name[0] != '.'){
		//	printf("  %s\n", current_file->d_name);
		//}
		//if(current_file->d_type == DT_DIR){
		//	printf("IT'S A DIRECTORY: %s\n", current_file->d_name);
		//} else {
		//	printf("    %s\n", current_file->d_name);
		//}
		file_counter++;
	}
	//rewinddir(current_dir);
	//while(file_counter != 0){
		//int counter = file_counter;
		//while(counter != 0){
		//	current_file = readdir(current_dir);
		//	counter--;
	//	}
	//	printf("%s\n", current_file->d_name);
	//	rewinddir(current_dir);
	//	file_counter--;
	
	rewinddir(current_dir);
	struct dirent *entries[file_counter];
	int counter = 0;
	while(counter < file_counter){
		current_file = readdir(current_dir);
		entries[counter] = current_file;
		counter++;
	}

	if(1==1){
		reverse_array(entries, 0, file_counter - 1);
	}

	for(int i = 0; i < file_counter; i++){
    	struct stat statbuf;
		if (stat(entries[i]->d_name, &statbuf) == -1){
			printf("error\n");
			continue; 
		}
		printf("%-10.10s ", s_perm (statbuf.st_mode));
		printf("%-10d", statbuf.st_size);
		printf("%-10s\n", entries[i]->d_name);
	}
	closedir(current_dir);
}

int main(int argc, char* argv){
	ls();
	return 0;
}
