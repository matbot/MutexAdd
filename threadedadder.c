/* Mat McDade
 * Mutex Exercise
 * An exercise to show meaningful use of pthreads and mutex locks to ensure safe multithreaded
 * processing. Takes an integer command line argument, in the range 0 to 1000000000, and uses four
 * threads to sum the integers in the range from 0 to the argument value. Each thread uses a mutex
 * lock before accessing a shared variable that stores the sum value.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

//This struct will hold all arguments to be passed to threads; no global values.
struct threadArgs {
	int threadNumber;
	long int rangeStart;
	long int rangeEnd;
	unsigned long long int* totalSum;
	pthread_mutex_t* mutex;
};

//Takes a struct of args and adds integers in a specified range to a sum value without the use of a
//local sum variable. Avoids race conditions on the sum variable by locking a mutex prior to
//access.
void* addRange(void* args) {
	//result holds the return value of the mutex lock.
	int result;
	//cast the args back to the correct struct format.
	struct threadArgs* addArgs = (struct threadArgs*)args;

	printf("The range for thread %d is from %li to  %li.\n",
			addArgs->threadNumber, addArgs->rangeStart, addArgs->rangeEnd);

	for(int i=addArgs->rangeStart;i<addArgs->rangeEnd;i++) {
		//for each integer, lock the thread.
		if((result = pthread_mutex_lock(addArgs->mutex))!=0) {
			//return an error if mutex lock fails.
		}
		*addArgs->totalSum+=(i+1);
		//and unlock the thread.
		pthread_mutex_unlock(addArgs->mutex);
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_mutex_t sumMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t tid[4];
	int result;

	//The sum for range 1000000000 is long long.
	//This is the only variable that will hold a sum value for all threads.
	unsigned long long int sum = 0;

	//get cli arg.
	long int inputValue;
	inputValue = atoi(argv[1]);
	long int quartileValue;
	//I could probably do the rounding better to set quartiles, but it works for now.
	quartileValue = roundl(((long double)(inputValue-1))/4);
	//printf("%d\n",quartileValue);

	struct threadArgs* qThreadArgs = (struct threadArgs*)calloc(4,sizeof(struct threadArgs));
	if(qThreadArgs==NULL) {
		//return an error.
	}

	for(int i=0;i<4;i++) {
		qThreadArgs[i].threadNumber = i+1;
		qThreadArgs[i].rangeStart = quartileValue * (i);
		qThreadArgs[i].rangeEnd = quartileValue * (i+1);
		qThreadArgs[i].totalSum = &sum;
		qThreadArgs[i].mutex = &sumMutex;
	}
	//handle any excess range from rounded division.
	qThreadArgs[3].rangeEnd = inputValue;

	for(int i=0;i<4;i++) {
		if((result = pthread_create(&tid[i],NULL,addRange,(void*)&qThreadArgs[i]))!=0) {
			//report error and exit.
		}
	}

	for(int i=0;i<4;i++) {
		pthread_join(tid[i],NULL);	
	}

	free(qThreadArgs);

	printf("Sum of intergers from 0 to %li: %lli\n",inputValue,sum);
	return 0;
}
