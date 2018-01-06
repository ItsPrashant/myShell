#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER 512						//Maximum length of the command string which can be given. 
#define MAX_PROC_ARG 100   					//Max no. of arguments which can be provided to child processes
#define MAX_PROCESS 100						//Max no. of processes which can be forked in given command string

char promptStr[]="prashantShell >>";		//Prompt string

unsigned char status=1;
char buffer[MAX_BUFFER+10];					//Buffer which reads command string
char delim=';';								//Delimiter for different commands

void createProcess(char *cmd);

int main(int argc,char **argv)
{
	
	if(argc>2)														// Test for multiple arguments to shell
	{
		perror("ERROR: I can only process single batch file\n"); 
		exit(-1);
	}
	if(argc==2)									// If Batch file is provided
	{
		FILE *fptr;
		fptr=fopen(argv[1],"r");									//Open file to read
		if(fptr==NULL)												//Check if file opened successfully
		{
			printf("Error opening file\n");
			exit(-1);
		}
		
		int bufferLen;
		while(status==1)											//while quit is not found status is true;
		{
			char *cmdArray[MAX_PROCESS];

			if(fgets(buffer,MAX_BUFFER,fptr)==NULL)					//Get line from file.
			{
				//perror("ERROR: Getting line from file\n");
				exit(0);											//Exit if line can't be read or EOF reached.
			}
			bufferLen=strlen(buffer);								
			if(bufferLen>=MAX_BUFFER)								//Check if received String is greater than or equal to MAX_BUFFER
			{
				printf("WARNING: Command is too long\n");
				continue;
			}
			printf("%s",buffer);
			int procNo=0;
			cmdArray[procNo++]=strtok(buffer,&delim);				//Parse received string to extract different commands
			while(cmdArray[procNo-1]!=NULL)
			{
				cmdArray[procNo++]=strtok(NULL,&delim);
			}
			procNo--;
			int i=procNo;
			while(procNo>0)											//process every command and create child process for it.		
			{
				createProcess(cmdArray[--procNo]);
			}
			while(i>0)												//Now wait for every child process to return
			{
				wait(NULL);
				i--;
			}
			
		}
		fclose(fptr);												//Close file/
		
	}
	else 											// Interactive mode
	{
		while(status==1)											//while quit is not found status is true;
		{
			char *cmdArray[MAX_PROCESS];

			printf("%s",promptStr);
			if(fgets(buffer,MAX_BUFFER,stdin)==NULL) 				//Get line from standard input.
			{
				perror("ERROR: Getting string from stdin\n");		//Exit if line can't be read.
				exit(-1);
			}
			if(strlen(buffer)>=MAX_BUFFER)							//Check if received String is greater than or equal to MAX_BUFFER
			{
				printf("WARNING: Command is too long\n");
				continue;
			}
			int procNo=0;
			cmdArray[procNo++]=strtok(buffer,&delim);				//Parse received string to extract different commands
			while(cmdArray[procNo-1]!=NULL)
			{
				cmdArray[procNo++]=strtok(NULL,&delim);
			}
			procNo--;
			int i=procNo;
			while(procNo>0)											//process every command and create child process for it.		
			{
				createProcess(cmdArray[--procNo]);
			}
			while(i>0)												//Now wait for every child process to return
			{
				wait(NULL);
				i--;
			}
			//printf("\n");
			fflush(stdout);
			
		}
		
		
	}
	
	
	return 0;
}
/* This function processes every command and generate the required arguments for command execution.
 * It creates child processes as well using fork() and execvp()
 * 
 * 
 */

void createProcess(char *cmd)
{
	char *procArgv[MAX_PROC_ARG]; 					//Array for arguments 
//	printf("Command string: %s\n",cmd);
	char localDelim=' '; 
	int argNo=0; 									//Total no. of arguments
	procArgv[argNo++]=strtok(cmd,&localDelim); 		//Parse string for arguments
	while(procArgv[argNo-1]!=NULL)	
	{
		procArgv[argNo++]=strtok(NULL,&localDelim);
	}
	
	if(argNo>1&&!strcmp(procArgv[argNo-2],"\n"))	//Check if any endline '\n' character is found as an argument
	{
		procArgv[argNo-2]=NULL; 					//Assign NULL to it.
		argNo--;									//Decrement agrument counter.
	}
	
	if(argNo==1)
	{
		return ;									//If no arguments are found then return
	}

	localDelim='\n';														
	procArgv[argNo-2]=strtok(procArgv[argNo-2],&localDelim);		//Now remove any newline character in command to be executed
	if(!strcmp(procArgv[0],"quit"))					//If quit is found then unset the status(global) flag to indicate end of loop
	{
		//printf("BYE\n");
		status=0;
		return;
	}

	int retVal;										
	retVal=fork();									
	if(retVal==-1)									//Check is fork failed
	{
		perror("ERROR: fork() failed\n");			
		exit(-1);
	}
	else
	if(retVal==0)									//Check return value to differentiate between child and parent
	{

		if(execvp(procArgv[0],procArgv)==-1)        //exec the command and check if exec returned and signal error
		{
			perror("ERROR: Invalid command\n");
			exit(-1);
		}
		
	}
	//return 0;
	
}


