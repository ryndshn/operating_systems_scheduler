// Ryan DuShane 
// Professor Kanchi
// CS451 
// Assignment 2
// linkedlist.c
// Taken from my CS202 assignment 2, and adjusted for use here.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "A2.h"

// Print list to standard output.
void printList(struct linkedList *linkedListVariable) {
   struct node *nodeVariable = linkedListVariable->head;
   printf("\n");
   // Loop from the beginning
   while(nodeVariable != NULL) {
      printf("key:%d process_number:%d arrival_time:%d burst:%d priorities:%d pid:%d is_running:%d\n", nodeVariable->key, nodeVariable->process_number, nodeVariable->arrival_time, nodeVariable->burst, nodeVariable->priorities, nodeVariable->pid, nodeVariable->is_running);
      nodeVariable = nodeVariable->next;
   }
}

// Inserts a new node at the end of the list.
void insertLast(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running) {
  struct node *nodeVariable = linkedListVariable->head;
  // Loop from the begining
  while(nodeVariable->next != NULL) {
    nodeVariable = nodeVariable->next;
  }
  struct node *newNode = (struct node*) malloc(sizeof(struct node));
  newNode->key = key;
  newNode->process_number = process_number;
  newNode->arrival_time = arrival_time;
  newNode->burst = burst;
  newNode->priorities = priorities;
  newNode->pid = pid;
  newNode->is_running = is_running;
  newNode->next = NULL;
  nodeVariable->next = newNode;
}

// Delete last element in the list.
void deleteLast(struct linkedList *linkedListVariable) {
  struct node *nodeVariable = linkedListVariable->head;
  struct node *nextNode = nodeVariable->next;
  int deleted = 0;
  // Loop through list.
  while(nodeVariable != NULL && deleted != 1) {
    if(nextNode->next == NULL) {
      nodeVariable->next = NULL;
    } else {
      nodeVariable = nodeVariable->next;
      nextNode = nextNode->next;
      deleted = 1;
    }
  }
}

// Deletes a list given a list pointer.
void deleteList(struct linkedList *linkedListVariable) {
  struct node *nodeVariable = linkedListVariable->head;
  // Loop from the beginning
  while(nodeVariable != NULL) {
     struct node *tempNext = nodeVariable->next;
     free(nodeVariable);
     nodeVariable = tempNext;
  }
  linkedListVariable->head = NULL;
}

// Initilize the list.
void initList(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running) {
  struct node *nodeVariable = (struct node*) malloc(sizeof(struct node));
  nodeVariable->key = key;
  nodeVariable->process_number = process_number;
  nodeVariable->arrival_time = arrival_time;
  nodeVariable->burst = burst;
  nodeVariable->priorities = priorities;
  nodeVariable->pid = pid;
  nodeVariable->is_running = is_running;
  nodeVariable->next = NULL;
  linkedListVariable->head = nodeVariable;
}

// Insert node at the beginning of the list
void insertFirst(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running) {
   // Create a node
   struct node *newNode = (struct node*) malloc(sizeof(struct node));
   newNode->key = key;
   newNode->process_number = process_number;
   newNode->arrival_time = arrival_time;
   newNode->burst = burst;
   newNode->priorities = priorities;
   newNode->pid = pid;
   newNode->is_running = is_running;
   // Point new node to old first node
   newNode->next = linkedListVariable->head;
   // Point the head to the new first node.
   linkedListVariable->head = newNode;
}

// Insert node in ordered location based on priority and arrival time.
void insertSorted(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running) { 
    insertFirst(linkedListVariable, key, process_number, arrival_time, burst, priorities, pid, is_running);
    sort(linkedListVariable); 
}

// Delete first item in the list
void deleteFirst(struct linkedList *linkedListVariable) {
   linkedListVariable->head = linkedListVariable->head->next;
}

// Checks if list is empty
bool isEmpty(struct linkedList *linkedListVariable) {
  return linkedListVariable->head == NULL;
}

// Counts the length of the list.
int length(struct linkedList *linkedListVariable) {
  int length = 0;
  struct node *nodeVariable = linkedListVariable->head;
  // Loop from the beginning
  while(nodeVariable != NULL) {
    nodeVariable = nodeVariable->next;
    length++;
  }
  return length;
}

// Sorts the list by priority or by arrival time and priority.
void sort(struct linkedList *linkedListVariable) {
  int temp_key, temp_process_number, temp_arrival_time, temp_burst, temp_priorities, temp_pid, temp_is_running;
  struct node *current = linkedListVariable->head;
  struct node *next = current->next;
  int listSize = length(linkedListVariable);
  int i = 0;
  while(i < listSize && next != NULL) {
    if(current->priorities > next->priorities || (current->priorities == next->priorities && current->arrival_time > next->arrival_time)) {
      // Swap process_number  
      temp_process_number = current->process_number;
      current->process_number = next->process_number;
      next->process_number = temp_process_number;
      // Swap arrival_time
      temp_arrival_time = current->arrival_time;
      current->arrival_time = next->arrival_time;
      next->arrival_time = temp_arrival_time;
      // Swap burst
      temp_burst = current->burst;
      current->burst = next->burst;
      next->burst = temp_burst;
      // Swap priorities
      temp_priorities = current->priorities;
      current->priorities = next->priorities;
      next->priorities = temp_priorities;
      // Swap pid
      temp_pid = current->pid;
      current->pid = next->pid;
      next->pid = temp_pid;
      // Swap is_running
      temp_is_running = current->is_running;
      current->is_running = next->is_running;
      next->is_running = temp_is_running;
      // Swap keys
      temp_key = current->key;
      current->key = next->key;
      next->key = temp_key;

      // Reset loop
      i = 1;
      current = linkedListVariable->head;
      next = current->next;
    } else {
      i++;
      current = current->next;
      next = next->next;
    }
  }
}

// Delete a node given a key.
void delete(struct linkedList *linkedListVariable, int key) {
   struct node* current = linkedListVariable->head;
   struct node* previous = NULL;
   // While loop through list
   while(current->key != key && current->next != NULL) {
     previous = current;
     // Go to next node
     current = current->next;
   }
   // If match is the head, replace the node
   if(current == linkedListVariable->head) {
      // Change first to point to next node
      linkedListVariable->head = linkedListVariable->head->next;
   }
   // If node to be delted is not the head, replace.
   else {
      // Skip the current node
      previous->next = current->next;
   }
}

