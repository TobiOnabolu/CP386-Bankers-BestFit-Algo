#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>

void begin();
void print_status();
int* add_rows(int *work, int *allocation);
bool isless(int *work, int *need);
int* subtract_rows(int *work, int *allocation);
void print_thread(int arr[][4], int i);
char* get_input();
char** split_word(char *input, int length);
int get_arg_count(char *input);
int* safe_sequence();
int* create_copy(int *arr);
bool is_safe(int *finish);
bool request_granted(int p, int *request);
bool release_granted(int p, int *request);
void handle_threads(int *finish);
void* runner(void *thread_id);

//hardcoded matrices
int max_needs[5][4] = { { 6, 4, 7, 3 }, { 4, 2, 3, 2 }, { 2, 5, 3, 3 }, { 6, 3,
		3, 2 }, { 5, 5, 7, 5 } };
int allocated[5][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0,
		0, 0 }, { 0, 0, 0, 0 } };
int needs[5][4] = { { 6, 4, 7, 3 }, { 4, 2, 3, 2 }, { 2, 5, 3, 3 },
		{ 6, 3, 3, 2 }, { 5, 5, 7, 5 } };
int available[4] = { 10, 5, 7, 8 };
int threads = 5;
int resources = 4;

int main(int argc, char *argv[]) {

	//we didnt get the available resources
	if (argc == 1) {
		printf("Not enough arguments, closing");
		return 1;
	}

	//int request[4] = { 1, 0, 0, 1 };

	//request_granted(0, request);

	//int *finish = safe_sequence();

	//for (int i = 0; i < threads; i++) {
	//printf("%d ", finish[i]);

	//}

	//printf("\n");

	begin();

	return 0;
}

//Function to start the program
void begin() {
	bool running = true;

	//keep running until exit command is typed
	while (running) {
		//collect input
		printf("\nEnter Command: ");
		char *input = get_input();

		//get length of input
		char *c_input = strdup(input);
		int length = get_arg_count(c_input);

		//split the input into an array
		char **values = split_word(input, length);

		//Process which command should be run

		if (strcmp(values[0], "status") == 0) {
			print_status();
		}

		else if (strcmp(values[0], "exit") == 0) {
			printf("Exiting Program...\n");
			running = false;
		}

		else if (strcmp(values[0], "run") == 0) {

			//get safe sequence
			int *finish = safe_sequence();
			if (is_safe(finish)) {
				//pass safe sequence to thread handler
				handle_threads(finish);
			} else {
				printf(
						"Tried to run but you are in unsafe state. You should never be in unsafe state, Something went wrong\n");
			}

			//thread handler will execute 5 thread and pass the safe sequence position number to thread id

		} else if (strcmp(values[0], "rq") == 0) {
			if (length == resources + 2) {	//correct number of arguments for rq
			//get process # and its request
				int thread = atoi(values[1]);
				int *request = malloc(sizeof(int) * resources);
				for (int i = 0; i < resources; i++) {
					request[i] = atoi(values[i + 2]);
				}

				//attempt to grant request
				bool granted = request_granted(thread, request);
				if (granted == true) {
					printf("State is safe, and request is satisfied\n");
				} else {
					printf(
							"Request denied. Request will put system in unsafe state.");
				}

			} else {
				printf("Incorrect number of arguments for Request command");
			}

		} else if (strcmp(values[0], "rl") == 0) {
			if (length == resources + 2) {	//correct number of arguments for rl
			//get process # and its request
				int thread = atoi(values[1]);
				int *request = malloc(sizeof(int) * resources);
				for (int i = 0; i < resources; i++) {
					request[i] = atoi(values[i + 2]);
				}

				//attempt to grant request
				bool granted = release_granted(thread, request);
				if (granted == true) {
					printf("State is safe, and request is satisfied\n");
				} else {
					printf(
							"Request denied. Process does not have that many resources to release.");
				}

			} else {
				printf("Incorrect number of arguments for Release command");
			}

		} else {
			printf("Invalid input, use one of RQ, RL, Status, Run, Exit");
		}
	}
}

