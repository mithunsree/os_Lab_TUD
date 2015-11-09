#include <stdio.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAXLENGTH 100
#define MAXLINE 50
#define MYMSGQUEUENAME "/DEEDS_lab1_mq"


int my_value = 42; /* external variable in initialized data */

int main(void) {
	//pid_t pid;
	int n;
	
	int pid;
	char line[MAXLINE];
	int msgq_id;
	int status = 0;
	char name1[MAXLINE];
	char name2[MAXLINE];
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 100;
	attr.mq_curmsgs = 0;

	msgq_id = mq_open( MYMSGQUEUENAME, O_CREAT | O_RDWR, 0777, &attr);

	/*if (pipe(fd) < 0)
	 {
	 fprintf(stderr,"pipe error");
	 }*/

	if (msgq_id == -1) {
		perror("msgget failed with error\n");
		exit(0);
	}

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
	} else if (pid > 0) { /* parent */
		//fprintf(stderr, "Hi, I am your parent. My PID = %d, my_value = %d\n", getpid(), my_value);
		

		msgq_id = mq_open(MYMSGQUEUENAME, O_WRONLY);
		sprintf(name1,"Hi, I am your parent. My PID=%d and my_value=%d",getpid(), my_value);
		
		status = mq_send(msgq_id, (const char*) name1,MAXLENGTH , 1);
		//sleep(2);
		if (status != 0) {
			fprintf(stderr, "Sending failed\n");
		} else {
			fprintf(stderr, "Sending successful\n");
			fprintf(stderr, "Msg:%s\n", name1);
		}
		mq_close(msgq_id);
	}

	else {
		sleep(3); /* child */
		my_value = 18951;
		msgq_id = mq_open(MYMSGQUEUENAME, O_RDONLY);
		status = mq_receive(msgq_id, name2,MAXLENGTH, NULL);
		if (status >= 0) {
			fprintf(stderr, "Receiving sucess\n");
			fprintf(stderr, "Name 2 = %s\n", name2);
		} else if (status == -1) {
			fprintf(stderr, "Message receiving failed\n");
		}
		mq_close(msgq_id);
	}
	if (pid != 0) {
		wait();
		mq_unlink(MYMSGQUEUENAME);
		fprintf(stderr, "I am the parent:");
	}
	fprintf(stderr, "pid = %d, my_value = %d\n", getpid(), my_value);
	exit(0);
}
