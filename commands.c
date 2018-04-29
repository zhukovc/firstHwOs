//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
char prevWD[MAX_LINE_SIZE];

typedef enum {SUCCESS, FAILURE} Result;

int Min(int a, int b){
	return (a < b) ? a : b;
}

bool IsEqual(char* str1, char* str2){
	return (strcmp(str1, str2) == 0 ? TRUE : FALSE);
}

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void RollBackHistories(Smash smash){
	int i = 0;
	for(i = 0; i < NUM_OF_HISTORIES - 1; i++){
		strcpy(smash->history[i], smash->history[i + 1]);
	}
}

void AddToHistory(Smash smash, char* command){
	if(NUM_OF_HISTORIES - 1 == smash->currHistory){
		RollBackHistories(smash);
	}
	strcpy(smash->history[smash->currHistory], command);
	smash->currHistory = Min(smash->currHistory + 1, NUM_OF_HISTORIES - 1);
	return;
}

void DisplayHistory(Smash smash){
	int i = 0;
	for(i = 0; i < smash->currHistory - 1; i++){
		printf("%s\n", smash->history[i]);
	}
	return;
}

Result ChangeDirectory(char* path){
	char pwd[MAX_LINE_SIZE], auxWD[MAX_LINE_SIZE];
	getcwd(pwd, MAX_LINE_SIZE);
	strcpy(auxWD, prevWD);

	if(IsEqual(path, "-")){
		strcpy(pwd, auxWD);
		getcwd(prevWD, MAX_LINE_SIZE);
		chdir(pwd);
		return SUCCESS;
	}

	int cdResult = chdir(path);

	if(0 != cdResult){  // chdir command failed
		memset(prevWD, 0x0, MAX_LINE_SIZE);
		strcpy(prevWD, auxWD);
		return FAILURE;
	}
	else {	// chdir command succeeded
		strcpy(prevWD, pwd);
		getcwd(pwd, MAX_LINE_SIZE);
		return SUCCESS;
	}
}

Result InsertJob(Smash smash, char *cmdString, pid_t pID, time_t time){
	if(smash->numOfJobs >= MAX_NUM_OF_PROCESSES){
		perror("Too many processes");
		return FAILURE;
	}

	strcpy(smash->jobs[smash->numOfJobs].jobName, cmdString);
	smash->jobs[smash->numOfJobs].pid = pID;
	smash->jobs[smash->numOfJobs].startTime = time;
	smash->numOfJobs = Min(smash->numOfJobs + 1, MAX_NUM_OF_PROCESSES );
	return SUCCESS;
}

void PrintJobs(Smash smash){
	int i=0;
	for(i = 0; i < smash->numOfJobs; i++){
		printf("[%d] ", i+1);
		printf("%s : ", smash->jobs[i].jobName);
		printf("%d ", smash->jobs[i].pid);
		printf("%d secs\n", (int)(time(NULL) - smash->jobs[i].startTime));
	}
	return;
}

bool IsBackGround(char* cmdString){
	char* delimiters = " \t\n";
	//char* revString = (char*) malloc (sizeof(char) * MAX_LINE_SIZE + 1);
	//revString = strrev(cmdString);
	char* token = strtok(strrev(cmdString), delimiters);
	printf("Last argument is ***%s***\n", token);
	//free(revString);
	return IsEqual(token, "&");
}

int ExeCmd(Smash smash, void* jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	AddToHistory(smash, cmdString);

	if (!strcmp(cmd, "cd") ) 
	{
		switch (num_arg) {
			case 1:
				if (SUCCESS != ChangeDirectory(args[1])){
					printf("smash error: > \"%s\" - path not found\n", args[1]);
				}
				if(IsEqual(args[1], "-")){
					char* cwd = getcwd(pwd, MAX_LINE_SIZE);
					printf("%s\n", cwd);
				}
				break;

			default:
				illegal_cmd = TRUE;
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		switch (num_arg){
			case 0:
				getcwd(pwd, MAX_LINE_SIZE);
				printf("%s\n", pwd);
				break;

			default:
				illegal_cmd = TRUE;
		}

	}
	
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
 		switch (num_arg){
			case 0:
				DisplayHistory(smash);
				break;

			default:
				illegal_cmd = TRUE;
 		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "mv"))
	{
 		switch (num_arg){
			case 2:
				if (!rename(args[1], args[2])){
					printf("%s has been renamed to %s\n", args[1], args[2]);
				}else{
					perror("Error: ");
				}
				break;

			default:
				illegal_cmd = TRUE;
 		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		PrintJobs(smash);
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
 		switch (num_arg){
			case 0:
				printf("smash pid is %d\n", getpid());
				break;

			default:
				illegal_cmd = TRUE;
 		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
 		switch (num_arg){
			case 0:
				break;

			case 1:
				break;

			default:
				illegal_cmd = TRUE;
 		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		switch (num_arg){
			case 0:
				break;

			case 1:
				break;

			default:
				illegal_cmd = TRUE;
 		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
 		switch (num_arg){
			case 0:
				exit(1);
				break;

			case 1:
				if(IsEqual(args[1], "kill")){

				}
				else{
					illegal_cmd = TRUE;
				}
				break;

			default:
				illegal_cmd = TRUE;
 		}
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(smash, args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(Smash smash, char *args[MAX_ARG], char* cmdString)
{
	int pID;
	printf("entering ExeExternal\n");
    	switch(pID = fork()) 
	{
    		case -1: 
    			perror("Could not create a new process");
    			break;

        	case 0 :
				// Child Process
				setpgrp();
				// printf("son code running\n");
				execvp(args[0], args);
				perror("Could not execute external command");
				exit(1);
				break;
			
			default:
				InsertJob(smash, cmdString, pID, time(NULL));
				if(IsBackGround(cmdString)){ 	// Run in bg
					printf("Run in background\n");
				}
				else{	// Run in fg
					printf("Run in foreground\n");
					int status;
					if(-1 == waitpid(pID, &status, WUNTRACED)){
						perror("waitpid() failed");
					}
				}
				break;
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	//char ExtCmd[MAX_LINE_SIZE+2]; // TODO: use it
	//char *args[MAX_ARG];	// TODO: use it
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	/* TODO: use all these
	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];*/
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

