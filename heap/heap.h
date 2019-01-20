#ifndef HEAP_H
#define HEAP_H
#include <stdbool.h>  /* bool */
#include <stddef.h>	  /* size_t */

/*
Priority queue using a max-heap.

For a min-heap, use a comparision function which returns:
>0 if a < b
<0  if a > b
*/

/* Heap comparison function. Returns:
<0 if a < b
0 if a == b
>0  if a > b*/
typedef int (*cmp_func_t) (const void *a, const void *b);

typedef struct heap heap_t;

/* Allows to sort an array using heapsort (in-place O(nlog(n)) sort).*/
void heap_sort(void *elements[], size_t items, cmp_func_t cmp);

/*******************************************************************
 * Primitives			
 ******************************************************************/

/* Creates a new heap.*/
heap_t *heap_create(cmp_func_t cmp);

/*Alternative constructor for the heap, using an array to initialize it.*/
heap_t *heap_create_arr(void *array[], size_t n, cmp_func_t cmp);

/* Destroys the heap. If necessary (e.g. dynamic memory has been allocated for
the data stored in the heap), a data destroy function can be specified (not
NULL).*/
void heap_destroy(heap_t *heap, void destroy_data(void *e));

/* Returns the number of elements in the heap. */
size_t heap_size(const heap_t *heap);

/* Returns true if the heap is empty. */
bool heap_is_empty(const heap_t *heap);

/* Adds a new element to the heap (not NULL).
Returns false in case of an error. */
bool heap_push(heap_t *heap, void *elem);

/* Returns the element with highest priority (the first one in the heap, 
according to the comparison function) */
void *heap_get_max(const heap_t *heap);

/* Removes and returns the element with highest priority*/
void *heap_pop(heap_t *heap);

void pruebas_heap_alumno(void); ///

#endif // HEAP_H
