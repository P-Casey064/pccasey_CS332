/*
 * search.c
 *
 * A simplified re-implementation of the UNIX "find" command called
 * "search". It walks a directory hierarchy and prints it, with optional
 * flags:
 *
 *   -S               show size, permissions and last-access-time for
 *                     every file/directory (size is 0 for directories)
 *   -s <bytes>        only list files whose size is <= <bytes>
 *   -f <pattern> <n>  only list files whose name contains <pattern> AND
 *                     whose depth (relative to the starting directory,
 *                     which is depth 0) is <= <n>
 *
 * Any combination of -S, -s and -f may be given, in any order, and the
 * starting directory (optional, defaults to ".") may appear anywhere on
 * the command line as the one argument that does not start with '-'.
 *
 * Build:
 *   gcc -Wall -o search search.c
 *
 * Usage examples:
 *   ./search
 *   ./search /some/dir
 *   ./search -S
 *   ./search -s 1024
 *   ./search -f jpg 1
 *   ./search -S -s 1024 -f jpg 2
 *   ./search mydir -f jpg 2 -s 1024 -S
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

/* ---------- global option state ---------- */
static int   opt_S = 0;              /* -S flag                       */
static int   opt_s_set = 0;          /* whether -s was given          */
static long  opt_s = 0;              /* -s <bytes>                    */
static int   opt_f_set = 0;          /* whether -f was given          */
static char *opt_f_pattern = NULL;   /* -f <pattern>                  */
static int   opt_f_depth = 0;        /* -f <depth>                    */

/* ---------- helpers ---------- */

/* Build a "rwxr-xr-x" style permission string from st_mode into buf
 * (buf must be at least 11 bytes). */
static void mode_to_str(mode_t mode, char *buf)
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

/* Return a static buffer holding a human readable last-access-time,
 * without the trailing newline that ctime() adds. */
static char *time_to_str(time_t t)
{
    static char buf[64];
    char *s = ctime(&t);
    strncpy(buf, s, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
        buf[len - 1] = '\0';
    return buf;
}

/* Print the "(size, permissions, last access time)" suffix used by -S.
 * size_override is used so directories can be forced to report size 0. */
static void print_S_info(const struct stat *st, long size_override)
{
    char perm[11];
    mode_to_str(st->st_mode, perm);
    printf(" (%ld, %s, %s)", size_override, perm, time_to_str(st->st_atime));
}

/* Does this (non-directory) file pass the -s / -f filters? depth is the
 * file's depth relative to the traversal root (root == depth 0). */
static int passes_filters(const char *name, long size, int depth)
{
    if (opt_s_set && size > opt_s)
        return 0;
    if (opt_f_set) {
        if (strstr(name, opt_f_pattern) == NULL)
            return 0;
        if (depth > opt_f_depth)
            return 0;
    }
    return 1;
}

/* Print `depth` tab characters. */
static void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        putchar('\t');
}

/* Compare function for scandir: alphabetical order. */
static int name_cmp(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

/* Forward declaration */
static void walk(const char *path, int depth);

/*
 * Process a single directory entry (file, symlink or subdirectory).
 * `dirpath` is the path of the containing directory, `name` is the
 * entry's name, and `depth` is this entry's depth (root == 0).
 */
static void handle_entry(const char *dirpath, const char *name, int depth)
{
    char fullpath[4096];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, name);

    struct stat st;
    if (lstat(fullpath, &st) != 0) {
        fprintf(stderr, "search: cannot stat %s: %s\n", fullpath, strerror(errno));
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        /* Directories are always shown as part of the hierarchy. */
        print_indent(depth);
        printf("%s", name);
        if (opt_S)
            print_S_info(&st, 0L);   /* directories report size 0 */
        printf("\n");

        walk(fullpath, depth + 1);
        return;
    }

    /* Non-directory entry: apply -s / -f filters. */
    long size = (long) st.st_size;
    if (!passes_filters(name, size, depth))
        return;

    print_indent(depth);

    if (S_ISLNK(st.st_mode)) {
        char target[4096];
        ssize_t n = readlink(fullpath, target, sizeof(target) - 1);
        if (n >= 0) {
            target[n] = '\0';
            printf("%s (%s)", name, target);
        } else {
            printf("%s (?)", name);
        }
    } else {
        printf("%s", name);
    }

    if (opt_S)
        print_S_info(&st, size);

    printf("\n");
}

/* Walk `path` (a directory) at the given depth, printing its entries. */
static void walk(const char *path, int depth)
{
    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, name_cmp);
    if (n < 0) {
        fprintf(stderr, "search: cannot open %s: %s\n", path, strerror(errno));
        return;
    }

    for (int i = 0; i < n; i++) {
        const char *name = namelist[i]->d_name;
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
            handle_entry(path, name, depth);
        free(namelist[i]);
    }
    free(namelist);
}

static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s [directory] [-S] [-s size] [-f pattern depth]\n", prog);
}

int main(int argc, char *argv[])
{
    const char *start_dir = ".";

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-S") == 0) {
            opt_S = 1;
            i++;
        } else if (strcmp(argv[i], "-s") == 0) {
            if (i + 1 >= argc) { usage(argv[0]); return 1; }
            opt_s_set = 1;
            opt_s = atol(argv[i + 1]);
            i += 2;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 2 >= argc) { usage(argv[0]); return 1; }
            opt_f_set = 1;
            opt_f_pattern = argv[i + 1];
            opt_f_depth = atoi(argv[i + 2]);
            i += 3;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "search: unknown option %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        } else {
            start_dir = argv[i];
            i++;
        }
    }

    struct stat st;
    if (stat(start_dir, &st) != 0) {
        fprintf(stderr, "search: cannot stat %s: %s\n", start_dir, strerror(errno));
        return 1;
    }
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "search: %s is not a directory\n", start_dir);
        return 1;
    }

    walk(start_dir, 0);

    return 0;
}