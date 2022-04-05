# Assignment 4
In this assingment we used Bankers algorithm to apply the concept of dealock avoidance and Best Fit algorithm to apply the concept of contigous memory allocation!

## Makefile

```
CC = gcc
CFLAGS = -Werror -Wall -g -std=gnu99
LDFLAGS = -lrt -lpthread
TARGET = Question1 Question2
OBJFILES = Question1.o Question2.o
all: $(TARGET)

Question1: Question1.c
	$(CC) $(CFLAGS) -o Question1 Question1.c $(LDFLAGS)
Question2: Question2.c
	$(CC) $(CFLAGS) -o Question2 Question2.c $(LDFLAGS)
	
runq1: Question1
	./Question1 10 5 7 8
runq2: Question2
	./Question2 1000000
	
clean:
	rm -f $(OBJFILES) $(TARGET) *~ 
```

## Contributors
- [Tobi Onabolu](https://github.com/TobiOnabolu)<br/>

- [Paige Broussard](https://github.com/Brou-0510)<br/>

## Preview 

<!-- Pictures -->
Screenshots of our question 1 output!!<br/>

![Screen Shot 2022-04-04 at 11 18 23 PM](https://user-images.githubusercontent.com/80122517/161672252-a14aae26-b328-40a1-87da-4eae21fb072e.png)<br/>
![Screen Shot 2022-04-04 at 11 18 51 PM](https://user-images.githubusercontent.com/80122517/161672261-8b0383c9-171e-47f0-897a-1146ba83c14e.png)<br/>


Screenshots of our question 2 output!!<br/>

![Screen Shot 2022-04-04 at 11 13 31 PM](https://user-images.githubusercontent.com/80122517/161671872-86cc9e93-1583-4cd4-b65c-745151cd31ef.png)<br/>


Question1.c
```c
void begin(); // Tobi and Paige
void print_status(); // Tobi
int* add_rows(int *work, int *allocation); // Tobi
bool isless(int *work, int *need); // Tobi
int* subtract_rows(int *work, int *allocation); // Tobi
void print_thread(int arr[][4], int i); // Tobi
char* get_input(); // Tobi. and Paige
char** split_word(char *input, int length); // Tobi
int get_arg_count(char *input); // Tobi
int* safe_sequence(); // Tobi and Paige
int* create_copy(int *arr); // Tobi
bool is_safe(int *finish); // Tobi
bool request_granted(int p, int *request); // Tobi and Paige
bool release_granted(int p, int *request); // Tobi and Paige
void handle_threads(int *finish); // Tobi
void* runner(void *thread_id); // Tobi
```

Question2.c
```c
mBlock *make_memory_block(size_t low, size_t high, const char *name, mBlock *prev, mBlock *next); // Paige 
int request_memory(const char *name, size_t size, char algorithm); // Paige and Tobi
int release_memory(const char *name); // Paige and Tobi
void request_aux(); // Paige 
void release_aux(); // Paige 
void print_status(); // Paige 
int init(int argc, char **argv); // Paige 
int main(int argc, char **argv); // Paige 
```

## Tests
Example Question 1: 
```
./Question1 10 7 5 8

RQ 0 1 0 0 1

RQ 2 2 2 2 2

RQ 4 1 1 1 1

Status

Run

Exit
```
Example Question 2: 
```
./Question2 1000000

RQ P0 200000 B

RQ P1 350000 B

RQ P2 300000 B

RL P0

Status

RL P2

Exit
```

## About The Developers 
- Tobi and Paige are both computer science majors at Wilfrid Laurier University!

## License
This project uses the **MIT License** which can be located [here](https://github.com/TobiOnabolu/CP386-Bankers-BestFit-Algo/blob/main/LICENSE)
