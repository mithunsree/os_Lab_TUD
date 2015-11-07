#include <stdio.h>
#include<pthread.h>  //header file for thread functions


void* my_thread_function(void *arg);

int main(void)
{
	pthread_t my_thread_id;
	fprintf(stderr,"Inside Main Thread\n");
	int err,err1;
	err = pthread_create(&my_thread_id, NULL, my_thread_function, NULL);  //create a new thread
	
	if(err==0) //Thread creation is successfull
	{
		err1 = pthread_join(my_thread_id, NULL);  //main thread waits till new thread is executed.

		if(err1==0)  // Thread joining successfull
		{
			fprintf(stderr,"Thread joined successfully");
		}
		else
		{	
			fprintf(stderr,"Thread join failed");
		}
	}
	else
	{
		fprintf(stderr,"Thread creation failed");
	}


	return 0;
}

void* my_thread_function(void *arg) 

{

	fprintf(stderr,"Inside Thread \n");
	pthread_exit(0);

}
