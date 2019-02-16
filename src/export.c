#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int export(char* assignment){
	FILE *fp;
	if((fp = fopen("env_vars", "r")) == NULL){
		fp = fopen("env_vars", "w+");
		fputs (assignment, fp);
		fputs("\n", fp);
		fclose(fp);
		return 1;
	} else {
		FILE *fp_write = fopen("env_vars_temp", "w+");
		char *original = (char *)(malloc(sizeof(char) * strlen(assignment)));
		strcpy(original, assignment);
		char *left = strtok(assignment, "=");
		char *left_copy = (char *)(malloc(sizeof(char) * (strlen(left) + 1)));
		strcpy(left_copy, left);
		strcat(left_copy, "=");

		char *line = NULL;
		size_t len = 0;
		ssize_t read;
		while ((read = getline(&line, &len, fp)) != -1) {
			if(strstr(line, left_copy)){
				fputs(original, fp_write);
				fputs("\n", fp_write);
			} else {
				fputs(line, fp_write);
			}
		}
		fclose(fp);
		fclose(fp_write);
		remove("env_vars");
		rename("env_vars_temp", "env_vars");
		free(original);
		free(left_copy);
	}
	return 0;
}

int main(int argc, char** argv){
	if(argc > 2 || argc == 1){
		printf("usage: export [assignment]\n");
		return 1;
	}
	export(argv[1]);
	return 0;
}
