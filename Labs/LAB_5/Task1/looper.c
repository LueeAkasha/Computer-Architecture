#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static void signal_handler(int signum) {
	fprintf(stdout,strcat(strsignal(signum), " was received!") , "%s");
	signal(signum, SIG_DFL);
	raise(signum);
	
	if (signum == SIGCONT)
	{
		signal(SIGTSTP, signal_handler);
	}
	else if (signum == SIGTSTP)
	{
		signal(SIGCONT, signal_handler);
	}
}

int main(int argc, char **argv){ 

	printf("Starting the program\n");
	signal(SIGTSTP, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGCONT, signal_handler);
	while(1) {
		sleep(2);
	}
	return 0;
}