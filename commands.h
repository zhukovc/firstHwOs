#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Extra history, for the last "history" command
#define NUM_OF_HISTORIES (50 + 1)
#define NUM_OF_PROCESSES (100)
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
typedef enum { FALSE , TRUE } bool;

typedef struct job_t {
	char* jobName;
	int pid;
	int startTime;
} Job ;
// typedef struct job_t *Job;

struct smash_t {
	char** history;
	int currHistory;
	Job* jobs;
	int currJob;
};
typedef struct smash_t *Smash;

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(Smash smash, void* jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif

