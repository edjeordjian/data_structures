#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stdlib.h>
#include <stdbool.h>

/*Dynamic array for storing numbers.*/

/*******************************************************************
 * Structures
 ******************************************************************/

typedef struct dyn_array {
    size_t size;
    int* data;
} dyn_array_t;

/*******************************************************************
 * Primitives
 ******************************************************************/

/*Creates a new void dynamic array of size 'arr_size'.*/
dyn_array_t* dyn_array_create(size_t arr_size);

/*Destroys the dynamic array.*/
void dyn_array_destroy(dyn_array_t* dyn_array);

/*Resizes the dynamic array. Returns false if resize is not possible.*/
bool dyn_array_resize(dyn_array_t* dyn_array, size_t new_size);

/*Stores in 'value' what is stored in the position 'pos' of the dynamic array.
Returns false if 'pos' is an invalid position.*/
bool dyn_array_get(dyn_array_t* dyn_array, size_t pos, int* value);

/*Stores 'value' in the position 'pos' of the dynamic array.
Returns false if 'pos' is a invalid position.*/
bool dyn_array_store(dyn_array_t* dyn_array, size_t pos, int value);

/*Returns the size of the dynamic array.*/
size_t dyn_array_get_size(dyn_array_t* dyn_array);

#endif // DYN_ARRAY_H
