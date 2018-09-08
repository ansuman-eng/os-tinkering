#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<math.h>

int check(int * hash)
{
    for(int i=0;i<10;i++)
    if(hash[i]==0)
    return 0;
   
    return 1;
}
 
int main()
{
    int pid;
    int pdf[2];
    char temp[100];
    pipe(pdf);
    pid=fork();
    if(pid==0)
    {
        int sum = 0;
        int num = 0;
        read(pdf[0],temp,9999);				//Reading from pipe
        //printf("%s\n", temp);
        while(temp!="X")				// Terminate when parent sends X (it sends X when all the elements are already sent once and process is to be terminated)
        {
        num=0;
        
        for(int i =0;i<strlen(temp);i++)
        {
            num=num*10+(temp[i]-'0'); 			// converting string into integer
        }
        int sq=1;	
        while((sq+1)*(sq+1)<=num)			// finding square root of number
            {
                sq++;			
            }
        //printf("Square root is %d %d\n",num, sq);
        int boo = 0;	
        for(int i=2;i<=sq;i++)		// checking prime or not 
        {
            if(num%i==0)
            {
                boo = 1;
                break;
            }
        }
        if(boo!=1)		// add in sum if num is prime
        {
            sum+=num;
        }
        printf("current value of sum is %d\n",sum);
        sleep(num%3);
        read(pdf[0],temp,9999);
        }
    }
    else
    {
        char a[10][10]={"11","12","13","14","15","16","17","18","19","20"};
        int hash[10]={0};
        int x ;								// current
        int y ;								// next
        int curr_ind;
        int nex_ind=-1;
        while(check(hash)==0)						//Check if all elements are visited
        {
		curr_ind= rand()%10;					
		nex_ind=curr_ind;
		while(nex_ind==curr_ind)
		    nex_ind=rand()%10;
		hash[curr_ind]=hash[nex_ind]=1;
		//printf("x = %s & y = %s\n",a[curr_ind],a[nex_ind]);
		write(pdf[1],a[nex_ind],strlen(a[nex_ind])+1);		//Writing into pipe
		int num=0;
		for(int i =0;i<strlen(a[curr_ind]);i++)
		{
		    num=num*10+(a[curr_ind][i]-'0');			//converting string into number
		}
		sleep(num/3);
        }
        write(pdf[1],"X",2);						//writing terminating string
        
    }
}
