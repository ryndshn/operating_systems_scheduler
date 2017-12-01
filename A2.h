// Ryan DuShane 
// Professor Kanchi
// CS451 
// Assignment 2
// A2.h

struct node
{
   int process_number;
   int arrival_time;
   int burst;
   int priorities;
   int pid;
   int is_running;
   int key;
   struct node *next;
};

struct linkedList {
  struct node *head, *current;
};

// Prototypes
void print_gantt();
int higher_priority(struct node* current_process, struct node* running_process);
void handle_process(struct node* current_process);
void handle_timer(int signal);
void process_complete(int);
void run_timer(); 
void check_processes();
char** string_split(char* string_to_split, const char delimeter);
int string_length(char** s);
void printList(struct linkedList*);
void insertFirst(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running);
void deleteFirst(struct linkedList*);
void delete(struct linkedList*, int);
void sort(struct linkedList*);
void initList(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running);
void simplify(struct linkedList*);
void insertSorted(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running);
void multiply(struct linkedList*, struct linkedList*, struct linkedList*);
void deleteList(struct linkedList*);
bool isEmpty(struct linkedList*);
struct node* search(struct linkedList*, int);
void replaceElement(struct linkedList*, int, int);
void deleteLast(struct linkedList*);
void insertLast(struct linkedList *linkedListVariable, int key, int process_number, int arrival_time, int burst, int priorities, int pid, int is_running); 
