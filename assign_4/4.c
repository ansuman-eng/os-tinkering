#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
int main()
{
	int n,k,r;
	scanf("%d %d %d",&n,&r,&k);
	k=k+1;
	int pid1,pid2;
	int pfdk[2];
	char k_val[100];
	sprintf(k_val,"%d",k);
	while(1)
	{
		pipe(pfdk);
		pid1=fork();
		if(pid1==0)
		{
		int count=0;
		while(count<r)
		{
			int status;
			int pfd1[2];
			pipe(pfd1);
			pid2=fork();
			if(pid2==0)
			{
				if (dup2 (pfd1 [1], STDOUT_FILENO) == -1) 
	        	perror ("dup2");
	    		if (close (pfd1 [1]) == -1) 
				perror ("close"); 
				if (close (pfd1 [0]) == -1) 
				perror ("close"); 
				execlp("/bin/ps","ps","-eo","pid,ppid,cmd,%mem,%cpu","--sort=-%cpu",NULL);
			}
			int pid3=fork();
			if(pid3==0)
			{
				if (dup2 (pfd1 [0], STDIN_FILENO) == -1) 
						perror ("dup2"); 
				if (close (pfd1 [1]) == -1) 
					perror ("close"); 
				if (close (pfd1 [0]) == -1) 
					perror ("close"); 
				execlp("head","head","-n",k_val,NULL);
		}
		close(pfd1[0]);
		close(pfd1[1]);
		waitpid(pid2,NULL,0);
		waitpid(pid3,NULL,0);
		sleep(5);
		count++;
		
	}
	sleep(n);
	char pidk[100];
	close(pfdk[1]);
	read(pfdk[0],pidk,sizeof(pidk));
	printf("%s\n",pidk);
	execlp("kill","kill",pidk,NULL);
		
	}
	else
	{
			char pidk[100];
			scanf("%s",pidk);
			close(pfdk[0]);
			write(pfdk[1],pidk,strlen(pidk)+1);
			sleep(10);
	}
	}
}