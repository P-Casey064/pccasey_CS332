#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
pccasey
6/9/2026
CS332

to compile in terminal
gcc -o lab3 lab3.c

to run in terminal
./lab3

*/
void insertionSort(char *arr, int A)
{
    char temp;
    int loc;
    for(int i = 0; i < A; i++)
    {
        loc = i;
        while (loc > 0 && arr[loc - 1] > arr[loc])
        {
            temp = arr[loc];
            arr[loc] = arr[loc - 1];
            arr[loc-1] = temp;
            loc--;
        }
    }
}

void displayString(char *arr, int A)
{
    printf("[");
    for(int i = 0; i < A; i++)
    {
        printf("%c", arr[i]);
    
    if(i < A - 1)
    {
        printf(", ");
    }
}
    printf("]\n");
}

int readString(char *arr, int N)
{
    if (arr[0] == '\0') //this checks if the null pointer is first, which would be an improper input
    {
        printf("Not a proper string\n");
        return 0;
    }
    else if(strlen(arr) + 1 != N) //adding 1 for null pointer
    {
        printf("Size and length of string do not match\n");
        return 0;
    }
    else
    {
        return 1;
    }


}

int main(int argc, char** argv)
{
    int N;
    printf("Input the length of the array: ");
    scanf("%d", &N);
    N++; //adding 1 for null pointer
    if(N == 1 || N <= 0)  //this force ends the code for improper inputs
    {
        printf("Improper array length\n");
        return 0;
    }

    char *arr = (char*) malloc((N + 1) * sizeof(char));
    printf("Please enter a String: ");
    scanf(" %[^\n]", arr);
    if(readString(arr, N) == 1)
    {
        insertionSort(arr, N);
        displayString(arr, N);
    }



}