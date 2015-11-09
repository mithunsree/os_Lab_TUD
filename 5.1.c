#include <stdio.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>


#define MAXLINE 50
#define MYSHMNAME "/DEEDS_lab1_shm"

int glob = 6; /* external variable in initialized data */
int main(void)
{
	//pid_t pid;
	int n;
	int pid;
	char line[MAXLINE];
	int msgq_id;
	int status = 0;
	int r = 0;
	char name1[MAXLINE];
	char name2[MAXLINE];
	const size_t region_size = sysconf(_SC_PAGE_SIZE);

	int fd = shm_open(MYSHMNAME, O_CREAT | O_TRUNC | O_RDWR, 0666);
  	if (fd == -1)
    	{
		fprintf(stderr,"shm_open failed\n");
	}

  	r = ftruncate(fd, region_size);
  	if (r != 0)
    	{
		fprintf(stderr,"ftruncate failed\n");
	}

  	void *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 	if (ptr == MAP_FAILED)
    	{
		fprintf(stderr,"mmap");
	}
  	close(fd);

	if ((pid = fork()) < 0) 
	{
		fprintf(stderr,"fork error\n");
	}
	
	if (pid == 0) {
		sleep(3);
    		//u_long *d = (u_long *) ptr;
    		//*d = 0xdbeebee;
    		//exit(0);
		//fprintf(stderr,"Parent wrote:%s\n",*(char *)ptr);
		fprintf(stderr,"Parent wrote %s\n",(char *) ptr);
  	}
	else
	{
		int status;
//    		waitpid(pid, &status, 0);
    		//printf("child wrote %#lx\n", *(u_long *) ptr);
		char *name;
		name = (char *)malloc(sizeof(7));
//		sprintf(name,"sachin");
		name = (char *) ptr;
		sprintf(name,"sachin");
                //*d = 0xdbeebee;
		waitpid(pid,&status,0);
	}
	
	 r = munmap(ptr, region_size);
  	if (r != 0)
	{
    		fprintf(stderr,"munmap failed");
	}

  	r = shm_unlink(MYSHMNAME);
 	if (r != 0)
    	fprintf(stderr,"shm_unlink failed");
  	return 0;	
}
