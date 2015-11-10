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


#define MAXLENGTH 100
#define MYSHMNAME "/DEEDS_lab1_shm"

int my_value = 42; /* external variable in initialized data */
int main(void)
{
        //int n;
        int pid;
//      char line[MAXLINE];
        //int msgq_id;
        int status = 0;
        int r = 0;
//      char name1[MAXLINE];
//      char name2[MAXLINE];
        const size_t region_size = sysconf(_SC_PAGE_SIZE);

        int fd = shm_open(MYSHMNAME, O_CREAT | O_TRUNC | O_RDWR, 0666);
        if (fd == -1)
        {
	fprintf(stderr,"shm_open failed\n");
	return -1;
        }

        r = ftruncate(fd, region_size);
        if (r != 0)
        {
                fprintf(stderr,"ftruncate failed\n");
		r = shm_unlink(MYSHMNAME);
		return -1;
        }

        void *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED)
        {
                fprintf(stderr,"Mapping Falied");
		r = shm_unlink(MYSHMNAME);
		return -1;
        }
        close(fd);

        if ((pid = fork()) < 0)
        {
                fprintf(stderr,"fork error\n");
		r = shm_unlink(MYSHMNAME);
		return -1;
        }
        //usleep(150000);

	if(pid > 0)
	{
                char *message1 = NULL;
                message1 = (char *) ptr;
                sprintf(message1,"Hi, I am your parent. My PID=%d and my_value=%d",getpid(),my_value);
        }
	
	if( pid == 0 )
	{
		char *message2 = NULL;
                //usleep(500000);
                my_value = 18951;
                fprintf(stderr,"I am the child.Parent wrote:\n %s\n",(char *) ptr);
                message2 = (char *)ptr;
                sprintf(message2,"Hi, I am your child. My PID=%d and my_value=%d",getpid(),my_value);
		exit(1);
	}

	if(pid > 1)
	{
		usleep(150000);
		//fprintf(stderr,"I am the Parent.Child wrote:\n %s\n",(char *) ptr);
//		if(WIFSTOPPED(status))
//		{
			fprintf(stderr,"I am the Parent.Child wrote:\n %s\n",(char *) ptr);	
//		}
        	r = munmap(ptr, region_size);
        	if (r != 0)
        	{
                	fprintf(stderr,"munmap failed");
			r=shm_unlink(MYSHMNAME);
			return -1;
        	}
        	r = shm_unlink(MYSHMNAME);
        	if (r != 0){
	        	fprintf(stderr,"shm_unlink failed");
			return -1;		
		}
	}
	wait(&status);
	return 0;
}
