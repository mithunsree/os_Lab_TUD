#include <stdio.h>
#include <pthread.h>  //header file for thread functions


void* my_thread_function(void *arg);
int my_value=42;
int main(void)
{
	pthread_t my_thread_id;
	fprintf(stderr,"Inside Main Thread\n");
	int err,err1;
	fprintf(stderr,"My Value inside main thread before join is :%d \n",my_value);
	pthread_t id = pthread_self(); // to get thread id of main thread

	fprintf(stderr,"Main thread id %lu\n", id);

	err = pthread_create(&my_thread_id, NULL, my_thread_function, NULL);  //create a new thread
	usleep(150);
	
	if(err==0) //Thread creation is successfull
	{	fprintf(stderr,"New Thread's Id is %lu\n", (unsigned long) my_thread_id);  
		err1 = pthread_join(my_thread_id, NULL);  //main thread waits till new thread is executed.
		
		if(err1==0)  // Thread joining successfull
		{
			fprintf(stderr,"Thread joined successfully \n");
			fprintf(stderr,"My Value inside main thread after join :%d \n",my_value);
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
	my_value = 18951;
	fprintf(stderr,"Inside New Thread \n");
	fprintf(stderr,"My Value inside new thread is : %d \n",my_value);
	usleep(500);
	pthread_exit(0);

}
