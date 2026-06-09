#include <stdio.h>
/*
pccasey
6/9/2026
CS332

to compile in terminal
gcc -o lab_2 lab2.c

to run in terminal
./lab_2

*/
int insertionSort(int n[], int A)
{
    int temp, loc;
    for(int i = 0; i < A; i++)
    {
        loc = i;
        while (loc > 0 && n[loc - 1] > n[loc])
        {
            temp = n[loc];
            n[loc] = n[loc - 1];
            n[loc-1] = temp;
            loc--;
        }
    }
    printf("[");
    for(int i = 0; i < A; i++)
    {
        printf("%d", n[i]);
    }
    if(i < A - 1)
    {
        printf(", ");
    }
    printf("]\n");
}




int main(int argc, char** argv)
{
    int A;
    int z;
    printf("Input the length of the array: ");
    scanf("%d", &A);
    int n[A];

    for(int i = 0; i < A; i++)
    {
        printf("Please enter an element of the array: ");
        scanf("%d", &n[i]);
    }

    insertionSort(n, A);

}