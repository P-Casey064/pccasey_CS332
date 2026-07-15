#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Step 1: open the input file for reading */
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen (input file)");
        exit(EXIT_FAILURE);
    }

    /* Open the log file for writing (truncate/create) */
    FILE *output = fopen("output.log", "w");
    if (output == NULL) {
        perror("fopen (output.log)");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE]; //this is a line in <filename>
//
    
    while (fgets(line, sizeof(line), file) != NULL) {
 
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
 
        if (strlen(line) == 0) {
            continue;
        }
        char command_copy[MAX_LINE];
        strncpy(command_copy, line, sizeof(command_copy) - 1); //strncpy(var to hold info, info, size of info)
        command_copy[sizeof(command_copy) - 1] = '\0'; //removes '\n'

        char *args[MAX_ARGS];
        int temp = 0;
 
        char *token = strtok(line, " \t"); //reads each word. ex: ls -l. it first gets ls
        while (token != NULL && temp < MAX_ARGS - 1) { //loops through the argument getting each token
            args[temp++] = token;
            token = strtok(NULL, " \t"); 
        }
        args[temp] = NULL;
 
        if (temp == 0) { //if there was only a blank line
            continue; 
        }


        time_t start_time = time(NULL); //gets the time before fork

        pid_t pid = fork();
 
        if (pid < 0) { //error catcher
            perror("fork");
            continue;
        } else if (pid == 0) { //this is where the child process reads and executes
            execvp(args[0], args);//child process reads the command
 
            fprintf(stderr, "execvp failed for '%s': %s\n", args[0], strerror(errno)); //only runs if execvp() failed
            _exit(EXIT_FAILURE);
        } else { //parent process
            int status;
            waitpid(pid, &status, 0); //lets the parent process know the child process is done
            time_t end_time = time(NULL);

            char start_str[64];
            char end_str[64];
            //formatting
            strncpy(start_str, ctime(&start_time), sizeof(start_str) - 1);
            start_str[sizeof(start_str) - 1] = '\0';
            start_str[strcspn(start_str, "\n")] = '\0';
 
            strncpy(end_str, ctime(&end_time), sizeof(end_str) - 1);
            end_str[sizeof(end_str) - 1] = '\0';
            end_str[strcspn(end_str, "\n")] = '\0';
            //output to output.txt
            fprintf(output, "%s\t%s\t%s\n", command_copy, start_str, end_str);

        }


    
    }
    fclose(file);
    fclose(output);

    return 0;
}