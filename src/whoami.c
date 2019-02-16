// whoami implementation

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

int whoami(){
	int uid = getuid();
	printf("%s\n", getpwuid(uid)->pw_name);
	return EXIT_SUCCESS;
}

int main(int argc, char** argv){
	if(argc > 1){
		printf("usage: whoami\n");
		return 1;
	}
	whoami();
	return 0;
}
