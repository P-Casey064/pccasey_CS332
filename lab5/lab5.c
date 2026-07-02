/* Sample C
program to illustrate the use of opendir, readdir, and closedir
* system calls.
* This version prints the file type next to the file name.

execute with ./lab5 .
THE LAST DOT IS IMPORTANT
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
char *filetype(unsigned char type) {
    char *str;
    switch(type) {
        case DT_BLK: str = "block device"; break;
        case DT_CHR: str = "character device"; break;
        case DT_DIR: str = "directory"; break;
        case DT_FIFO: str = "named pipe (FIFO)"; break;
        case DT_LNK: str = "symbolic link"; break;
        case DT_REG: str = "regular file"; break;
        case DT_SOCK: str = "UNIX domain socket"; break;
        case DT_UNKNOWN: str = "unknown file type"; break;
        default: str = "UNKNOWN";
    }
return str;
}

void listDir(const char *path, int depth) {
  DIR *dir = opendir(path);
  if (dir == NULL) { //makes sure there is a directory
    printf("Error opening directory '%s'\n", path);
    return;
  }

  struct dirent *entry;
  int count = 1;

  while ((entry = readdir(dir)) != NULL) { //entry->d_name is a shorter way for d_name to access entry
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char fullPath[4096]; //full path is needed to show the whole thing. ex ./test(directory) -> ./test/test2(regular file)
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name); //this builds the path in memory, doesn't print anything
    
    for (int i = 0; i < depth; i++)
      printf("    ");

    printf("[%d] %s (%s)\n", count, fullPath, filetype(entry->d_type)); //prints the build path from snprintf
    count++;

    if (entry->d_type == DT_DIR) { //checks if the type is directory and starts the recursion if it is
      listDir(fullPath, depth + 1);
    }
  }

  closedir(dir);
}

int main (int argc, char **argv) {
    if (argc < 2) {
        printf ("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }
    listDir(argv[1], 0);
    
    return 0;
}