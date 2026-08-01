/*
Name:       Patrick Casey
BlazerID:   pccasey
Project #:  3
to compile: $ gcc -Wall pccasey_HW3.c -o HW3
or          $ make
to run:     $ ./HW3 <directory>
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>


int read_txt(const char *name)
{
    int count = 0;
    int is_char = 0;
    int space;
    FILE *file = fopen(name, "r");
    if (file == NULL) //this checks if the file opened correctly or not
    {
        if (errno == EACCES) //checks read permissions
        {
            fprintf(stderr, "Error: You do not have permission to read this file.\n");
        } 
        else if (errno == ENOENT) //if the file does not exist
        {
            fprintf(stderr, "Error: The file does not exist.\n");
        } 
        else //if it failed to open for some other reason
        {
            fprintf(stderr, "File open failed: %s\n", strerror(errno));
        }
        return -1;
    }
    while ((space = fgetc(file)) != EOF) //this iterates through each character till end of file
    {
        if(space == ' ' || space == '\t' || space == '\n') //checks for each type of space
        { //this reads it as a whitespace
            is_char = 0;
        }
        else if (!is_char) //this reads as a new word and adds a count
        {
            is_char = 1;
            count++;
        }
    }

    fclose(file);
    return count;
}



int main(int argc, char *argv[])
{
    if (argc < 2) //checks for directory argument
    {
        printf("Usage: ./HW3 <directory_name>\n");
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
        char fullPath[4096]; //holds directory path
        snprintf(fullPath, sizeof(fullPath), "%s/%s", start_dir, entry->d_name);
        //entry->d_name gets the file name

        struct stat st;
        stat(fullPath, &st);
        if (S_ISDIR(st.st_mode)) //skips subdirectories
        {
            continue;
        }
        pid_t pid = fork();

        if (pid < 0) //catches if fork fails
        {
            perror("Fork failed");
            closedir(dir);
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) //child proccess
        {
            //this will contain the meat of the program. Print file name, file size in bytes, 
            //how many words the file contains for .txt files

            if(strstr(entry->d_name, ".txt") != NULL) //checks for .txt files
            {
                //call to a seperate method for word count
                int words = read_txt(fullPath);
                printf("File: %s | Size: %ld | Words: %d\n", entry->d_name, (long)st.st_size, words); //adds word count for .txt files
            }
            else
            {
                printf("File: %s | Size: %ld\n", entry->d_name, (long)st.st_size); //no word count for non .txt files
            }
            exit(0);
        }
        else //parent process
        {
            int status;
            waitpid(pid, &status, 0); //waits for child processes to end
        }

    }
    closedir(dir);
    return 0;
}