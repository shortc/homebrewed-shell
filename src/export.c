#include<stdlib.h>
#include<stdio.h>

int main(int argc, char** argv){
	FILE *fp = fopen("env_vars", "w+");
	char *path_name = argv[1];
	fputs("PATH=", fp);
	fputs(path_name, fp);
	fputs("\n", fp);
	fclose(fp);
}
