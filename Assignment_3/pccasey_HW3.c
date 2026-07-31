#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("No arguments given, exiting program");
        return 0;
    }
    const char *start_dir = argv[1];
    DIR *dir = opendir(start_dir);

    if (dir == NULL) //checks if the directory exists
    {
        if (errno == ENOENT) 
        {
            printf("Error: The directory '%s' does not exist.\n", start_dir);
        } else 
        {
            printf("Error opening directory: %s\n", strerror(errno));
        }
        return 1;
    }


    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            closedir(dir);
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) //child proccess
        {
            //this will contain the meat of the program. Print file name, file size in bytes, 
            //how many words the file contains for .txt files
            char fullPath[4096]; 
            snprintf(fullPath, sizeof(fullPath), "%s/%s", start_dir, entry->d_name);
            printf("%s\n", entry->d_name);
            exit(0);
        }
        else //parent process
        {
            int status;
            waitpid(pid, &status, 0);
        }

    }
    closedir(dir);
    return 0;
}