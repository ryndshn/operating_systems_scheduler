// Ryan DuShane 
// Professor Kanchi
// CS451 
// Assignment 2
// prime_calc.c
// Program that determines the next prime number for a given burst time.

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define START_NUMBER 99999999

int pid;
struct itimerval process_time;
long process_time_long;
struct itimerval cpu_time;
struct tm* current_time;
time_t milliseconds;
long last_prime = START_NUMBER;

void process_complete(int);
void handle_timer_expiration(int);
long next_prime(long);
int is_prime(long);
void process_will_suspend(int);
void process_continue(int);

int main(int argc, char *argv[]) {
    int burst = atoi(argv[1]);
    
    // Tripped when process is about to be suspended.
    if(signal(SIGTSTP, process_will_suspend) == SIG_ERR) {
        printf("\n\n\n SIGTSTP ERR\n\n\n");
    }
    // Tripped when process needs to be conintued.
    signal(SIGCONT, process_continue);
    // Tripped when process has completed running for its burst.	
    signal(SIGTERM, process_complete);
    // Tripped when timer has expired.
    signal(SIGPROF, handle_timer_expiration);

    cpu_time.it_value.tv_sec = burst;
    cpu_time.it_interval.tv_sec = burst;

    setitimer(ITIMER_PROF, &cpu_time, NULL);
    pid = getpid();
    milliseconds = time(NULL);
    current_time = localtime(&milliseconds);

    // Continue running until we get the signal to stop.
    while(1) {
        last_prime = next_prime(last_prime);
    }
}

// Function used to start execution again after pause.
void process_continue(int signal) {
    return;
}

// Finds next prime.
long next_prime(long number) {
    number++;
    long i = number;
    while(1) {
	if(is_prime(i)) {
	   return i;	
	}
	i++;
    }
}

// Determines if the input is a prime number.
int is_prime(long number) {
    int i;
    for(i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
           return 0;
        }
    }
    return 1;
}

// Function is called on SIGTSTP. Prints information before a suspend happens.
// Uses pause to stop execution until process receives another signal.
void process_will_suspend(int signal) {
    getitimer(ITIMER_PROF, &process_time);
    process_time_long = process_time.it_value.tv_sec;
    milliseconds = time(NULL);
    current_time = localtime(&milliseconds);
    printf("\nPrintout %d is about to be suspended... ", pid);
    printf("The current time is %02d:%02d:%02d hours.\n", current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
    printf(" A CPU time of %lu  milliseconds was used in the previous burst.", process_time_long*1000);
    printf("\nIn that time it was determined that %lu was a prime number.\n", last_prime);
    pause();
}

// Called when the burst time is complete and we are ready to output the final information.
void process_complete(int signal) {
    milliseconds = time(NULL);
    current_time = localtime(&milliseconds);
    printf("\nPrintout from process whose pid is %d. PID %d has completed its task.\n", pid, pid);
    printf(" The current time is %02d:%02d:%02d hours.", current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
    printf(" The CPU time of %lu milliseconds (=%lu seconds) was used by PID %d.", (process_time_long*1000), process_time_long, pid);
    printf(" In that time it was determined that %lu was a prime number.\n", last_prime);
    kill(getppid(), SIGUSR1);
    exit(0);
}

// Called when the timer has reached its expiration. 
void handle_timer_expiration(int signal) {
    getitimer(ITIMER_PROF, &process_time);
    process_time_long = process_time.it_value.tv_sec;
    kill(pid, SIGTERM);
}





