#include <stdio.h>
#include <math.h>
/*
Compile:
gcc -o hw1 pccasey_HW1.c -lm

Run:
./hw1
*/
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
    printf("total: %d\n\n", total);
    return 0;
}

int UABMaxMinDiff(int n[], int A)
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
    int diff = max - min;
    printf("Difference: %d\n\n", diff);
    return 0;
}

int replaceEvenWithZero(int n[], int A)
{
    for(int i = 0; i < A; i++)
    {
        if(n[i] % 2 == 0)
        {
            n[i] = 0;
        }
    }
    printf("[");
    for(int i = 0; i < A; i++)
    {
        printf("%d", n[i]);
        if(i < (A - 1))
        {
            printf(", ");
        }
    }
    printf("]\n\n");
    return 0;
}

int perfectSquare(double B)
{
    int C = (int)sqrt(B);
    if(C * C == B)
    {
        printf("True\n\n");
    }
    else
    {
        printf("False\n\n");
    }
}

int countVowels(char n[], int A)
{
    const char vowels[] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
    int count = 0;
    int size = sizeof(vowels) / sizeof(vowels[0]);
    for(int i = 0; i < A; i++)
    {
        for(int z = 0; z < size; z++)
        {
            if(vowels[z] == n[i])
            {
                count++;
            }
        }
    }
    printf("Total vowels: %d\n", count);
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
    int C;
    printf("Input the length of the Array: ");
    scanf("%d", &C);
    int t[C];
    for(int i = 0; i < C; i++)
    {
        printf("Input an element for the array: ");
        scanf("%d", &t[i]);
    }
    UABMaxMinDiff(t, C);

/*replaceEvenWithZero*/
int D;
    printf("Input the length of the Array: ");
    scanf("%d", &D);
    for(int i = 0; i < D; i++)
    {
        printf("Input an element for the array, even or odd: ");
        scanf("%d", &t[i]);
    }
    replaceEvenWithZero(t, D);

/*perfectSquare*/
    double B;
    printf("Input a number: ");
    scanf("%lf", &B);
    perfectSquare(B);

/*countVowels*/
    int F;
    printf("Input the size of your string: ");
    scanf("%d", &F);
    char l[F + 1]; //so this includes the null space at the end of every string
    printf("Input your string: ");
    scanf(" %[^\n]", l);
    countVowels(l, F);
}