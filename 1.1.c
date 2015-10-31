#include <stdio.h>
#include <stdlib.h>
//#include "types.h"
int glob = 6; /* external variable in initialized data */
int main(void)
{
	//pid_t pid;
	int pid = 0;
	if ((pid = fork()) < 0) 
	{
		fprintf(stderr,"fork error");
	} 
	else if (pid == 0) 
	{ /* child */
		fprintf(stderr,"I am the Child:");
		glob++; /* modify variables */
	} 
//	else 
//	{
//		sleep(2); /* parent */
		wait();
//	}
	if(pid !=0 )	
	{
		fprintf(stderr,"I am the parent:");
	}
	fprintf(stderr,"pid = %d, glob = %d\n", getpid(), glob);
	exit(0);
}
