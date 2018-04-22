/* 
 * File:   deadlockAvoidance.c
 * Author: zinth
 * 
 * About: Program to show deadlock avoidance using timers when a process is starved.
 *
 * Created on April 20, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <asm/errno.h>

#define MAX_WAIT_TIME (6)

pthread_t thread[2];
pthread_cond_t timeout;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int lock = 0;

//prototypes
void create_thread(int i);

void print_e(char* str){
    fprintf(stderr, "%s...\n", str);
}

void print_l(char* str){
    fprintf(stdout, "%s\n", str);
}

/*
 *  --- Error Log Handling ---
 */
void unlock_error(const int unlock_e){
    print_e("Mutex unlock error detected");
    switch (unlock_e){
        case EINVAL:
            print_e("ME01: Value does not refer to initialized mutex");
            break;
        case EAGAIN:
            print_e("ME02: Mutex not acquired: max recursive locks has been exceeded");
            break;
        case EBUSY:
            print_e("ME03: Mutex not acquired: already unlocked");
            break;
        case EPERM:
            print_e("ME04: The current thread does not own the mutex");
            break;
        default:
            break;
    }
    
}

void lock_error(const int lock_e){
    print_e("Mutex lock error detected");
    switch(lock_e){
        case EINVAL:
            print_e("ME05: Invalid mutex: Value does not refer to initialized mutex");
            break;
        case EAGAIN:
            print_e("ME06: Mutex not acquired: max recursive locks has been exceeded");
            break;
        case EBUSY:
            print_e("ME07: Mutex not acquired: already locked");
            break;
        case EDEADLK:
            print_e("ME08: Deadlock detected or thread owns mutex already");
            break;
        default:
            break;
            
    }
}

void signal_error(const int signal_e){
    print_e("Condition signal error detected");
    if(signal_e == EINVAL) print_e("CE01: Condition value does not refer to the initialised condition");
}

void cancel_error(const int cancel_e){
    print_e("Thread cancelability error detected");
    if(cancel_e == EINVAL) print_e("TE01: Invalid value for type");
}

void create_error(const int create_e){
    print_e("Thread creation error detected");
    switch(create_e){
        case EAGAIN:
            print_e("TE02: Not enough resources to create thread");
            break;
        case EINVAL:
            print_e("TE03: Invalid settings in attributes");
            break;
        case EPERM:
            print_e("TE04: No permission to set parameter/scheduling policy");
            break;
        default:
            break;
    }
}

void timewait_error(const int timewait_e){
    print_e("Time-wait condition error detected");
    switch(timewait_e){
        case ETIMEDOUT:
            print_e("CE02: Time specified to wait has passed");
            break;
        case EINVAL:
            print_e("CE03: Invalid value in abstime, cond or mutex");
            break;
        case EPERM:
            print_e("CE04: The mutex is not owned by the thread at time of call");
            break;
        default:
            break;
    }
}

void join_error(const int join_e){
    print_e("Thread join error detected");
    switch(join_e){
        case EINVAL:
            print_e("TE05: Value specified by thread does not refer to joinable thread");
            break;
        case ESRCH:
            print_e("TE06: Thread ID not found");
            break;
        case EDEADLK:
            print_e("TE07: Deadlock detected or thread already called");
            break;
        default:
            break;
    }
}

void gettime_error(const int gettime_e){
     print_e("Clock get time error detected");
    switch(gettime_e){
        case EFAULT:
            print_e("GTE01: tp points outside the accessible address space");
            break;
        case EINVAL:
            print_e("GTE02: Unsupported clock id");
            break;
        case EPERM:
            print_e("GTE03: Permission not given to clock_settime()");
            break;
        default:
            break;
    }
}

void timeout_lock(){
    //If lock ever = 1 then the mutex is locked and thread should trigger timeout condition
    if(lock != 0){
        timewait_error(0);
        
        print_l("Waiting a few seconds before restarting the thread");
        sleep(rand() % MAX_WAIT_TIME);
        pthread_exit((void**)1);
    }else{
        const int lock_e = pthread_mutex_lock(&mutex);
        if(lock_e){
            lock_error(lock_e);
        }else{
            print_l("Mutex locked to thread!");
            lock = 1;
        }
    }  
}

void timeout_unlock(){
    const int unlock_e = pthread_mutex_unlock(&mutex);
    if(unlock_e){
        unlock_error(unlock_e);
    }else{
        print_l("Mutex unlocked from thread!");
        lock = 0;
    }
}

/*
 *  --- Thread Function ---
 */
void *thread_run(void *result){
    print_l("Thread start:");
    
    print_l("Attempting to lock resource to thread.");
    timeout_lock();
    
    //THREAD CRITICAL SECTION START
    print_l("Start of threads critical section.");
    
    sleep(rand()% 10); // sleep for 0-10 seconds to simulate process working on critical section
    
    print_l("End of thread critical section.");
    
    print_l("Unlocking resource from thread");
    //try and unlock mutex
    timeout_unlock();
    
    pthread_exit(0);    
}

void *thread_walk(void *result){
    print_l("Thread start:");
    
    print_l("Attempting to lock resource to thread.");
    timeout_lock(1);
    
    //THREAD CRITICAL SECTION START
    print_l("Start of threads critical section.");
    
    sleep(rand()% 10); // sleep for 0-10 seconds to simulate process working on critical section
    
    print_l("End of thread critical section.");
    
    print_l("Unlocking resource from thread");
    //try and unlock mutex
    timeout_unlock();
    
    pthread_exit(0);    
}




/*
 * --- Create Thread Function ---
 */
void create_thread(int i){
    if(i == 0){
        const int create_e = pthread_create(&thread[0], 0, &thread_run, 0);
        if(create_e){
            create_error(create_e);
        }else{
            print_l("Thread created.");
        }
    }else{
        //create thread using running for every odd number
        const int create_e = pthread_create(&thread[1], 0, &thread_walk, 0);
        if(create_e){
            create_error(create_e);
        }else{
            print_l("Thread created.");
        }
    }
}

/*
 *  --- Main Function ---
 */
int main(int argc, char** argv) {
    int status;
    
    pthread_cond_init(&timeout, 0);
    
    create_thread(0);
    create_thread(1);
    
    pthread_join(thread[0], (void**)&status);
    if(status != 0){
        print_l("Restarting run thread.");
        create_thread(0);
    }
    
    pthread_join(thread[1], (void**)&status);
    if(status != 0){
        print_l("Restarting walk thread.");
        create_thread(1);
    }
    
   
    return (EXIT_SUCCESS);
}

