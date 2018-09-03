#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>

int main(int argc, char* argv[])
{
	
	int pfd[2];
	pipe(pfd);

	int pid=fork();
	if(pid==0)
	{
		
		close(pfd[0]);											//don't need to read from pipe2
		close(1);												//closing stdout
		dup(pfd[1]);											//mapping stdout to pipe2's write end
		execlp("/bin/grep","grep","-n",argv[1],argv[2],NULL);
						
		
		exit(0);
	}
	else
	{

		
		wait(NULL);
		
		close(pfd[1]);											//closing pipe2's write end
		close(0);												//closing stdin
		dup(pfd[0]);											//mapping stdin to pipe2's read end
	
		execlp("cut","cut","-f1","-d:", NULL);
		//execlp("bin/cut","cut","--d:","-f","1",NULL);	
		
		exit(0);
	}
	
}