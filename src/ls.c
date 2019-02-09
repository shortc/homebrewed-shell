#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>

int ls(){
	char* dirname = getenv("PWD");
	DIR *current_dir = opendir(dirname);

	struct dirent *current_file;
	while ((current_file = readdir(current_dir)) != NULL){
		//if(current_file->d_name[0] != '.'){
		//	printf("  %s\n", current_file->d_name);
		//}
		if(current_file->d_type == DT_DIR){
			printf("IT'S A DIRECTORY: %s\n", current_file->d_name);
		} else {
			printf("    %s\n", current_file->d_name);
		}
	}
	closedir(current_dir);
}

int main(int argc, char* argv){
	ls();
	return 0;
}
