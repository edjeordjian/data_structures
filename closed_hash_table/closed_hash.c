#include "closed_hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CAPACITY 97
#define PERCENT 100
#define MAX_LOAD 70
#define MIN_LOAD 15

/*******************************************************************
 *Structures				
 ******************************************************************/

typedef enum {EMPTY, REMOVED, OCCUPIED} state_t;

typedef struct hash_node{
	char* key;
	void* value;
	state_t state;
}hash_node_t;

struct hash{
	hash_node_t* array;
	size_t items;
	size_t size;
	hash_destroy_data_t destroy;
};

struct hash_iter{
	const hash_t* hash;
	size_t position;
};

/*******************************************************************
 *Auxiliary Functions				
 ******************************************************************/

/*Bernstein's hashing function. 
Based on the generic multiplicative function from: 
https://www.strchr.com/hash_functions */
size_t hashing(const char *str, size_t capacity){
    int nhash = 5381;
    int c;

    while ((c = *str++)){
        nhash = ((nhash << 5) + nhash) + c; /* hash * 33 + c */
    }

    return (nhash % capacity);
}

/*Recreates the hash table, resizing it.*/
bool hash_resize(hash_t* hash){
	size_t new_capacity;
	
	if(MIN_LOAD >= ((float)hash->items / (float)hash->size) * PERCENT){
		new_capacity = (hash->size) / 2;
	}
	
	else{
		new_capacity = (hash->size) * 2;
	}
	
	size_t pos = 0;
	hash_node_t* new_array = malloc(sizeof(hash_node_t) * new_capacity);
	
	if (new_array == NULL){
		return false;
	}
	
	hash_node_t* aux = hash->array;
	hash->items = 0;
	hash->array = new_array;
	size_t old_capacity = hash->size;
	hash->size = new_capacity;
	
	for (size_t aux_pos = 0; aux_pos<(hash->size); aux_pos++){
		hash->array[aux_pos].state = EMPTY;
	}

	while(pos != old_capacity){
		if(aux[pos].state == OCCUPIED){
			hash_store(hash, aux[pos].key, aux[pos].value);
			free(aux[pos].key);
		}

		pos++;
	}

	free(aux);
	return true;
}

/*******************************************************************
 *Primitives				
 ******************************************************************/

hash_t *hash_create(hash_destroy_data_t destroy_data){
	hash_t * hash = malloc(sizeof(hash_t));
	
	if(hash == NULL){
		return NULL;
	}
	
	hash->array = malloc(sizeof(hash_node_t) * CAPACITY);
	
	if (hash->array == NULL){
		free(hash);
		return NULL;
	}
	
	hash->items = 0;
	hash->size = CAPACITY;
	
	for (int pos = 0; pos < (hash->size); pos++){
		hash->array[pos].state = EMPTY;
	}
	
	hash->destroy = destroy_data;
	return hash;	
}

void hash_destroy(hash_t *hash){
	size_t pos = 0;

	while(pos != hash->size ){
		if (hash->array[pos].state == OCCUPIED){
			if (hash->destroy != NULL){
				hash->destroy(hash->array[pos].value);
			}
			
			free(hash->array[pos].key);
		}
		
		pos++;
	}

	free(hash->array);
	free(hash);
}

size_t hash_size(const hash_t *hash){
	return hash->items;
}

bool hash_store(hash_t *hash, const char *key, void *data){
	if(((float)hash->items / (float)hash->size) * PERCENT >= MAX_LOAD){
		 bool redimension = hash_resize(hash);
		 
		 if (!redimension){
			 return false;
		 }
	 }

	size_t pos = hashing(key, hash->size);
	
	while(hash->array[pos].state != EMPTY){
		if(hash->array[pos].state != REMOVED){
			if (strcmp(hash->array[pos].key, key) == 0){
				if (hash->destroy){
					hash->destroy(hash->array[pos].value);
				}

				hash->items--;
				free(hash->array[pos].key);
				break;
			}
		}

		pos++;
		
		if(pos == hash->size){
			pos = 0;
		}
	}
	
	hash->array[pos].key = malloc(strlen(key) + 1);
	strcpy(hash->array[pos].key, key);
	hash->array[pos].value = data;
	hash->array[pos].state = OCCUPIED;
	hash->items++;
	return true;
}

void *hash_remove(hash_t *hash, const char *key){
	if((float)hash->size != CAPACITY && MIN_LOAD >= (((float)hash->items / (float)hash->size) * PERCENT)){
		 bool redimension = hash_resize(hash);
		 
		 if (!redimension){
			 return false;
		 }
	 }

	size_t pos = hashing(key, hash->size);
	
	while(hash->array[pos].state != EMPTY){
		if(hash->array[pos].state == OCCUPIED && strcmp(hash->array[pos].key, key) == 0){
			hash->array[pos].state = REMOVED;
			hash->items--;
			free(hash->array[pos].key);
			return hash->array[pos].value;
		}

		pos++;
		
		if(pos == hash->size){
			pos = 0;
		}
	}

	return NULL;	
}

void* hash_get(const hash_t *hash, const char *key){
	size_t pos = hashing(key, hash->size);

	while(hash->array[pos].state != EMPTY){
		if(hash->array[pos].state == OCCUPIED && strcmp(hash->array[pos].key, key) == 0){
			return hash->array[pos].value;
		}
		
		pos++;
		
		if(pos == hash->size){
			pos = 0;
		}
	}

	return NULL;
}

bool hash_is_in(const hash_t *hash, const char *key){
	
	if (hash_get(hash, key) != NULL || strcmp(key, "") == 0){
		return true;
	}

	return false;
}

/*Iterator*/

hash_iter_t *hash_iter_create(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	
	if(iter == NULL){
		return NULL;
	}
	
	iter->hash = hash;
	iter->position = -1;
	hash_iter_next(iter);
	return iter;
}

bool hash_iter_at_end(const hash_iter_t *iter){
	return (iter->position == iter->hash->size);
}

bool hash_iter_next(hash_iter_t *iter){
	if(hash_iter_at_end(iter)){
		return false;
	}
	
	iter->position++;
	
	while(iter->position != iter->hash->size){
		if(iter->hash->array[iter->position].state == OCCUPIED){
			break;
		}
		
		iter->position++;
	}
	
	return true;
}

const char *hash_iter_get_current(const hash_iter_t *iter){
	if(hash_iter_at_end(iter)){
		return NULL;
	}
	
	return iter->hash->array[iter->position].key;
}

void hash_iter_destroy(hash_iter_t* iter){
	free(iter);
}