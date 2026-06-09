#include <stdio.h>

int sumOfDigits(char n[], int A)
{
    int total = 0;
    if(n[0] == '-' || (("%d", n[0]) - '0') == 0) /*subtracting the '0' removes the ascii value*/
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
    printf("total: %d\n", total);
    return 0;
}

int UABMaxMinDiff(int A, int n[])
{
    int min, max;
    min = n[0];
    max = n[0];
    for(int i = 0; i < A; i++)
    {
        if(n[i] >= max)
        {
            max = n[i];
        }
        if(n[i] <= min)
        {
            min = n[i];
        }
    }
    printf("%d\n", max);
    printf("%d\n", min);
    int diff = max - min;
    printf("Difference: %d\n", diff);
    return 0;
}


int main(int argc, char** argv)
{
/*sumOfDigits*/
    int A;
    printf("Input the length of the number: ");
    scanf("%d", &A);
    
    char n[A];
    printf("Input the number: ");
    scanf("%s", n);
    sumOfDigits(n, A);

/*UABMaxMinDiff*/
    printf("Input the length of the Array: ");
    scanf("%d", &A);
    int t[A];
    for(int i = 0; i < A; i++)
    {
        printf("Input an element for the array: ");
        scanf("%d", &t[i]);
    }
    UABMaxMinDiff(A, t);
}