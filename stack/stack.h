#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

/******************************************************************
 * Structures
 ******************************************************************/

struct stack;
typedef struct stack stack_t;

/******************************************************************
 * Primitives
 ******************************************************************/

/*Creates a new empty stack.*/
stack_t* stack_create(void);

/*Destroys the stack.*/
void stack_destroy(stack_t *stack);

/*Returns true if the stack is empty.*/
bool stack_is_empty(const stack_t *stack);

/*Adds 'value' to the stack.
Returns false in case of an error.*/
bool stack_push(stack_t *stack, void* value);

/*Returns stack's top element.*/
void* stack_top(const stack_t *stack);

/*Removes and returns stack's top element.*/
void* stack_pop(stack_t *stack);

#endif // STACK_H