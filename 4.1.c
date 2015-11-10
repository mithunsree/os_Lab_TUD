#include <stdio.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>


#define MAXLENGTH 100
#define MAXLINE 50
#define MYMSGQUEUENAME "/DEEDS_lab1_mq"


int my_value = 42;

int main(void) {
	
	
	int pid;
	int msgq_id;
	int status = 0;
	char name1[MAXLINE];
	char name2[MAXLINE];
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 100;
	attr.mq_curmsgs = 0;

	msgq_id = mq_open( MYMSGQUEUENAME, O_CREAT | O_RDWR, 0777, &attr);  //creates and opens the queue in Read write mode


	if (msgq_id == -1) {
		perror("mq_open failed with error\n");   //  open fails
		exit(0);
	}

	if ((pid = fork()) < 0) {
		fprintf(stderr, "Fork error\n");    // fork fails
		mq_close(msgq_id);
		mq_unlink(MYMSGQUEUENAME);
		exit(0);

	}
		usleep(150000); 

 	 if (pid > 0)
	{ /* parent */
			

		msgq_id = mq_open(MYMSGQUEUENAME, O_WRONLY);   //writes message into queue
		sprintf(name1,"Hi, I am your parent. My PID=%d and my_value=%d",getpid(), my_value);
		
		status = mq_send(msgq_id, (const char*) name1,MAXLENGTH , 1); //writes message into queue
		//sleep(2);
		if (status != 0) {
			fprintf(stderr, "Sending failed\n");
			mq_close(msgq_id);
			mq_unlink(MYMSGQUEUENAME);
			exit(0);
		} else {
			fprintf(stderr, "Msg:%s\n", name1);
			fprintf(stderr, "Sending successful\n");
			
		}
		wait(&status);
		fprintf(stderr, "I am Parent MY Pid = %d and my_value = %d\n", getpid(), my_value);
		mq_close(msgq_id);
		mq_unlink(MYMSGQUEUENAME);
	}

	else {
		sleep(3); /* child */
		usleep(500000);
		my_value = 18951;
		msgq_id = mq_open(MYMSGQUEUENAME, O_RDONLY);
		status = mq_receive(msgq_id, name2,MAXLENGTH, NULL);  //reads message from queue
		if (status >= 0) {
			fprintf(stderr, "Receiving sucess\n");
			fprintf(stderr, "I am Child Message. from my Parent is : %s\n", name2);
			fprintf(stderr, "I am Child MY Pid = %d and my_value = %d\n", getpid(), my_value);
		} else if (status == -1) {
			fprintf(stderr, "Message receiving failed\n");
			mq_close(msgq_id);
			mq_unlink(MYMSGQUEUENAME);
			exit(0);
		}
		mq_close(msgq_id);
	}
	if (pid != 0) {
		wait(&status);
		mq_unlink(MYMSGQUEUENAME);
		
	}
	
	
	exit(0);
}
