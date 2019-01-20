#include "queue.h"
#include <stdlib.h>

/*******************************************************************
 * Structures
 ******************************************************************/

typedef struct node{
	void* data;
	struct node* next;
}node_t;

struct queue {
    node_t* first;
	node_t* last;
};

/*******************************************************************
 *Auxiliary Functions
 ******************************************************************/

/*Creates a new node with the specified data.*/
node_t* node_create(void* data){
	node_t* node = malloc(sizeof(node_t));
	
	if(!node){
		return NULL;
	}
	
	node->data = data;
	node->next = NULL;
	return node;
}

/*******************************************************************
 *Primitives 
 ******************************************************************/

queue_t* queue_create(){
	queue_t* queue = malloc(sizeof(queue_t));
	
	if(!queue){
		return NULL;
	}
	
	queue->first = NULL;
	queue->last = NULL;
	return queue;
}

void queue_destroy(queue_t *queue, void destroy_data(void*)){	
	void* data = NULL;
	
	while(!queue_is_empty(queue)){		
		data = queue_dequeue(queue);
		
		if(destroy_data){
			destroy_data(data);
		}
	}		
	
	free(queue);
}

bool queue_is_empty(const queue_t *queue){
	return !queue->first;
}

bool queue_enqueue(queue_t *queue, void* value){	
	node_t* new_node = node_create(value);
	
	if(!new_node){
		return false;	
	}
	
	if (queue->last!=queue->first){
		queue->last->next = new_node;
	}
	
	else if(!queue_is_empty(queue)){
		queue->first->next = new_node;
	}
	
	else {
		queue->first = new_node;
	}
	
	queue->last = new_node;
	return true;
}

void* queue_front(const queue_t *queue){
	if(queue_is_empty(queue)){
		return NULL;
	}
	
	return queue->first->data;
}

void* queue_dequeue(queue_t *queue){
	if(queue_is_empty(queue)){
		return NULL;
	}

	void* data = queue->first->data;
	node_t* aux_node = queue->first;
	queue->first = queue->first->next;
	free(aux_node);
	
	if(queue_is_empty(queue)){
		queue->last = NULL;
	}
	
	return data;
}