/*	smash->c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

char* L_Fg_Cmd;
void* jobs = NULL; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 

void InitHistory(Smash *smashPtr){
	Smash smash = *smashPtr;
	smash->history = (char**) malloc (sizeof(char*) * NUM_OF_HISTORIES);
	if(NULL == smash->history){
		return;
	}

	// Init array of histories
	int i = 0, j = 0;
	for(i = 0; i < NUM_OF_HISTORIES; ++i){
		smash->history[i] = (char*) malloc (sizeof(char) * MAX_LINE_SIZE + 1);
		if(NULL == smash->history[i]){
			for(j = 0; j < i; ++j){
				free(smash->history[i]);
			}
			free(smash->history);
		}
	}
}

void InitJobs(Smash *smashPtr){
	Smash smash = *smashPtr;
	smash->jobs = (Job*) malloc (sizeof(Job) * NUM_OF_PROCESSES);
	if(NULL == smash->jobs){
		return;
	}

	int i = 0, j = 0;
	for(i = 0; i < NUM_OF_PROCESSES; ++i){
		smash->jobs[i].jobName = (char*) malloc (sizeof(char) * MAX_LINE_SIZE + 1);
		if(NULL == smash->jobs[i].jobName){
			for(j = 0; j < i; ++j){
				free(smash->jobs[i].jobName);
			}
			free(smash->jobs);
		}
	}
}

Smash InitSmash(Smash *smashPtr){
	Smash smash = *smashPtr;
	smash = (Smash) malloc (sizeof(smash));
	if(NULL == smash){
		return NULL;
	}

	InitHistory(&smash);
	if(NULL == smash->history){
		return NULL;
	}

	InitJobs(&smash);
	if(NULL == smash->jobs){
		return NULL;
	}

	smash->currHistory = 0;
	return smash;
}

void freeSmash(Smash smash){
	int i = 0;

	for(i = 0; i < NUM_OF_HISTORIES; ++i){
		free(smash->history[i]);
	}
	free(smash->history);

	for(i = 0; i < NUM_OF_HISTORIES; ++i){
		free(smash->jobs[i].jobName);
	}
	free(smash->jobs);

	return;
}

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];
    Smash smash = NULL;
    smash = InitSmash(&smash);

	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		if(!ExeComp(lineSize)) continue; 
					// background command	
	 	if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(smash, jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    freeSmash(smash);
    return 0;
}

