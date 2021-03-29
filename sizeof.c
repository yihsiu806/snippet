#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    printf("The size of char is %ld byte(s)\n", sizeof(char));
    printf("The size of bool is %ld byte(s)\n", sizeof(bool));
    printf("The size of short is %ld byte(s)\n", sizeof(short));
    printf("The size of int is %ld byte(s)\n", sizeof(int));
    printf("The size of long int is %ld byte(s)\n", sizeof(long int));
    printf("The size of long long int is %ld byte(s)\n", sizeof(long long int));
    printf("The size of float is %ld byte(s)\n", sizeof(float));
    printf("The size of double is %ld byte(s)\n", sizeof(double));
    printf("The size of long double is %ld byte(s)\n", sizeof(long double));

    return 0;
}