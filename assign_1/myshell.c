#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
/*
Commands to be implemented
1) cd directory - done
2) echo comment - done
3) ls directory - done
4) wc <option> <file>
option=l(lines),w(words),c(bytes)
5) quit - done
6) anything else - report error that command doesn't exist- done
7) Specific SIGINT handler - done
8) Show pwd constantly
*/
//Handle cd separately since it is a builtin
int my_shell_cd(char* args)
{
	if(args==NULL)
	{
		printf("Argument expected for \"cd\"\n");
	}
	else
	{
		if(chdir(args)!=0)
		{
			perror("Error in CD.\n");
		}
	}
	return 1;
}
int my_shell_sp(char *command, char *args)
{
	int pid;
	int status;
	pid=fork();
	if(pid<0)
	{
		printf("Error in forking\n");
	}
	if(pid==0)
	{
		if(strcmp(command,"ls")==0)
		{
			if(args==NULL)
				execlp("/bin/ls","ls",NULL);						//If no arguments, run ls for present directory
			else if(execlp("/bin/ls","ls",args,NULL)==-1)
				perror("Error in ls.\n");
				
		}
		else if(strcmp(command,"wc")==0)
		{
			if(args==NULL)
				{
					printf("Not enough arguments");
					exit(0);
				}
			else
			{
				char * option=strsep(&args," ");
				//printf("Options %s\n",option);
				//printf("File %s\n",args);
				if(execlp("wc","wc",option,args,NULL)==-1)
					perror("Error in wc.\n");

			}
		}
		else
		{
			if(execlp("/bin/echo","echo",args,NULL)==-1)
				perror("Error in echo.\n");
		}
	}
	else
	{
		wait(&status);
		return 1;
	}
}
void sigint_handler(int sigint_num)
{
	printf("You can't exit with Ctrl+C. Please type in the \"quit\" command.\n");
	return;
}
char* pres_dir()
{
	int pfd[2];
	pipe(pfd);
	int pid=fork();
	if(pid==0)
	{
		close(pfd[0]);
		close(1);
		dup(pfd[1]);
		execlp("/bin/pwd","pwd",NULL);
	}
	else
	{
		wait(NULL);
		close(pfd[1]);
		char *buffer=(char*)malloc(1000*sizeof(char));
		read(pfd[0],buffer,1000);
		return buffer;
	}
}
int main()
{
	
	printf("				****Welcome to MyShell****\n");
	//Basic loop start
	signal(SIGINT,sigint_handler);
	while(1)
	{
		char *pwd=pres_dir();
		//printf("%d",pwd[strlen(pwd)-1]=='\n');
		pwd[strlen(pwd)-1]='\0';
		printf("(%d) (%s) >>> ",getpid(),pwd);
		char line[1000], *command, *temp_line, buf_clear;
		line[0]='\0'; //clear it out		
		scanf("%[^\n]s ",line);
		scanf("%c",&buf_clear); //clear out the buffer
		//printf("%s\n",line);
		temp_line=line;

		if(strcmp(temp_line,"quit")==0)
			break;

		command=strsep(&temp_line," ");
		//printf("%s\n",command);
		
		if(strcmp(command,"ls")==0 || strcmp(command,"echo")==0 || strcmp(command,"wc")==0)
		{
			my_shell_sp(command,temp_line);
		}
		else if(strcmp(command,"cd")==0)
		{			
			if(temp_line==NULL)
				printf("%s\n",pwd);
			my_shell_cd(temp_line);
		}
		else 
		{
			printf("The command doesn't exist\n");
		}		

	}
	
	return 0;
}
