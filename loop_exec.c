#include <unistd.h> //fork, execv
#include <stdio.h> //printf, scanf
#include <wait.h> //waitpid
#include <string.h> //strncmp
#include <errno.h> //perror, errno

int main(int argc, char* argv[])
{
  //This example builds on fork_exec.c by repeating that
  //program's functionality in an infinite loop and adding
  //user interaction.
  while (1)
  {
    char cmd[256];

    //A simple prompt. I'm sure you'll be more creative.
    printf("command> ");
    scanf("%s", cmd);

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
    if (strncmp("exit", cmd, 256) == 0)
    {
      return 0;
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
      char *new_args[] = {cmd     /* argv[0] */, 
                          NULL    /* NULL terminates the argument list */};
      
      if (execv(cmd, new_args) < 0)
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
          case EACCES:
            printf("Error. Permission denied.\n");
            break;
          default:
            perror("Error. ");
        }
        return -1;
      }
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
