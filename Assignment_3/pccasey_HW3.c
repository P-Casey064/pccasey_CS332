#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    const char *start_dir = argv[1];
    DIR *dir = opendir(start_dir);

    if (dir == NULL) //checks if the directory exists
    {
        if (errno == ENOENT) 
        {
            printf("Error: The directory '%s' does not exist.\n", dir_path);
        } else 
        {
            printf("Error opening directory: %s\n", strerror(errno));
        }
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
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
        }

    }
}