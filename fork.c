#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	if (!fork()) {
		/* child process */
		printf("I am the child.\n");
		exit(0);
	} else {
		/* parant process */
		printf("I am the parent.\n");
		wait(NULL);
	}

	return 0;
}
