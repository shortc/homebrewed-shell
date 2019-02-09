#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>

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
	ls();
	return 0;
}
