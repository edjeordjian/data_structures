#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

#define INITIAL_CAPACITY 1024
#define INCREASEMENT_FACTOR 2
#define REDUCTION_FACTOR 4

/*******************************************************************
 * Structures				
 ******************************************************************/

struct heap{
	void** data;
	size_t items;
	size_t size;
	cmp_func_t cmp;
}; 

/*******************************************************************
 * Auxiliary Functions		
 ******************************************************************/

/*Resizes the heap with the specified size.
Returns false in the case of an error. */
bool heap_resize(heap_t* heap, size_t new_size){	
    if(new_size == 0) {
    	return false;
    }
	
	void** new_data = realloc(heap->data, sizeof(void*) * new_size);
	
	if(new_data == NULL) {
		return false;
	}
	
	heap->data = new_data;
	heap->size = new_size;
	return true;
}

/*Swaps the elements in the specified positions in the given array.*/
void swap(void** data, size_t a, size_t b){
	void* aux = data[a];
	data[a] = data[b];
	data[b] = aux;
}

/*From a given position, searches and returns the element with highest
priority in the array (according to the given comparison function)*/
size_t get_pos_son_max(void** data, size_t pos, size_t items, cmp_func_t cmp){
	size_t left_son_pos = (pos * 2) + 1;
	size_t right_son_pos = (pos * 2) + 2;
	
	if(left_son_pos >= items) {
		return right_son_pos; 
	}
	
	if(right_son_pos >= items) {
		return left_son_pos; 
	}

	if(cmp(data[left_son_pos], data[right_son_pos]) > 0) {	
		return left_son_pos;
	}
	
	return right_son_pos;
}

/*Moves down the element in the given position.*/
void downheap(void** data, size_t items, size_t pos, cmp_func_t cmp){	
	size_t left_son_pos = (pos * 2) + 1;
	size_t right_son_pos = (pos * 2) + 2;
	
	if(left_son_pos >= items && right_son_pos >= items) {
		return;
	}

	size_t pos_h_max = get_pos_son_max(data, pos, items, cmp);
	
	if(cmp(data[pos], data[pos_h_max]) < 0){
		swap(data, pos, pos_h_max);
		downheap(data, items, pos_h_max, cmp);	
	}
}

/*Moves up the element in the given position*/
void upheap(void** data, size_t pos, cmp_func_t cmp){
	if(pos > 0 && cmp(data[pos], data[(pos - 1) / 2]) > 0){
		swap(data, pos, (pos - 1) / 2);
		pos = (pos - 1)/2;
		upheap(data, pos, cmp);
	}
}
	
/*Turns the given array (in-place) into a max-heap.*/
void heapify(void** elements, size_t n, cmp_func_t cmp){
	for(size_t k = (n / 2); k > 0; k--){
		downheap(elements, n, k - 1, cmp);
	}
}

/*******************************************************************
 * Primitives			
 ******************************************************************/

heap_t *heap_create(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));

	if(!heap) {
		return NULL;
	}
	
	heap->size = INITIAL_CAPACITY;
	heap->items = 0;
	heap->cmp = cmp;
	heap->data = malloc(sizeof(void*) * (heap->size));

	if (!heap->data) {
		heap_destroy(heap, NULL);
		return NULL;
	}
	
	return heap;
}

heap_t *heap_create_arr(void *array[], size_t n, cmp_func_t cmp){
	heap_t* heap = heap_create(cmp);
	
	if(!heap) {
		return NULL;
	}

	if(n == 0) {
		return heap;
	}
	
	if(n >= heap->size){
		if(!heap_resize(heap, n * INCREASEMENT_FACTOR)){
			return NULL;
		}
	}
		
	for(int i = 0; i < n; i++){
		heap->data[i] = array[i];
		heap->items+=1;
	}
	
	heapify(heap->data, heap->items, heap->cmp);
	return heap;
}

void heap_destroy(heap_t *heap, void destroy_data(void *e)){
	size_t items = heap->items;

	if(destroy_data){
		for(int i = 0; i < items; i++){
			destroy_data(heap->data[i]);
		}
	}

	free(heap->data);
	free(heap);
}

size_t heap_size(const heap_t *heap){
	return heap->items;
}

bool heap_is_empty(const heap_t *heap){
	return heap->items == 0;
}

bool heap_push(heap_t *heap, void *elem){
	if(heap->items == heap->size){
		if(!heap_resize(heap, heap->size * INCREASEMENT_FACTOR)) {
			return false;
		}
	}
		
	heap->data[heap->items] = elem;
	upheap(heap->data, heap->items, heap->cmp);
	heap->items+=1;
	return true;
}

void *heap_get_max(const heap_t *heap){
	if(heap_is_empty(heap)) {
		return NULL;
	}

	return heap->data[0];
}

void *heap_pop(heap_t *heap){
	if(heap_is_empty(heap)) {
		return NULL;
	}
	
	void* value = heap->data[0];
	heap->data[0] = heap->data[heap->items - 1];
	downheap(heap->data, heap->items, 0, heap->cmp);
	heap->items -= 1;
	
	if(heap->items <= heap->size / REDUCTION_FACTOR && heap->size > REDUCTION_FACTOR){
		heap_resize(heap, (size_t)(heap->size / REDUCTION_FACTOR));
	}
	
	return value;
}

void heap_sort(void *elements[], size_t items, cmp_func_t cmp){
	heapify(elements, items, cmp);
	
	for(size_t i = items-1; i > 0; i--){
		swap(elements, 0, i);
		downheap(elements, i, 0, cmp);
	}
}