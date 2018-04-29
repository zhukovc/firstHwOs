//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
char prevWD[MAX_LINE_SIZE];

typedef enum {SUCCESS, FAILURE} Result;

int min(int a, int b){
	return (a < b) ? a : b;
}

bool isEqual(char* str1, char* str2){
	return (strcmp(str1, str2) == 0 ? TRUE : FALSE);
}

void RollBackHistories(Smash smash){
	int i = 0;
	for(i = 0; i < NUM_OF_HISTORIES - 1; i++){
		strcpy(smash->history[i], smash->history[i + 1]);
	}
}

void addToHistory(Smash smash, char* command){
	if(NUM_OF_HISTORIES - 1 == smash->currHistory){
		RollBackHistories(smash);
	}
	strcpy(smash->history[smash->currHistory], command);
	smash->currHistory = min(smash->currHistory + 1, NUM_OF_HISTORIES - 1);
	return;
}

void DisplayHistory(Smash smash){
	int i = 0;
	for(i = 0; i < smash->currHistory - 1; i++){
		printf("%s\n", smash->history[i]);
	}
	return;
}

Result cd(char* path){
	char pwd[MAX_LINE_SIZE], auxWD[MAX_LINE_SIZE];
	getcwd(pwd, MAX_LINE_SIZE);
	strcpy(auxWD, prevWD);

	if(isEqual(path, "-")){
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
	addToHistory(smash, cmdString);

	if (!strcmp(cmd, "cd") ) 
	{
		char* path = args[1];
		if (SUCCESS != cd(path)){
			printf("smash error: > \"%s\" - path not found\n", path);
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		getcwd(pwd, MAX_LINE_SIZE);
		printf("%s\n", pwd);
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
 		DisplayHistory(smash);
	}
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		pid_t smashPid = getpid();
		printf("smash pid is %d\n", smashPid);
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
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
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					break;
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
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

