/*
pccasey
CS332
*/

#include <stdio.h>

int sumOfDigits()
{
    char n[3];
    printf("Enter a number: ");
    scanf("%s", n);
    int total = 0;
    int length = (sizeof(n) / sizeof(n[0]));
    for(int i = 0; i < length; i++)
    {
        printf("%c\n", n[i]);
        total = total + ("%d", n[i]);
    }
    printf("%d\n", total);
    return 0;
}


int main(int argc, char** argv)
{
    sumOfDigits();
}

