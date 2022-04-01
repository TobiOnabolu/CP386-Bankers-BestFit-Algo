#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

void begin();
void print_status();
int* add_rows(int *work, int *allocation);
bool isless(int *need, int *work);
int* subtract_rows(int *work, int *allocation);
void print_thread(int arr[][4], int i);
char* get_input();
char** split_word(char *input, int length);
int get_arg_count(char *input);

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
			printf("Executing run command");

			//get safe sequence

			//pass safe sequence to thread handler

			//thread handler will execute 5 thread and pass the safe sequence position number to thread id

		} else if (strcmp(values[0], "rq") == 0) {
			if (length == resources + 2) {	//correct number of arguments for rq
				printf("Executing request command");
			} else {
				printf("Incorrect number of arguments for Request command");
			}

		} else if (strcmp(values[0], "rl") == 0) {
			if (length == resources + 2) {	//correct number of arguments for rl
				printf("Executing release command");
			} else {
				printf("Incorrect number of arguments for Release command");
			}

		} else {
			printf("Invalid input, use one of RQ, RL, Status, Run, Exit");
		}
	}
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
bool isless(int *work, int *need) {
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
