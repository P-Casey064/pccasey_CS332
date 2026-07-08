#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

//Static so there is no changes from outside the file
static int   option_S = 0;              // -S flag
static int   option_s = 0;              // -s was given
static long  option_s_size = 0;         // -s <bytes>
static int   option_f = 0;              // whether -f was given
static char *option_f_pattern = NULL;   // -f <pattern>
static int   option_f_depth = 0;        // -f <depth>

static void print_S_info(const struct stat *st, long size_override)
{
    char perm[11];
    //mode_to_str(st->st_mode, perm);
    printf(" (%ld, %s, %s)", size_override, perm, "l"); //time_to_str(st->st_atime)
}

void display_struct(const char *path, int depth) {
  DIR *dir = opendir(path);
  if (dir == NULL) { //makes sure there is a directory
    printf("Error opening directory '%s'\n", path);
    return;
  }

    struct dirent *entry;


    while ((entry = readdir(dir)) != NULL) { //entry->d_name is a shorter way for d_name to access entry
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

        char fullPath[4096]; //full path is needed to show the whole thing. ex ./test(directory) -> ./test/test2(regular file)
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name); //this builds the path in memory, doesn't print anything
    
        struct stat st;
        if (lstat(fullPath, &st) != 0) 
        {
            printf("Error reading '%s'\n", fullPath);
            continue; // skip this entry, keep processing siblings
        }
    
        for (int i = 0; i < depth; i++) {
            printf("    ");
        }

        printf("%s", entry->d_name);

        if (option_S) {
            print_S_info(&st, (long)st.st_size);
        }
        printf("\n");

        if (S_ISDIR(st.st_mode)) {
            display_struct(fullPath, depth + 1);
        }

    }

  closedir(dir);
}

int main(int argc, char *argv[])
{
    const char *start_dir = ".";  //period is a mark of the start of the directory always
 
    int i = 1; //setting to 1 skips the initial call to the program
    while (i < argc)
    {
        if (strcmp(argv[i], "-S") == 0) //strcmp compares two arguments, give them a num value, and subtracts both. if the same, will = 0
        {
            option_S = 1;
            i++;
        }
        else if(strcmp(argv[i], "-s") == 0)
        {
            if(i + 1 >= argc) //checks the value after -s, returns if nothing there
            {
                return 1;
            }
            option_s = 1; //basically makes it known that there is an input after -s
            option_s_size = atol(argv[i + 1]); //Converts the "string" input to long
            i += 2;

        }
        else if(strcmp(argv[i], "-f") == 0)
        {
            if(i + 2 >= argc) //checks the values after -f, returns if nothing there
            {
                return 1;
            }
            option_f = 1;
            option_f_pattern = argv[i + 1];
            option_f_depth = atoi(argv[i + 2]);
            i += 3;

        }
        else 
        {
            start_dir = argv[i];
            i++;
        }

    }
    display_struct(start_dir, 0);
    return 0;~
}