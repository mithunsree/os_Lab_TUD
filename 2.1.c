#include <stdio.h>

//#include "types.h"

#define MAXLINE 50

int glob = 6; /* external variable in initialized data */
int main(void)
{
	//pid_t pid;
	int n;
	int fd[2];
	int pid;
	char line[MAXLINE];

	if (pipe(fd) < 0)
	{
		fprintf(stderr,"pipe error");
	}

	if ((pid = fork()) < 0) 
	{
		fprintf(stderr,"fork error");
	} 
	else if (pid > 0) 
	{ /* parent */
		fprintf(stderr,"I am the Child:");
		glob++; /* modify variables */
		close(fd[0]);
		write(fd[1], sprintf(line,"Hi, I am your parent. My PID=%d and my_value=%d\n",getpid(),glob), 46);
	//	close(fd[0]);
	//	n = read(fd[1], line, MAXLINE);
//		sleep(5);
	///	write(1,line,n);

	} 
	else 
	{
		sleep(2); /* child */
//		wait();
		close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(1, line, n);
	}
	if(pid !=0 )	
	{
		//wait();
		sleep(3);
		fprintf(stderr,"I am the parent:");
	}
	fprintf(stderr,"pid = %d, glob = %d\n", getpid(), glob);
	exit(0);
}
