#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <sys/time.h>
#include <pthread.h>

#include <sys/queue.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>
// 
const char rfc_2822[] = "%a, %d %b %Y %T %z";

typedef struct{
    pthread_mutex_t lock;
} thread_data_t;


static void timer_thread(union sigval sigval);
static bool setup_timer(int clock_id, timer_t timerid, unsigned int timer_period, struct timespec *start_time);
static inline void timespec_add(struct timespec *result, const struct timespec *ts_1, const struct timespec *ts_2);

timer_t timerid;
thread_data_t td;

int main(void){

	struct timespec start_time;
	struct sigevent sev;
	memset(&td, 0, sizeof(thread_data_t));

	if(pthread_mutex_init(&td.lock, NULL) != 0){
        syslog(LOG_ERR, "aesdsocket: %d, %s failed initializing thread mutex", errno, strerror(errno));
    }
	/* code */
    int clock_id = CLOCK_MONOTONIC;
    memset(&sev, 0, sizeof(struct sigevent));
    // Setup a call to timer_thread passing in the td structure as the sigev_value argument
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_value.sival_ptr = &td;
    sev.sigev_notify_function = timer_thread;
    if(timer_create(clock_id, &sev, &timerid) != 0 ){
        syslog(LOG_ERR, "aesdsocket: %d, %s failed to create timer", errno, strerror(errno));
    }

	setup_timer(clock_id, timerid, 5, &start_time);
	int i = 0;
	while(1){
		i++;
		printf("%d\n", i);//
		for(int j = 0; j < 100000000; j++){
			;//
		}
	}
	return 0;
}

/**
* A thread which runs every timer_period_ms milliseconds
* Assumes timer_create has configured for sigval.sival_ptr to point to the
* thread data used for the timer
* inspired by https://github.com/cu-ecen-5013/aesd-lectures/blob/master/lecture9/timer_thread.c
*/
static void timer_thread(union sigval sigval){
    thread_data_t *td = (thread_data_t*) sigval.sival_ptr;
    int ret = 0;
	// char outstr[50];
	// time_t t;
	// struct tm *tmp;
	// t = time(NULL);
	// tmp = localtime(&t);

	// fptr = fopen(datfile, "a+");
	// if(fptr == NULL){
	// 	syslog(LOG_ERR, "fopen failed to open file, errno: %s", strerror(errno));
	// }

    if(pthread_mutex_lock(&td->lock) != 0){
        printf("Error %d (%s) locking thread data!\n",errno,strerror(errno));
    } else {
		// if(strftime(outstr, sizeof(outstr), rfc_2822, tmp) == 0){
		// 	syslog(LOG_ERR, "aesdsocket: failed to get time, errno: %d", errno);
		// }
		// printf("timestamp:%s\n", outstr);
		// ret = printf("timestamp:%s\n", outstr);
		// if(ret < 0){
		// 	syslog(LOG_ERR, "sensor: failed to write time, errno: %d", errno);
		// 	printf("could not print timestamp to file\n");
		// }

		ret = system("./gettemp.sh");
		if(ret != 0){
			syslog(LOG_ERR, "sensor: failed to get sensor temp value, errno: %d", errno);
			perror("system(getsensor.sh)");
		}

        if(pthread_mutex_unlock(&td->lock) != 0){
            printf("Error %d (%s) unlocking thread data!\n", errno, strerror(errno));
        }
    }

	// // close file and print error to syslog if it fails
	// if(fclose(fptr) != 0){
	// 	syslog(LOG_ERR, "fclose failed to close file, errno: %s", strerror(errno));
	// }

}


/**
* Setup the timer at @param timerid (previously created with timer_create) to fire
* every @param timer_period seconds, using @param clock_id as the clock reference.
* The time now is saved in @param start_time
* @return true if the timer could be setup successfuly, false otherwise
* inspired by https://github.com/cu-ecen-5013/aesd-lectures/blob/master/lecture9/timer_thread.c
*/
static bool setup_timer(int clock_id, timer_t timerid, unsigned int timer_period, struct timespec *start_time){
    bool success = false;
    if (clock_gettime(clock_id,start_time) != 0) {
        printf("Error %d (%s) getting clock %d time\n", errno, strerror(errno), clock_id);
    } else{
        struct itimerspec itimerspec;
        itimerspec.it_interval.tv_sec = timer_period;
        itimerspec.it_interval.tv_nsec = 0;
        timespec_add(&itimerspec.it_value, start_time, &itimerspec.it_interval);

        if(timer_settime(timerid, TIMER_ABSTIME, &itimerspec, NULL ) != 0) {
            printf("Error %d (%s) setting timer\n", errno, strerror(errno));
        } else{
            success = true;
        }
    }
    return success;
}


/**
* set @param result with @param ts_1 + @param ts_2
* inspired by https://github.com/cu-ecen-5013/aesd-lectures/blob/master/lecture9/time_functions_shared.h
*/
static inline void timespec_add( struct timespec *result, const struct timespec *ts_1, const struct timespec *ts_2){
    result->tv_sec = ts_1->tv_sec + ts_2->tv_sec;
    result->tv_nsec = ts_1->tv_nsec + ts_2->tv_nsec;
    if(result->tv_nsec > 1000000000L){
        result->tv_nsec -= 1000000000L;
        result->tv_sec ++;
    }
}