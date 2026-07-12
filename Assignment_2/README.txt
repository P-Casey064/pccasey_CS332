To compile: $ make
        or  $ gcc -o search pccasey_HW2.c
To run: ./search -S -s <bytes> -f <string pattern> <depth>
        -S -s -f can be ran in any order

Documentation:

permissions() creates a char array that will check if a file has certain permissions and changes that array with a '-' for no permissions or a 'r', 'w', 'x' if it does have permissions.
It will also start the array with a 'd' for directory, 'l' for symlink, or '-' if neither

last_access() provides the last time the file or directory was accessed

print_S() prints the byte size, permissions, and last access time. It is called by display_struct() when -S is used when running the code

display_struct() is the recursion function that handles displaying purely the names of the files and directories while also having the filters is -s and -f are used

main() is where it figures out if -S, -s, or -f is used when running the code.  It loops until it goes through all arguments in argc.  
It also gets the name of the directory the .c file is in for easier viewing.

