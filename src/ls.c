#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>

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

	printf("%s\n", entries[2]->d_name);
	closedir(current_dir);
}

int main(int argc, char* argv){
	ls();
	return 0;
}
