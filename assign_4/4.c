#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
int main()
{
    int n,k,r;
    scanf("%d %d %d",&n,&k,&r);  //input as asked
    k=k+1;
    int pid1,pid2;  
    int pfdk[2];
    char k_val[100];
    sprintf(k_val,"%d",k);  //conversion of k to string so that it can be put in execlp
    while(1)
    {
        pipe(pfdk);			// creation of pipe to send and receive pid of process to be killed
        pid1=fork();   
        if(pid1==0)	       // using child for printing the top process and killing the child as given by parent
        {
        	int count=0;
        	while(count<r)
        	{
            	int status;
            	int pfd1[2];
	            pipe(pfd1);   // creation of pipe to read and write the ps command after sorting
	            pid2=fork();
	            if(pid2==0)
	            {
	                if (dup2 (pfd1 [1], STDOUT_FILENO) == -1)     // opening the pipe for output
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
	                if (dup2 (pfd1 [0], STDIN_FILENO) == -1)   // opening the pipe for input
	                        perror ("dup2");
	                if (close (pfd1 [1]) == -1)
	                    perror ("close");
	                if (close (pfd1 [0]) == -1)
	                    perror ("close");
	                execlp("head","head","-n",k_val,NULL);
	            }
        		close(pfd1[0]);
        		close(pfd1[1]);
        		waitpid(pid2,NULL,0);     // waiting for the children of the child to complete for printing the top k process
        		waitpid(pid3,NULL,0);
        		
        		count++;
			sleep(n);
      		}
   			
    		char pidk[100];
    		close(pfdk[1]);
    		read(pfdk[0],pidk,sizeof(pidk));
    		if(strcmp(pidk,"-1")!=0)
    			printf("The Process Killed is %s\n",pidk);
   			if(strcmp(pidk,"-1")==0)
    		{
        		execlp("echo","echo","No Process Killed",NULL);
    		}
           	execlp("kill","kill",pidk,NULL);   // killing the child whose pid is given by the parent
      	}
    	else
    	{
            char pidk[100];
            scanf("%s",pidk);  //scanning the pid to be killed
            close(pfdk[0]);
            write(pfdk[1],pidk,strlen(pidk)+1);
            sleep(5);
    	}
    }
}

