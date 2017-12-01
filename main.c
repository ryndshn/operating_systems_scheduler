// Ryan DuShane 
// Professor Kanchi
// CS451 
// Assignment 2
// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "A2.h"

// Keeps track of whether or not there are still processes to be read.
int processes_available;
// Global linkedList is required to manipulate list from signal.
struct linkedList processes_list;
struct linkedList processes_queue;
// Keep track of seconds.
int current_second;
// Keep track of current process time for gantt
int current_process_time;
// Keep file pointer for final output.
FILE* gantt;

int main() {
    signal(SIGUSR1, process_complete);

    gantt = fopen("gantt.txt", "w");

    char file_name[50];
    printf("Input file name: ");
    scanf("%s", file_name);
    char* line = NULL;
    size_t len = 0;
    ssize_t read; 
    FILE* filep = fopen(file_name, "r");
    int iteration = 0;
   
    while((read = getline(&line, &len, filep)) != -1) {
       fprintf(gantt, "%s", line); 
       
        char** split_line = string_split(line, ' ');
        if(strcmp(split_line[0], "Process") != 0) {
            
            if(iteration == 0) {
	    	    initList(&processes_list, iteration, atoi(split_line[0]), atoi(split_line[1]), atoi(split_line[2]), atoi(split_line[3]), 0, 0);
                processes_available = 1;
	        } else {
		        insertLast(&processes_list, iteration, atoi(split_line[0]), atoi(split_line[1]), atoi(split_line[2]), atoi(split_line[3]), 0, 0);
	        }
	        iteration++;
        }
    }
    printList(&processes_list);
    run_timer();
    fclose(filep);
}

// Called when the current process finishes, and signal is sent.
void process_complete(int signal) {
    // Print gantt chart here; process is finished, and final gantt information can be outputted.
    print_gantt();
    kill(processes_queue.head->pid, SIGKILL);
    // Delete the first process in the queue. 
    // This should be the one that is currently running.
    deleteFirst(&processes_queue);
    if(processes_queue.head) {
        int kill_status = kill(processes_queue.head->pid, SIGCONT);
        if(kill_status == 0) {
            printf("PID %d resumed\n", processes_queue.head->pid);
        }
    }
}

// Sets up and runs timer. While loops through processes while there is still some to be read, 
// and while the queue still has tasks to run.
void run_timer() {
    struct itimerval time;
    signal(SIGALRM, handle_timer);
    time.it_value.tv_sec = 1;
    time.it_interval = time.it_value;
    setitimer(ITIMER_REAL, &time, NULL);
    current_second = 0;
    current_process_time = -1;
    while(processes_list.head || processes_queue.head) {
        // Loop through processes_list to determine
        while(processes_list.head && processes_list.head->arrival_time <= current_second) {
            handle_process(processes_list.head);
            deleteFirst(&processes_list);
        }
        pause();
    }
    deleteList(&processes_list);
    deleteList(&processes_queue);
    printf("QUITING\n");
    exit(0);
}

// Handles new process pulled from initial list. 
// This function decides which case it is, and handles it.
// Three cases, one for empty list, one for new process being of higher priority,
// and the last being a new process without higher priority. 
void handle_process(struct node* current_process) {

    char* args[3];
    args[0] = "./prime_calc.o";
    char burst[10];
    snprintf(burst, 10, "%d", current_process->burst);
    args[1] = burst;
    args[2] = NULL;

    // Queue is empty
    if(processes_queue.head == NULL) {
        int child = fork();
        if(child == 0) {
            execvp(args[0], args);
            printf("errno: %d\n", errno);
        } else {
            initList(&processes_queue, current_process->key, current_process->process_number, current_process->arrival_time, current_process->burst, current_process->priorities, child, 1);
        }
    }       

    // Arrived process has higher priority. 
    // Suspend current process, add process to the front of the queue,fork and exec new process.
    else if(higher_priority(current_process, processes_queue.head)) {
	// Print gantt chart here; before process suspension.
        print_gantt();
        kill(processes_queue.head->pid, SIGTSTP);
        int child = fork();
        if(child == 0) {
            execvp(args[0], args);
            printf("errno: %d\n", errno);
        } else {
            insertFirst(&processes_queue, current_process->key, current_process->process_number, current_process->arrival_time, current_process->burst, current_process->priorities, child, 1);
        }
    } 

    // Arrived process does not have higher priority.
    // Add to queue, sorted(make sure it is in the correct place in the list).
    else {    
        int child = fork();
        if(child == 0) {
            execvp(args[0], args);
            printf("errno: %d\n", errno);
        } else {
            kill(child, SIGTSTP);
            insertSorted(&processes_queue, current_process->key, current_process->process_number, current_process->arrival_time, current_process->burst, current_process->priorities, child, 1);
        }
    }
    //printList(&processes_queue);
}

// Prints gantt chart for current process, and resets the counter.
void print_gantt() {
    struct tm* current_time;
    time_t milliseconds;
    milliseconds = time(NULL);
    current_time = localtime(&milliseconds);
    // Print current iteration of gantt chart
    fprintf(gantt, "\n\nPID: %d\nProcess Number: %d\nDuration: %d\nTime: %02d:%02d:%02d", processes_queue.head->pid, processes_queue.head->process_number, current_process_time, current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
    current_process_time = 0;
}

int higher_priority(struct node* current_process, struct node* running_process) {
   if(current_process->priorities < running_process->priorities) {
        return 1;
   } else {
        return 0;
   }
}

// Increment counters used to keep time.
void handle_timer(int signal) {
    current_second++;
    current_process_time++;
} 

// Create array of strings to be used in parsing.
char** string_split(char* string_to_split, const char delimeter) {
    size_t count = 0;
    char* last_delimeter = 0;
    int i = 0;
    // Count the number of times the delimeter occurs
    while(string_to_split[i] != '\0') {
        if(delimeter == string_to_split[i]) {
            count++;
        }
        i++;
    }
    count = count + 2;
    char** result_array = 0;
    result_array = malloc(sizeof(char*) * count);
    char* token = strtok(string_to_split, &delimeter);
    size_t id = 0;
    while(token) {
        result_array[id++] = strdup(token);
        token = strtok(0, &delimeter);
    }
    result_array[id] = 0;
    return result_array;

}

// Get string length
int string_length(char** s) {
    int i = 0;
    while(s[i] != '\0') {
        i++;
    }
    return i;
}
