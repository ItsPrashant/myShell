#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define MAX_PROC_ARGLEN 30
#define MAX_PROC_ARG 100

char buff[200];
char *cmd;

int processCmd(char *cmd2);
char *procArgv[100];
/*
int main()
{
	size_t len=100;
	if(fgets(buff,len,stdin)!=NULL)
	{
		printf("%s",buff);
	}
	//char *str;
	char del=';';
	int i=0;
	cmd=strtok(buff,&del);
	while(cmd!=NULL)
	{
		printf("%s\n",cmd);
		procArgv[i++]=strdup(cmd);
		cmd=strtok(NULL,&del);
	
	}
	while(i>0)
	{
		processCmd(procArgv[--i]);
	}

}


int processCmd(char *cmd2)
{
	printf("Process cmd -\n");

	char *procArgv[MAX_PROC_ARG];
	printf("cmd: %s\n",cmd2);
	
//	char *cmd2=cmd;
	char localDelim=' ';
	int noOfArg=0;
	procArgv[noOfArg++]=strtok(cmd2,&localDelim);

	while(procArgv[noOfArg-1]!=NULL)	
	{
		printf("i= %d : %s \n",noOfArg-1,procArgv[noOfArg-1]);
	
		procArgv[noOfArg++]=strtok(NULL,&localDelim);
	}
	printf("NoOfArg: %d\n ",noOfArg);
	if(noOfArg>1&&!strcmp(procArgv[noOfArg-2],"\n"))
	{
		printf("Comp result: %d\n",strcmp(procArgv[noOfArg-2],"\n"));
	
		procArgv[noOfArg-2]=NULL;
		noOfArg--;
	}
	
	return 0;
}
*/

int main()
{
	
	char *argv[10];
	argv[0]="cd";
	argv[1]="/home/";
	if(execvp(argv[0],argv)==-1)
	{
		printf("Error\n");
	}
	
	
}

