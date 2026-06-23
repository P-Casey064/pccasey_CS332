#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define BUFFSIZE 4096
#define SEEKSIZE -30

int main(int argc, char *argv[]) {

int RWFileDescriptor;
int writeFile;
long int n;
char buf[BUFFSIZE];
if (argc != 3){ //changed to 3 to include the second file
    printf("Usage: %s <filename>\n", argv[0]);
    exit (-1);
}

RWFileDescriptor = open(argv[1], O_RDONLY); //reads first file

if (RWFileDescriptor == -1){

    printf("Error with file open1\n");

    exit (-1);
}

if (lseek(RWFileDescriptor, SEEKSIZE, SEEK_END) >= 0){

    if((n = read(RWFileDescriptor, buf, BUFFSIZE)) > 0){

        if (write(STDOUT_FILENO, buf, n) != n) {

            printf("Error writing to file\n");

            exit (-1);

}

} else {

    printf("Error reading file\n");

    exit (-1);

}

}
 else {

    printf("lseek error (Part 1)\n");

    exit (-1);

}

RWFileDescriptor = open(argv[2], O_RDONLY); //reads second file

if (RWFileDescriptor == -1){

    printf("Error with file open1\n");

    exit (-1);
}

if (lseek(RWFileDescriptor, SEEKSIZE, SEEK_END) >= 0){

    if((n = read(RWFileDescriptor, buf, BUFFSIZE)) > 0){

        if (write(STDOUT_FILENO, buf, n) != n) {

            printf("Error writing to file\n");

            exit (-1);

}

} else {

    printf("Error reading file\n");

    exit (-1);

}

close(RWFileDescriptor);

RWFileDescriptor = open(argv[1], O_WRONLY); //opens the file for writing
writeFile = open(argv[2], O_RDONLY); //opens file to read
char ch;
lseek(RWFileDescriptor, 0, SEEK_END);
lseek(writeFile, 0, SEEK_SET);
ssize_t bytes_read;

while ((bytes_read = read(writeFile, &ch, 1)) > 0){

    if (write(RWFileDescriptor, &ch, 1) < 0) {

        printf("Error writing to file\n");
        break;
    }
    if (ch == '\n')
    {
        break;
    }

} 


close(RWFileDescriptor);
close(writeFile);

return 0;

}
}
