/* Simple program to illustrate the use of fork-exec-wait pattern. 
 * This version uses execvp and command-line arguments to create a new process.
 * To Compile: gcc -Wall lab9.c -o lab9
 * To Run: ./lab9 <command> [args]
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int y = 0;

void quit_handler(int x)
{
    y = 1;
}


int main(int argc, char **argv) {
    pid_t pid;
    int status;

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }

    pid = fork();
    if (pid == 0) { /* this is child process */
        execvp(argv[1], &argv[1]);
        printf("If you see this statement then execl failed ;-(\n");
        perror("execvp");
        exit(-1);
    } 
    else if (pid > 0) { /* this is the parent process */
        printf("Wait for the child process to terminate\n");
        signal(SIGINT, SIG_IGN); 
        signal(SIGTSTP, SIG_IGN);
        signal(SIGQUIT, quit_handler);
        
        for(;;)
        {
            pid_t w = waitpid(pid, &status, WUNTRACED); /* wait for the child process to terminate */
            if (w == -1) 
            {
                break;
            }
                if (WIFEXITED(status)) { /* child process terminated normally */
                    printf("Child process exited with status = %d\n", WEXITSTATUS(status));
                    break;
                } 
                else if (WIFSTOPPED(status))
                {
                    printf("Child process stopped with status = %d\n", WIFSTOPPED(status));
                    break;
                }
                else if(WIFSIGNALED(status))
                {
                    printf("Child process was terminated by signal %d\n", WTERMSIG(status));
                    continue;
                }
                else { /* child process did not terminate normally */
                    printf("Child process did not terminate normally!\n");
                    break;
                    /* look at the man page for wait (man 2 wait) to determine
                how the child process was terminated */
                }
                
        
        break;
        }
        while (!y) 
        {
            pause();   
        }
    } else { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
    }
    
    printf("[%ld]: Exiting program .....\n", (long)getpid());

    return 0;
}