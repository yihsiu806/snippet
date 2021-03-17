#include <stdio.h>
#include <signal.h>

void sigproc(void);
void quitproc(void);

int main(void)
{
	signal(SIGINT, sigproc);
	signal(SIGQUIT, quitproc);
	printf("ctrl-c disabled use ctrl-\\ to quit\n");
	for(;;);
	return 0;
}

void sigproc()
{
	signal(SIGINT, sigproc);
	printf("you have pressed ctrl-c\n");
}

void quitproc()
{
	printf("ctrl-\\ pressed to quit\n");
	exit(0);
}
