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
    const char *start_dir = ".";  //period is a mark of the start of the directory always
 
    int i = 1;
    if (strcmp(argv[i], "-S") == 0) //strcmp compares two arguments, give them a num value, and subtracts both. if the same, will = 0
    {

    }
    else if(strcmp(argv[i], "-s") == 0)
    {
        if(i + 1 >= argc) //checks the value after -s, returns if nothing there
        {
            return 1;
        }
    }
    else if(strcmp(argv[i], "-f") == 0)
    {
        if(i + 2 >= argc) //checks the values after -f, returns if nothing there
        {
            return 1;
        }
    }
}