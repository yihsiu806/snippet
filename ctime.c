#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t now;
	time(&now);
	// ctime(t)
	// "Wed Jun 30 21:49:08 1993\n"
	char *timestamp;
	timestamp = ctime(&now) + 4;
	timestamp[15] = '\0';

	// Dec 15 14:55:47
	puts(timestamp);
	
	return 0;
}
