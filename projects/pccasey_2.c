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

static void permissions(mode_t mode, char *buf)
{
    buf[0] = S_ISDIR(mode)  ? 'd' :
             S_ISLNK(mode)  ? 'l' : '-';
    buf[1] = (mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (mode & S_IXUSR) ? 'x' : '-';
    buf[4] = (mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (mode & S_IXGRP) ? 'x' : '-';
    buf[7] = (mode & S_IROTH) ? 'r' : '-';
    buf[8] = (mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (mode & S_IXOTH) ? 'x' : '-';
    buf[10] = '\0';
}

static char *last_access(const struct stat *st)
{
    static char buf[32];
    strcpy(buf, ctime(&st->st_atime));
    for(int i = 0; i < 32; i++)
    {
        if(buf[i] == '\n')
        {
            buf[i] = '\0';
        }
    }
    return buf;
}


static void print_S(const struct stat *st, long size_override)
{
    char perm[11];
    permissions(st->st_mode, perm);
    printf(" (%ld, %s, %s)", size_override, perm, last_access(st));
}

void display_struct(const char *path, int depth) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    printf("Error opening directory '%s'\n", path);
    return;
  }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

        char fullPath[4096];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(fullPath, &st) != 0)
        {
            printf("Error reading '%s'\n", fullPath);
            continue;
        }
        

           
        int temp = option_s && !S_ISDIR(st.st_mode) && st.st_size > option_s_size; //checks if the file meets the -s requirement
        int temp2 = option_f && !(strstr(entry->d_name, option_f_pattern) != NULL && depth <= option_f_depth);
        //makes sure the file meets the -f requirement

        if(!temp && !temp2)
        {
            for (int i = 0; i < depth; i++) {
                printf("    ");
            }

            printf("%s", entry->d_name);

            if (option_S) 
            {
                print_S(&st, (long)st.st_size);
            }
            printf("\n");

        
        }
        if (S_ISDIR(st.st_mode)) 
        {
            display_struct(fullPath, depth + 1);
        }
    
    }

  closedir(dir);
}

int main(int argc, char *argv[])
{
    const char *start_dir = ".";
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
    char path_named[4096]; //holds the path name
    const char *display_path = start_dir;
    if (strcmp(start_dir, ".") == 0)
    {
        if (getcwd(path_named, sizeof(path_named)) != NULL)
        {
            display_path = path_named;
        }
    }
    const char *name = strrchr(display_path, '/');
    name = (name != NULL && *(name + 1) != '\0') ? name + 1 : display_path;

    printf("%s\n", name); 
    display_struct(start_dir, 1);
    return 0;
}