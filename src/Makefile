CC=gcc

all: wanna_be_bash.c ls.c export.c whoami.c
	mkdir -p ../bin
	$(CC) -o ../bin/wanna_be_bash wanna_be_bash.c
	$(CC) -o ../bin/ls ls.c
	$(CC) -o ../bin/export export.c
	$(CC) -o ../bin/whoami whoami.c
	$(CC) -o ../bin/rmdir rmdir.c
	$(CC) -o ../bin/mkdir mkdir.c

clean: 
	rm -r ../bin
