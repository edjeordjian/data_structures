#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>

/*Normal queue, with no particular priority (FIFO).*/

/*******************************************************************
 * Structures
 ******************************************************************/

struct queue;
typedef struct queue queue_t;

/*******************************************************************
 * Primitives 
 ******************************************************************/

/*Creates an empty queue.*/
queue_t* queue_create(void);

/*Destroys the queue. If needed, you may have to specify 
a data destroying function for the data stored in the queue (e.g. if 
dynamic memory has been allocated for the data stored in the queue). 
Else, use NULL as your destroying function*/
void queue_destroy(queue_t *queue, void destroy_data(void*));

/*Returns true if the queue is empty.*/ 
bool queue_is_empty(const queue_t *queue);

/*Adds a new element to the queue.
Returns false in the case of an error.*/
bool queue_enqueue(queue_t *queue, void* value);

/*Returns the first element of the queue, NULL if empty.*/
void* queue_front(const queue_t *queue);

/*Drops and returns the first element of the queue. 
Returns NULL is the queue is empty.*/
void* queue_dequeue(queue_t *queue);

#endif // QUEUE_H
