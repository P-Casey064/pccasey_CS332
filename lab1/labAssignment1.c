#include <stdio.h>
/*
pccasey
6/2/2026
CS332

to compile in terminal
gcc -o labAssignment1 labAssignment1.c

to run in terminal
./labAssignment1

*/
int main(int argc, char** argv)
{
    int none = 0;
    int prime = 0;
    int given_number;
    printf("Enter a number: ");
    scanf("%d", &given_number);

    if(given_number == 2)
    {
        prime = 1;
        printf("Is a Prime\n");
        return 0;
    }
    for (int i = 2; i <= 10; i++)
    {
        if(given_number % i == 0)
        {
            none++;
        }
        else{
            prime++;
        }
    }
    if(none > 0)
    {
        printf("Is not a Prime\n");
        return 0;
    }
    else{
        printf("Is a Prime\n");
        return 0;
    }

    
}