#include <stdio.h>

int main(void)
{
    printf("Filename is %s\n", __FILE__);
    printf("Function is %s\n", __func__);
    printf("Line number is: %d\n", __LINE__);
    printf("Date is: %s\n", __DATE__);
    printf("Time is: %s\n", __TIME__);

    return 0;
}