void handle_threads(int *finish) {
	printf("Safe sequence is: ");

	for (int i = 0; i < threads; i++) {
		printf("%d ", finish[i]);
	}
	printf("\n");

	//create the threads and run them
	for (int i = 0; i < threads; i++) {
		pthread_t tid;
		int id = finish[i];
		printf("Customer/Thread: %d\n", id);
		printf("Allocated Resources: ");
		for (int j = 0; j < resources; j++) {
			printf("%d ", allocated[id][j]);
		}
		printf("\n");
		printf("Needed: ");
		for (int j = 0; j < resources; j++) {
			printf("%d ", needs[id][j]);
		}
		printf("\n");
		printf("Available: ");
		for (int j = 0; j < resources; j++) {
			printf("%d ", available[j]);
		}
		printf("\n");
		pthread_create(&tid, NULL, runner, (void*) &id);
		pthread_join(tid, NULL);
	}
}

void* runner(void *thread_id) {
	int tid = *((int*) thread_id);

	printf("Thread has started\n");
	//add allocated to available
	add_rows(available, allocated[tid]);

	//set need back to max need
	add_rows(needs[tid], allocated[tid]);

	//set allocated to 0
	subtract_rows(allocated[tid], allocated[tid]);

	printf("Thread has finished\n");

	printf("Thread has releasing resources\n");

	printf("New Available: ");

	for (int i = 0; i < resources; i++) {
		printf("%d ", available[i]);
	}

	printf("\n");
	return NULL;
}

bool release_granted(int p, int *request) {
	//thread p request the request array of size resources
	//we will either grant request or wont

	if (isless(request, max_needs[p]) == false) { //request is > max this process can hold
		return false;
	}

	if (isless(request, allocated[p]) == false) { //request is > allocated for this process
		return false;
	}

	//subtract the resources used from allocated
	subtract_rows(allocated[p], request);

	//add the resources released back into available
	add_rows(available, request);

	//subtract needs now that we allocated some resources
	for (int j = 0; j < resources; j++) {
		needs[p][j] = max_needs[p][j] - allocated[p][j];
	}

	return true;

}

bool request_granted(int p, int *request) {
	//thread p request the request array of size resources
	//we will either grant request or wont

	//if granted = false and we ran safe seq, we assign allocated prve and needs prev back to our original arrays
	int *allocated_prev = create_copy(allocated[p]);
	int *needs_prev = create_copy(needs[p]);
	int *avail_prev = create_copy(available);

	if (isless(request, needs[p]) == false) { //request is > needs for this process
		return false;
	}

	if (isless(request, available) == false) { //request is greater than what we currently have available
		return false;
	}

	//subtract the resources used from availble
	subtract_rows(available, request);

	//add the resources request to allocated
	add_rows(allocated[p], request);

	//subtract needs now that we allocated some resources
	subtract_rows(needs[p], request);

	//retreive safe seq
	int *finish = safe_sequence();

	if (is_safe(finish) == false) { //the sequence wasnt safe revert back to prev needs and allocated array
		for (int j = 0; j < resources; j++) {
			allocated[p][j] = allocated_prev[j];
			needs[p][j] = needs_prev[j];
			available[j] = avail_prev[j];
		}
		return false;
	}

	return true;

}

bool is_safe(int *finish) {
	for (int i = 0; i < threads; i++) {
		if (finish[i] == -1) {
			return false;
		}
	}

	return true;
}

