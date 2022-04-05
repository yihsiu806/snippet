#include <stdio.h>
#include <stdarg.h>

void print_format_string(const char *fmt, ...)
{
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    printf("%s\n", buf);
}

void print_variable_numbers(const void *ptr, ...)
{
    int num;
    va_list ap;

    va_start(ap, ptr);
    num = va_arg(ap, int);
    va_end(ap);

    printf("num %d\n", num);
}

int main(void)
{
    const char str[] = "Hello, world!";
    const int num = 123;

    print_format_string("\t%s %d\n", str, num);

    print_variable_numbers(NULL, 1, 2, 3);

    return 0;
}