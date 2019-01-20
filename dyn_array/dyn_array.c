#include "dyn_array.h"

void dyn_array_destroy(dyn_array_t* dyn_array){
	free(dyn_array->data);
    free(dyn_array);
}

bool dyn_array_get(dyn_array_t* dyn_array, size_t pos, int* value){	
	if(pos >= dyn_array->size){
		return false;
    }
	
	*value = dyn_array->data[pos];
	return true;
}

bool dyn_array_store(dyn_array_t* dyn_array, size_t pos, int value){
	if(pos >= dyn_array->size){
		return false;
    }

	dyn_array->data[pos]=value;
	
	if(dyn_array->data[pos] != value){
		return false;
    }
	
	return true;
}

size_t dyn_array_get_size(dyn_array_t* dyn_array){
	return dyn_array->size;
}

dyn_array_t* dyn_array_create(size_t arr_size) {
    dyn_array_t* dyn_array = malloc(sizeof(dyn_array_t));

    if (dyn_array == NULL) {
        return NULL;
    }
    
    dyn_array->data = malloc(arr_size * sizeof(int));

    if (arr_size > 0 && dyn_array->data == NULL) {
        free(dyn_array);
        return NULL;
    }
 
    dyn_array->size = arr_size;
    return dyn_array;
}

bool dyn_array_resize(dyn_array_t* dyn_array, size_t new_size) {
    int* new_data = realloc(dyn_array->data, new_size * sizeof(int));

    // Realloc might fail.
    if (new_size > 0 && new_data == NULL) {
        return false;
    }

    dyn_array->data = new_data;
    dyn_array->size = new_size;
    return true;
}