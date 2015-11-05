/*#include <stdio.h>
#include<pthread.h>

//#include "types.h"

#define MAXLINE 50

void* my_thread_function(void *arg);
int main(void)
{
pthread_t my_thread_id;
int err,err1;
err = pthread_create(&my_thread_id, NULL, my_thread_function, NULL);


err1 = pthread_join(my_thread, NULL);
return 0;
}

void* my_thread_function(void *arg) {

fprintf(stderr,"inside Thread");
pthread_exit(0);

}
*/
