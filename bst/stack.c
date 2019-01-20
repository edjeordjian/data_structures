#include "stack.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 1024
#define EXTENSION_FACTOR 2
#define REDUCTION_FACTOR 4

/******************************************************************
 * Structures
 ******************************************************************/

struct stack {
    void** data;
    size_t items; 
    size_t size;
};

/******************************************************************
 * Auxiliary Functions
 ******************************************************************/

/* Resises the stack to the specified size. 
Returns false in case of an error. */
bool stack_resize(stack_t* stack, size_t new_size) {
    if(new_size == 0) {
        return false;
    }
	
	void** new_data = realloc(stack->data, sizeof(void*) * new_size);
	
	if(new_data == NULL){
		return false;
	}
	
	stack->data = new_data;
	stack->size = new_size;
	return true;
}

/******************************************************************
 * Primitives
 ******************************************************************/

stack_t* stack_create(void){
	stack_t* stack = malloc(sizeof(stack_t));
	
	if(!stack){
		return NULL;
	}
	
	stack->size = INITIAL_CAPACITY;
	stack->items = 0;
	stack->data = malloc(sizeof(void*) * (stack->size));
	
	if (!stack->data) {
		stack_destroy(stack);
		return NULL;
	}
	
	return stack;
}

void stack_destroy(stack_t *stack){
	if(stack){
		free(stack->data);
	}

	free(stack);
}

bool stack_is_empty(const stack_t *stack){
	return (!stack || stack->items == 0);
}

void* stack_top(const stack_t *stack){
	if(stack_is_empty(stack)){
		return NULL;
	}
	
	return stack->data[stack->items - 1];
}

bool stack_push(stack_t *stack, void* value){	
	if(!stack){
		return false;
	}
	
	if(stack->items == stack->size){
		stack->size *= EXTENSION_FACTOR;

		if(!stack_resize(stack, stack->size)){
			return false;
		}
	}
	 
	stack->data[stack->items] = value;
	stack->items += 1;
	return true;
}

void* stack_pop(stack_t *stack){
	if(stack_is_empty(stack)){
		return NULL;
	}
	
	void* top = stack_top(stack);
	stack->items -= 1;
	size_t quarter = stack->size / REDUCTION_FACTOR;
	size_t half = stack->size / EXTENSION_FACTOR;
	
	if((stack->items == quarter) && (half > 1)){
		stack_resize(stack, half);
	}
	
	return top;
}