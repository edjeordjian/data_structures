#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************
 * Structures				
 ******************************************************************/

typedef struct node{
	void* data;
	struct node* next;
}node_t;

struct list{
	node_t* first;
	node_t* last;
	size_t size; 
};

struct list_iter{
	list_t* list; 
	node_t* current; 	
	node_t* previous; 
};

/******************************************************************
 * Auxiliary Functions
 ******************************************************************/

/*Creates a new node with the given data.*/
node_t* node_create(void* data){
	node_t* node = malloc(sizeof(node_t));
	
	if(!node){
		return NULL;
	}
	
	node->data = data;
	node->next = NULL;
	return node;
}

/******************************************************************
 * Primitives
 ******************************************************************/

/*Inner iterator*/
void list_iterate(list_t *list, bool visit(void *data, void *extra), void *extra){
	node_t* current = list->first;
	bool iterarate = true;
	
	while(current && iterarate){
		iterarate = visit(current->data,extra);
		current = current->next;
	}
}

/*Outer iterator*/
list_iter_t *list_iter_create(list_t *list){	
	list_iter_t* iter = malloc(sizeof(list_iter_t));
	
	if(!iter){
		return NULL;
	}
	
	iter->list = list;
	iter->current = list->first;
	iter->previous = NULL;
	return iter;
}

void *list_iter_get_current(const list_iter_t *iter){
	if(!iter->current){
		return NULL;
	}
	
	return iter->current->data;
}

bool list_iter_at_end(const list_iter_t *iter){
	return iter->current==NULL;	
}

bool list_iter_continue(list_iter_t *iter){
	if(list_iter_at_end(iter)){
		return false;
	}
	
	iter->previous = iter->current;
	iter->current = iter->current->next;
	return true;
}

void list_iter_destroy(list_iter_t *iter){
	free(iter);
}

bool list_iter_insert(list_iter_t *iter, void *data){
	node_t* new = node_create(data);
	node_t* aux = iter->current;
	
	if(!new){
		return false;
	}
	
	bool is_at_end = list_iter_at_end(iter);
	iter->current = new;
	
	if(iter->list->size == 0){	
		iter->list->first = iter->list->last = iter->current; 
	}
	
	else if(aux == iter->list->first){
		iter->current->next = iter->list->first;
		iter->list->first = iter->current;
	}

	else if(aux == iter->list->last || is_at_end){
		iter->list->last->next = new;
		iter->list->last = iter->current;
	}
	
	else{
		new->next = aux;
		iter->previous->next = new;
	}
	
	iter->list->size+=1;
	return true;
}

void *list_iter_remove(list_iter_t *iter){

	if(iter->list->size == 0 || list_iter_at_end(iter)){
		return NULL;
	}
	
	node_t* aux = iter->current;
	void* data = iter->current->data;
	
	if(iter->current == iter->list->first){
		iter->list->first = iter->current->next; 
		iter->current = iter->list->first;
		iter->previous = NULL;
	}
	
	else if(iter->list->last == iter->current){
		iter->list->last = iter->previous;
		iter->current = NULL;
		iter->previous->next = iter->current;
	}
	
	else{
		iter->current = iter->current->next;
		iter->previous->next = iter->current;
	}
	
	iter->list->size-=1;
	free(aux);
	return data;
}

/*List*/

list_t *list_create(void){
	list_t* list = malloc(sizeof(list_t));

	if(!list){
		return NULL;
	}
	
	list->first = list->last = NULL;
	list->size = 0;
	return list;
}

bool list_is_empty(const list_t *list){
	return list->size==0;
}

bool list_add_first(list_t *list, void *data){
	node_t* new = node_create(data);
	
	if(!new){
		return false;	
	}
	
	if(list_is_empty(list)){
		list->last = new;
	}
	
	else{	
		new->next = list->first;
	}

	list->first = new;
	list->size +=1;
	return true;
}

bool list_add_last(list_t *list, void *data){
	node_t* new = node_create(data);
	
	if(!new){
		return false;	
	}
	
	if(list_is_empty(list)){ 
		list->first = new;
	}

	else{
		list->last->next = new;
	}
	
	list->last = new;
	list->size +=1;
	return true;
}

void *list_remove_first(list_t *list){
	if(list_is_empty(list)){
		return NULL;
	}
	
	void* data = list->first->data;
	node_t* aux = list->first;
	list->first = list->first->next;
	list->size-=1;
	
	if(list_is_empty(list)){
		list->last = NULL;
	}
	
	free(aux);
	return data;
}

void *list_get_first(const list_t *list){
	if(!list->first){
		return NULL;
	}
	
	return list->first->data;
}

void *list_get_last(const list_t* list){
	if(!list->last){
		return NULL;
	}
	
	return list->last->data;
}

size_t list_get_size(const list_t *list){
	return list->size;
}

void list_destroy(list_t *list, void destroy_data(void *)){
	void* data = NULL;
	
	while(!list_is_empty(list)){
		data = list_remove_first(list);
		
		if(destroy_data){
			destroy_data(data);
		}
	}
	
	free(list);
}