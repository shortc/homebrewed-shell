#include <unistd.h> //fork, execv
#include <stdio.h> //printf, scanf
#include <stdlib.h>
#include <sys/wait.h> //waitpid
#include <string.h> //strncmp
#include <stdarg.h> //arg parse
#include <errno.h> //perror, errno
#include <pwd.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	char* PATH = "../bin:../src";
    //This example builds on fork_exec.c by repeating that
    //program's functionality in an infinite loop and adding
    //user interaction.
    while (1)
    {	
		char *cmd = malloc(256);
		if (cmd == NULL) {
			printf("No memory\n");
			return -1;
		}

        char msg[100];
        char hostname[1024];
        gethostname(hostname, 1024);

        //A simple prompt. I'm sure you'll be more creative.
        int uid = getuid();
        printf("[%s@%s]$ ", getpwuid(uid)->pw_name, hostname);
    	//printf("%s@%s:%s$ ", getpwuid(uid)->pw_name, hostname, getenv("PWD"));
        //printf("$ ");
        fgets(cmd, 256, stdin);
		if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n')){
			cmd[strlen (cmd) - 1] = '\0';
		}

        //Break the loop and terminate if the user types "exit"
        //Note that the n in strncmp is significant. It puts
        //an upper bound on how far the function will consider
        //the candidate string before terminating. Normally, a
        //NULL byte terminates comparisons and other operations,
        //but carefully crafted values can exceed expected length
        //and affect program behavior. This is a significant origin
        //of security flaws in software called "buffer overflow"
        //or "buffer overrun" vulnarabilities. Applying a bound to
        //the length of the potential comparison prevents this
        //issue.
        //printf("%s\n", cmd);
        if (strncmp("exit", cmd, 256) == 0)
        {
			remove("env_vars");
            return 0;
        }
        if (strncmp("\n", cmd, 256) == 0)
        {
            continue;
        }
		if (strncmp("reset", cmd, 256) == 0){
			remove("env_vars");
			continue;
		}

        //If we make it here, it means we have a command name other
        //than "exit" and should spawn a process. The rest of this
        //section is basically the same as fork_exec.c with some
        //additional error handling.
        int child_pid = fork();
        if (child_pid == 0)
        {
            //Note that this example does not parse the user input for
            //arguments and add them to the argument vector. Your shell
            //will have to do that.
			

            //char *new_args[] = {cmd     /* argv[0] */,
            //                  NULL    /* NULL terminates the argument list */};
			
			char *new_args[128];
			int i = 0;
			char *arg_ptr = strtok(cmd, " ");
			while(arg_ptr != NULL){
				new_args[i] = arg_ptr;
				i++;
				arg_ptr = strtok(NULL, " ");
			}
			new_args[i] = arg_ptr;
			char *env_vars[128];
			char *line = NULL;
			size_t len = 0;
			ssize_t read;
			FILE *env_vars_file;
			
			char* PWD = NULL;
			i = 0;
			if((env_vars_file = fopen("env_vars", "r")) != NULL){
				while ((read = getline(&line, &len, env_vars_file)) != -1) {
					env_vars[i] = (char *)malloc(sizeof(char) * read);
					int line_len = strlen(line);
					if(line_len > 0 && line[line_len - 1] == '\n'){
						line[line_len - 1] = '\0';
					}
					strcpy(env_vars[i], line);
					if(line_len >= 5 && strstr(line, "PATH=") != NULL){
						PATH = (char *)malloc(sizeof(char) * (read - 5));
						strcpy(PATH, line + 5);
						//printf("NEW PATH IS: %s\n", PATH);
					}
					if(line_len >= 4 && strstr(line, "PWD=") != NULL){
						PWD = (char *)malloc(sizeof(char) * (read - 4));
						strcpy(PWD, line + 4);
						//printf("NEW PWD IS: %s\n", PWD);
					}
					i++;
				}
			}
			if(PWD == NULL){
				char *parent_PWD = getenv("PWD");
				env_vars[i] = (char *)malloc(sizeof(char) * ((strlen(parent_PWD) + 4)));
				if (env_vars[i] == NULL) {
					printf("No memory\n");
					return -1;
				}
				strcpy(env_vars[i], "PWD=");
				strcat(env_vars[i], parent_PWD);
				i++;
			}

			env_vars[i] = NULL;
			if(env_vars_file != NULL){
				fclose(env_vars_file);
			}

			char *check_p = (char *)malloc(sizeof(char) * strlen(PATH));
			strcpy(check_p, PATH);
			char *check_p_tok = strtok(check_p, ":");
			char *check_com = NULL;
			while(check_p_tok != NULL){
				check_com = (char *)malloc(sizeof(char) * (strlen(check_p_tok) + strlen(new_args[0]) + 1) );
				strcpy(check_com, check_p_tok);
				strcat(check_com, "/");
				strcat(check_com, new_args[0]);
				struct stat buffer;
				if(stat(check_com, &buffer) == 0){
					break;
				} else {
					check_p_tok = strtok(NULL, ":");
					free(check_com);
				}
			}
	    free(check_p);
            if (execve(check_com, new_args, env_vars) < 0)
			{
            //If we are here, it is bevause execv failed. Switch on errno
            //using constants defined in errno.h to provide user-friendly
            //feedback.
            switch (errno)
            {
                case ENOENT:
                    printf("Error. No such file or directory.\n");
                    break;
                case EPERM:
					printf("Operation not permitted.\n");
                case EACCES:
                    printf("Error. Permission denied.\n");
                    break;
                default:
                    perror("Error. ");
            }
            return -1;
        }
		free(cmd);
		free(check_com);
    }
    else
    {
        //Just like in fork_exec.c, wait for the child process to terminate
        //before continuing. Here, continue to the next user prompt instead
        //of terminating. Consider uncommenting the debug lines below to see
        //the timing of events more explicitly.
        int status;
        //printf("Parent waiting for child process with pid %d.\n", child_pid);
        waitpid(child_pid, &status, 0);
        //printf("Child terminated.\n");
    }
  }

}
