#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_value = 42; /* external variable in initialized data */
int main(void)
{
	int pid = 0;
	int status = 0;
        
	if ( (pid = fork()) < 0 )
        {
                fprintf(stderr,"fork error");
                return -1;
        }
	else if( pid !=0 )
	{
		fprintf(stderr,"Child created and its PID = %d\n",pid);
	}
	usleep(150000);

	if(pid !=0 )
        {
                fprintf(stderr,"I am the parent my PID = %d and my_value = %d\n",getpid(),my_value);
                wait(&status);
                if( status == 0 )
                {
                        fprintf(stderr,"Child terminated normally.It's PID was %d\n",pid);
                }
                else
                {
                        fprintf(stderr,"Child did not terminate normally!\n");
                }
        }


        if (pid == 0)     
        { /* child */

                my_value=18951; /* modify variables */
                fprintf(stderr,"I am the Child my PID = %d and my_value = %d\n",getpid(),my_value);
                usleep(500000);
        } 
        
/*        if(pid !=0 )    
        {
                fprintf(stderr,"I am the parent my PID = %d and my_value = %d\n",getpid(),my_value);
                wait(&status);
                if( status == 0 )
                {
                        fprintf(stderr,"Child terminated normally.It's PID was %d\n",pid);
                }
                else
                {
                        fprintf(stderr,"Child did not terminate normally!\n");
                }
        }*/
        return 0;
}
