CC=gcc

all: wanna_be_bash.c ls.c export.c whoami.c
	mkdir -p ../bin
	$(CC) -Wall -o ../bin/wanna_be_bash wanna_be_bash.c
	$(CC) -Wall -o ../bin/ls ls.c
	$(CC) -Wall -o ../bin/export export.c
	$(CC) -Wall -o ../bin/whoami whoami.c
	$(CC) -Wall -o ../bin/rmdir rmdir.c
	$(CC) -Wall -o ../bin/mkdir mkdir.c

clean: 
	rm -r ../bin