//checks if we are currently in a safe state and returns the safe sequence
int* safe_sequence() {

	//create copies of allocated and need (will be used in rq command)
	//int **allocated_copy = create_copy(allocated);
	//int **needs_copy = create_copy(needs);

	//finish array to hold safe sequence
	int *finish = malloc(sizeof(int) * threads);
	int *seq = malloc(sizeof(int) * threads);

	for (int i = 0; i < threads; i++) {
		finish[i] = -1; //singnifies thread i not in safe seq yet
		seq[i] = -1;
	}

	//copy currently available to work done
	int *work = create_copy(available);

	//bankers algo
	int finish_order = 0; //keep track of safe seq order
	for (int k = 0; k < threads; k++) { //run this loop to keep checking if we can now add a thread after a possible increase in work
		for (int i = 0; i < threads; i++) {
			for (int j = 0; j < resources; j++) {
				if (finish[i] == -1 && isless(needs[i], work)) { //this process has not been added to safe seq and its need <= work
					finish[i] = finish_order;
					seq[finish_order] = i;
					finish_order++;
					work = add_rows(work, allocated[i]);
				}
			}
		}
	}

	//return safe sequence
	return seq;
}

int* create_copy(int *arr) {
	int *copy = malloc(sizeof(int) * resources);

	for (int i = 0; i < resources; i++) {
		copy[i] = arr[i];
	}

	return copy;
}

//split the input into a character array
char** split_word(char *input, int length) {
	//make copy of input cause tokenizer will ruin input string
	char *c_input = strdup(input);
	char *token;
	char **vals = malloc(sizeof(char*) * length);

	token = strtok(c_input, " ");
	int i = 0;

	//copying each arg into vals array
	while (token != NULL) {
		vals[i] = malloc(sizeof(token) * sizeof(char));
		strcpy(vals[i], token);
		i++;
		token = strtok(NULL, " ");
	}

	return vals;

}

//get the number of args in the input
int get_arg_count(char *input) {
	int count = 0;
	char *token;
	token = strtok(input, " ");
	while (token != NULL) {
		count++;
		token = strtok(NULL, " ");
	}

	return count;

}

//helper to check if 1 row is less than the other
bool isless(int *need, int *work) {
	bool less = true;
	for (int i = 0; i < resources; i++) {
		if (need[i] > work[i]) {
			less = false;
		}
	}

	return less;

}

//helper to subtract 2 rows together and return the result
int* subtract_rows(int *work, int *need) {
	for (int i = 0; i < resources; i++) {
		work[i] = work[i] - need[i];
	}

	return work;
}

//helper to add 2 rows together and return the result
int* add_rows(int *work, int *allocation) {
	for (int i = 0; i < resources; i++) {
		work[i] = work[i] + allocation[i];
	}

	return work;
}

//function to parse input from user
char* get_input() {
	//Set the max buffer space
	int buffer = 80;
	//Allocate space for the input
	char *sentence = malloc(buffer);
	//Assign a pointer to our allocated memory
	char *line = sentence;
	//set length initially to the size of our buffer
	int length = buffer;
	//keep a bool to know when the user done typing
	bool typing = true;

	//While the user is typing run this code
	while (typing) {
		//collect each character the user types
		int c = tolower(fgetc(stdin));
		//if we have reached an end character then collect input
		if (c == EOF || c == '\n' || c == '\r')
			typing = false;

		//if we've rached end of buffer then typing == false again
		else if (--length == 0) {
			typing = false;
		}

		//collect the character and store it in sentence and move sentence up
		else if ((*sentence++ = c) == '\n')
			typing = false;
	}

	//make sure to append a null char to end of our sentence
	*sentence = '\0';
	sentence[strcspn(sentence, "\n")] = 0;

	return line;
}

//Prints the status when status command is asked
void print_status() {

	printf("Available resources: ");
	for (int i = 0; i < resources; i++) {
		printf("%d ", available[i]);
	}

	printf("\n");

	printf("Maximum Resources:\n");
	for (int i = 0; i < threads; i++) {
		print_thread(max_needs, i);
	}
	printf("Allocated Resources:\n");
	for (int i = 0; i < threads; i++) {
		print_thread(allocated, i);
	}
	printf("Need Resources:\n");
	for (int i = 0; i < threads; i++) {
		print_thread(needs, i);
	}

}

//helper to print a row from any 2d matrix
void print_thread(int arr[][4], int i) {
	for (int j = 0; j < resources; j++) {
		printf("%d ", arr[i][j]);
	}
	printf("\n");
}
