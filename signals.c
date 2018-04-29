// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

int sendSignal(int pid, int sig){
	if (0 != kill(pid, sig))
		{
			printf("smash error: > kill %d cannot send signal %s\n", pid, strsignal(sig));
			return -1;
		}
	printf("smash > signal %s was sent to pid %d\n", strsignal(sig), pid);
	return 0;
}
