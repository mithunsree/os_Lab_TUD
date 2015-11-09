#include <stdio.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 50
#define MYMSGQUEUENAME "/DEEDS_lab1_mq"

int glob = 6; /* external variable in initialized data */
int main(void)
{
	//pid_t pid;
	int n;
	int fd[2];
	int pid;
	char line[MAXLINE];
	int msgq_id;
	int status = 0;
	char name1[MAXLINE];
	char name2[MAXLINE];
	struct mq_attr attr;  
	attr.mq_flags = 0;  
	attr.mq_maxmsg = 50;  
	attr.mq_msgsize = 7;  
	attr.mq_curmsgs = 0; 

	msgq_id =  mq_open( MYMSGQUEUENAME,O_CREAT|O_RDWR, 0777,&attr);
	
	/*if (pipe(fd) < 0)
	{
		fprintf(stderr,"pipe error");
	}*/

	if (msgq_id == -1) {
	  perror("msgget failed with error\n");
	  exit(0);
	}

	if ((pid = fork()) < 0) 
	{
		fprintf(stderr,"fork error\n");
	}
	else if (pid > 0) 
	{ /* parent */
		fprintf(stderr,"I am the Parent.Sending message\n");
		glob++; /* modify variables */
	/*	close(fd[0]);
		write(fd[1], sprintf(line,"Hi, I am your parent. My PID=%d and my_value=%d\n",getpid(),glob), 46);
	//	close(fd[0]);
	//	n = read(fd[1], line, MAXLINE);
//		sleep(5);
	///	write(1,line,n);
	*/
		//name1 = (const char*)malloc(7);
		//sprintf(name1,"sachin")
		//snprintf(name1,MAXLINE,"Some blablabla");
		msgq_id = mq_open(MYMSGQUEUENAME, O_WRONLY);
		sprintf(name1,"sachin");		
		status = mq_send(msgq_id, (const char*)name1,7, 1);
		//sleep(2);
		if(status != 0)
		{
			fprintf(stderr,"Sending failed\n");
		}
		else
		{
			fprintf(stderr,"Sending successful\n");
			fprintf(stderr,"Msg:%s\n",name1);
		}
		mq_close(msgq_id);
	}
		 
	else 
	{
		sleep(3); /* child */
//		wait();
		/*close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(1, line, n);*/
		//name2 = (char *)malloc(7);
		msgq_id = mq_open(MYMSGQUEUENAME, O_RDONLY);
		status = mq_receive(msgq_id,name2,7,NULL);
		if(status >= 0)
		{
			fprintf(stderr,"Receiving sucess\n");
			fprintf(stderr,"Name 2 = %s\n",name2);
		}
		else if(status == -1)
		{
			fprintf(stderr,"Message receiving failed\n");
		}
		mq_close(msgq_id);
	}
	if(pid !=0 )	
	{
		wait();
		//sleep(3);
		mq_unlink(MYMSGQUEUENAME);
		fprintf(stderr,"I am the parent:");
	}
	fprintf(stderr,"pid = %d, glob = %d\n", getpid(), glob);
	//free(name1);
	//free(name2);
	exit(0);
}
