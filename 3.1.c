#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Macros
#define MAXLINE 100

//Globals
int my_value = 42;

//The main
int main(void)
{
	int n;
	int fd[2];
	int pid;
	int status = 0;
	int status_wait = 0;
//	int i = 0;
	char snd_msg[MAXLINE]="";
	char rcv_msg[MAXLINE]="";

//	for(i=0;i<100;i++){	
	if (pipe(fd) < 0)
		fprintf(stderr,"pipe error");
	
	if ((pid = fork()) < 0) {
		fprintf(stderr,"fork error");
	}
	usleep(150000); 

	if (pid > 0) { /* parent */
		close(fd[0]);
		sprintf(snd_msg,"Hi, I am your parent. My PID=%d and my_value=%d",getpid(),my_value);
		status = write(fd[1],snd_msg,sizeof(snd_msg));
		if(status > 0)
		{
			fprintf(stderr,"I am the parent successfully sent the mesaage\n");
			fprintf(stderr,"Parent PID:%d | my_value:%d\n",getpid(),my_value);
		}
		else
		{
			return -1;
		}
		wait(&status_wait);
	} 
	else { /* child */
		usleep(500000);
		my_value = 18951;
		close(fd[1]);
		n = read(fd[0], rcv_msg, MAXLINE);
		if( n > 0 )
		{
			fprintf(stderr,"I am the child I successfully read the message:\n");
		}
		write(STDOUT_FILENO, rcv_msg, n);
		fprintf(stderr,"\nChild PID:%d | my_value:%d\n",getpid(),my_value);
	}
//}
	return 0;
}
