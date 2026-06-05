#include <stdio.h>

int sumOfDigits(int A)
{
    char n[A];
    printf("Enter a number: ");
    scanf("%s", n);
    int total = 0;
    int length = (sizeof(n) / sizeof(n[0]));
    if(n[0] == '-' || (("%d", n[0]) - '0') == 0)
    {
        printf("-1\n");
        return 0;
    }

    for(int i = 0; i < A; i++)
    {
        if(n[i] != '\0')
        {
            total = total + (("%d", n[i]) - '0');
        }
    }
    printf("%d\n", total);
    return 0;
}


int main(int argc, char** argv)
{
    int A;
    printf("Input the length of the number: ");
    scanf("%d", &A);
    sumOfDigits(A);
}