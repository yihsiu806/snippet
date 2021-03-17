#include <stdio.h>
#include <unistd.h>

int main(void)
{
	char hostname[100];
	gethostname(hostname, 100);
	puts(hostname);
	return 0;
}
