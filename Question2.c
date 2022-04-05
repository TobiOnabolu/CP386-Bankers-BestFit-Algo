#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//doubly linked list struct
typedef struct mBlock {
    size_t low, high;
    char *name;
    struct mBlock *prev, *next;
} mBlock;

mBlock *make_memory_block(size_t low, size_t high, const char *name, mBlock *prev, mBlock *next);
int request_memory(const char *name, size_t size, char algorithm);
int release_memory(const char *name);
void print_status();
void request_aux();
void release_aux();
int init(int argc, char **argv);
int main(int argc, char **argv);

//global constants
#define OPERATOR_LENGTH 8
#define PROCESS 128


//global variables
size_t memory_size = 0;
mBlock *memory;


mBlock *make_memory_block(size_t low, size_t high, const char *name, mBlock *prev, mBlock *next) {
    mBlock *mems = malloc(sizeof(mBlock));
    if(mems == NULL) {
        //printf("allocation failed.\n");
        exit(-1);
    }
    mems->low = low, mems->high = high;
    if(strlen(name) != 0) { // allocate the mem!
    	mems->name = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(mems->name, name);
    } else {
    	mems->name = NULL;
    }

    mems->prev = prev, mems->next = next; //setting up our linked list YAY
    if(prev) {
        prev->next = mems;
    }
    if(next) {
        next->prev = mems;
    }
    return mems;
}

int request_memory(const char *name, size_t size, char algorithm) {
    mBlock *hole = NULL;
    if (algorithm == 'B'){ //best fit!!!!!!!!!! hehe
    	mBlock *where = memory;
        size_t min_size = -1;
        while(where) {
        	size_t hole_size = (where-> high - where->low + 1);
        	if(where->name == NULL && size <= hole_size && hole_size < min_size) {
        		min_size = hole_size;
        		hole = where;
        	}
        	where = where->next;
        }
    } else {
    	//printf("Select B\n");
    	return -1;
        }
    if(!hole || hole->name != NULL) {
        printf("No available memory to allocate.\n");
        return -2;
    }
    hole->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(hole->name, name);
    if(hole->high - hole->low + 1 == size) {   // this is for the chance that hole size is equal to size needed
        return 0;
    }
    hole->next = make_memory_block(hole->low + size, hole->high, "", hole, hole->next);
    hole->high = hole->low + size - 1;
    return 0;
}

int release_memory(const char *name) { //release for certain processes
    mBlock *where = memory;
    int flag = 1;
    while(where) {
        if(where->name && strcmp(where->name, name) == 0) {
            free(where->name);
            where->name = NULL;    // mark it unused
            flag = 0;
        }

        if(where->name == NULL && where->prev && where->prev->name == NULL) { //merge if possible to keep things simple
            mBlock *temp = where->prev;
            where->prev = temp->prev;
            if(temp->prev) {
                temp->prev->next = where;
            }
            where->low = temp->low;
            free(temp);
        }

        if(where->prev == NULL) { //update first block
        	memory = where;
        }
        where = where->next;
    }
    return flag;
}

void request_aux() {
    char name[PROCESS], strategy;
    size_t size;
    scanf("%s %zu %c", name, &size, &strategy);

    if (request_memory(name, size, strategy)) {
    	printf("Fail to allocate\n");
    } else {
    	printf("Successfully allocated %zu to process %s\n", size, name);
    }
}

void release_aux() {
    char name[PROCESS];
    scanf("%s", name);

    if(release_memory(name)) {
    	printf("Fail to release\n");
    } else {
    	printf("Releasing memory for process %s\n", name);
    }
}

void print_status() {
    mBlock *where = memory;
    while(where) {
        //printf("[%06zu - %06zu] ", where->low, where->high);
        if(where->name) {
        	printf("Address [%06zu:%06zu] Process %s\n", where->low, where->high, where->name);
        }
        where = where->next;
    }
    printf("\n");
    mBlock *there = memory;
    while(there) {
    	if(!there->name) {
    		printf("Address [%06zu:%06zu] len = %06zu\n", there->low, there->high, (there->high - there->low));
    	}
    	there = there->next;
    }
}

int init(int argc, char **argv) {
    if(argc != 2) {
        printf("Please put args in correctly\n");
        return -1;
    }
    sscanf(argv[1], "%zu", &memory_size);
    memory = make_memory_block(0, memory_size - 1, "", NULL, NULL);
    printf("Allocated %zu bytes of memory\n", memory_size);
    return 0;
}

int main(int argc, char **argv) {
    if(init(argc, argv) != 0) {
        return 0;
    }
    char op[OPERATOR_LENGTH];
    bool running = true;
    while(running) {
        printf("allocator> ");
        scanf("%s", op);
        if(strcmp(op, "RQ") == 0) {
            request_aux();
        } else if(strcmp(op, "RL") == 0) {
            release_aux();
        } else if(strcmp(op, "Status") == 0) {
            print_status();
        } else if (strcmp(op, "Exit") == 0) {
			printf("Exiting Program...\n");
			running = false;
		} else {
            printf("Unrecognized operation.\n");
        }
    }
    return 0;
}
