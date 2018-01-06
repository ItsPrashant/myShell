#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER 512
#define MAX_PROC_ARGLEN 30
#define MAX_PROC_ARG 100
#define MAX_PROCESS 100

char promptStr[]="prashantShell >>";

unsigned char status=1;
char buffer[1024];
char delim=';';

void createProcess(char *cmd);

int main(int argc,char **agrv)
{
	
	if(argc>2)	// Test for multiple arguments to shell;;
	{
		perror("ERROR: I can only process single batch file\n"); 
		exit(-1);
	}
	if(argc==2)	// If Batch file is provided
	{
		;
	}
	else 	// Interactive mode
	{
		while(status==1)	//while quit is not found status is true;
		{
			char *cmdArray[MAX_PROCESS];

			printf("%s",promptStr);
			if(fgets(buffer,MAX_BUFFER,stdin)==NULL)
			{
				perror("ERROR: Getting string from stdin\n");
				exit(-1);
			}
			int procNo=0;
			cmdArray[procNo++]=strtok(buffer,&delim);
			while(cmdArray[procNo-1]!=NULL)
			{
				cmdArray[procNo++]=strtok(NULL,&delim);
			}
			procNo--;
			int i=procNo;
			while(procNo>0)
			{
				createProcess(cmdArray[--procNo]);
			}
			while(i>0)
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

void createProcess(char *cmd)
{
	char *procArgv[MAX_PROC_ARG];
//	printf("Command string: %s\n",cmd);
	char localDelim=' ';
	int argNo=0;
/*	if(cmd==NULL)
	{
		printf("NULL \n");
	}
*/
	procArgv[argNo++]=strtok(cmd,&localDelim);
	while(procArgv[argNo-1]!=NULL)	
	{
		procArgv[argNo++]=strtok(NULL,&localDelim);
	}
	
	if(argNo>1&&!strcmp(procArgv[argNo-2],"\n"))
	{
		procArgv[argNo-2]=NULL;
		argNo--;
	}
	
	if(argNo==1)
	{
		return ;
	}
//	printf("argNo :%d \n",argNo);
	
	localDelim='\n';
	procArgv[argNo-2]=strtok(procArgv[argNo-2],&localDelim);
	if(!strcmp(procArgv[0],"quit"))
	{
		status=0;
		return;
	}
	int retVal;
	retVal=fork();
	if(retVal==-1)
	{
		perror("ERROR: fork() failed\n");
		exit(0);
	}
	else
	if(retVal==0)
	{
		//procArgv[0]="ls";
//		printf("exec cmd :%s\n",procArgv[0]);
		if(execvp(procArgv[0],procArgv)==-1)
		{
			perror("ERROR: Invalid command\n");
			exit(-1);
		}
		
	}
	//return 0;
	
}
