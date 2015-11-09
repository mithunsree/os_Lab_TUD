#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
//#include "types.h"
int my_value = 42; /* external variable in initialized data */
int main(void)
{
	//pid_t pid;
	int pid = 0;
        pid=fork();
        usleep(150000);
	if (pid < 0) 
	{
		fprintf(stderr,"fork error");
                fflush(stdout);
	} 
	else if (pid == 0) 
	{ /* child */
		fprintf(stderr,"I am the Child:");
                fflush(stdout);
//                 usleep(500000);
		my_value=18951; /* modify variables */
	} 
//	else 
//	{
	 usleep(500000);
//	sleep(2); /* parent */
		wait();
//	}
	if(pid !=0 )	
	{
		fprintf(stderr,"I am the parent:");
                fflush(stdout);
	}
	fprintf(stderr,"pid = %d, my_value = %d\n", getpid(), my_value);
        fflush(stdout);
	exit(0);
}
