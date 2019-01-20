#include "open_hash.h"
#include "list.h"
#include <string.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 101 
#define INCREASEMENT_COEFICIENT 2 
#define REDUCTION_COEFICIENT 0.25 
#define INCRESEMENT_FACTOR 2 
#define REDUCTION_FACTOR 0.5

/*******************************************************************
 * Structures				
 ******************************************************************/

struct hash {
    list_t** table;
    size_t items;
    size_t size;
	hash_destroy_data_t destroy_data;
};

typedef struct hash_field{
	char* key;
	void* value;
}hash_field_t; 

struct hash_iter{
	const hash_t* hash;
	list_iter_t* list_iter;
	size_t current_pos;
	size_t iterated;
};

/******************************************************************
 * Auxiliary Functions
 ******************************************************************/

double get_coeficient(hash_t* hash){
	double size = (double)hash->size;
	double items = (double)hash->items;
	return items / size;
}

/* Hashing function based on the one from: 
"The C Programming Language: Second Edition", Brian Kenighan & Dennis Ritchie 
(6.6, page 118) - Prentice Hall, 1988.
"This is not the best possible hash function, but it is short and effective."
*/
size_t get_hash(const char* key, size_t size){
	unsigned hashval;
	
	for (hashval = 0; *key != '\0'; key++){
		hashval = (unsigned int) *key + 31 * hashval;
	}
	
	return hashval % size;
}

/*Creates a new hash with an specific size.*/
hash_t* hash_create_especifico(hash_destroy_data_t destroy_data, size_t size){
	
	hash_t* hash = malloc(sizeof(hash_t));
	
	if(!hash) {
		return NULL;
	}
	
	hash->size = size;
	hash->items = 0;
	hash->destroy_data = destroy_data;
	hash->table = malloc(sizeof(list_t*) * hash->size);
	
	if(!hash->table) {	
		free(hash); 
		return NULL;
	}
	
	for(int i=0; i<hash->size; i++){
		hash->table[i] = list_create(); 

		if(!hash->table[i]){
			hash_destroy(hash); 
			return NULL;
		}
	}
	
	return hash;
}

/*Destroys a hash field.*/
void hash_field_destroy(hash_field_t* field, const hash_t* hash){
	
	if(hash){ 
		if(hash->destroy_data && hash->items == hash->size){ //Removing for a replacement
			hash->destroy_data(field->value);
		}
	}

	free(field->key);
	free(field);
}

void wrapper_hash_field_destroy(void * field){
	hash_field_destroy(field, NULL);
}

/*Recreates the hash table with the specified size. 
Returns false in case of an error. */
bool hash_redimensionar(hash_t* hash, double tam_new){
	bool copy_ok = true;
	hash_t* hash_aux = hash_create_especifico(hash->destroy_data, (size_t) tam_new);
	
	if(!hash_aux) {
		return false;
	}
	
	size_t old_size = hash->size;
	hash_field_t* field = NULL;
	list_iter_t* iter;
	
	for(int i=0; i<old_size && copy_ok; i++) {	
		iter = list_iter_create(hash->table[i]);
		
		if(!iter) {
			copy_ok = false; 
			break;
		}
		
		while(!list_iter_at_end(iter)){
			field = list_iter_get_current(iter);
			
			if(!hash_store(hash_aux, field->key, field->value)){
				copy_ok = false;
				break;
			}
			
			list_iter_continue(iter);
		}

		list_iter_destroy(iter);
		list_destroy(hash->table[i],wrapper_hash_field_destroy);
	}
	
	free(hash->table);
	hash->table = hash_aux->table;
	hash->size = hash_aux->size;
	free(hash_aux);
	return copy_ok;
}

/*Returns the hash field associated with the given key*/
hash_field_t* hash_get_field(const hash_t *hash, const char *key){
	size_t pos = get_hash(key, hash->size);
	list_iter_t* iter = list_iter_create(hash->table[pos]);

	if(!iter) {
		return NULL;
	}
	
	hash_field_t* field = NULL;

	while(!list_iter_at_end(iter)){
		field = list_iter_get_current(iter);

		if(strcmp(field->key,key) == 0) {
			break;
		}
		list_iter_continue(iter);
	}
	
	list_iter_destroy(iter);
	
	if(!field) {
		return NULL;
	}
	
	return field;
}

/* Moves the iterator to the beginning of the hash table.*/
void hash_iter_next_primero(hash_iter_t* iter){
	if(!list_iter_get_current(iter->list_iter)) {
		hash_iter_next(iter);
	}
	
	iter->iterated = 0;
}

/******************************************************************
 * Primitives
 ******************************************************************/

/*Hash table*/

hash_t *hash_create(hash_destroy_data_t destroy_data){
	return hash_create_especifico(destroy_data, INITIAL_CAPACITY);
}

void hash_destroy(hash_t *hash){	
	hash_field_t* field;

	for(int i=0; i<hash->size; i++) {
		
		while(!list_is_empty(hash->table[i])){
			field = list_remove_first(hash->table[i]);

			if(hash->destroy_data) {
				hash->destroy_data(field->value);
			}

			free(field->key);
			free(field);
		}
		
		list_destroy(hash->table[i], free);
	}
	
	free(hash->table);
	free(hash);
}

bool hash_is_included(const hash_t *hash, const char *key){
	size_t pos = get_hash(key, hash->size);
	list_iter_t* iter = list_iter_create(hash->table[pos]);

	if(!iter) {
		return false;
	}
	
	bool is_included = false;
	hash_field_t* field;
	
	while(!list_iter_at_end(iter)){
		
		field = list_iter_get_current(iter);
		
		if(strcmp(field->key,key) == 0){
			is_included = true;
			break;
		}
		
		list_iter_continue(iter);
	}
	
	list_iter_destroy(iter);
	return is_included;
}

size_t hash_size(const hash_t *hash){
	return hash->items;
}

void *hash_remove(hash_t *hash, const char *key){
	size_t pos = get_hash(key, hash->size);
	list_iter_t* iter = list_iter_create(hash->table[pos]);
	
	if(!iter) {
		return NULL;
	}

	hash_field_t* field;
	void* value = NULL;
	
	while(!list_iter_at_end(iter)){
		field = list_iter_get_current(iter);
		
		if(strcmp(field->key,key) == 0) {
			value = field->value;
			hash_field_destroy(list_iter_remove(iter), hash);
			hash->items -= 1;
			
			if(get_coeficient(hash) <= REDUCTION_COEFICIENT) {
				hash_redimensionar(hash,(double)hash->size * REDUCTION_FACTOR);
			}

			break;
		}

		list_iter_continue(iter);
	}

	list_iter_destroy(iter);		
	return value;	
}

bool hash_store(hash_t *hash, const char *key, void *data){
	
	if(get_coeficient(hash) >= INCREASEMENT_COEFICIENT) {
		if(!hash_redimensionar(hash,(double)hash->size * INCRESEMENT_FACTOR)){
			return false;
		}
	}
	
	size_t pos = get_hash(key, hash->size); 
	hash_field_t* new_field = malloc(sizeof(hash_field_t));

	if(!new_field) {
		return false;
	}
	
	if(hash_is_included(hash,key)){
		hash_field_t* existent_field = hash_get_field(hash, key);

		if(hash->destroy_data){
			hash->destroy_data(existent_field->value);
		}
		
		existent_field->value = data;
		free(new_field);
	}
	
	else{
		
		new_field->key = malloc(sizeof(char) * (strlen(key)+1)); //\0
		strcpy(new_field->key, key);
		new_field->value = data;

		if(!list_add_first(hash->table[pos],new_field)) {
			return false;
		}

		hash->items += 1;
	}
	return true;
}

void *hash_get(const hash_t *hash, const char *key){
	hash_field_t* field = hash_get_field(hash, key);
	
	if(!field) {
		return NULL;
	}
	
	return field->value;
}

/*Iterator*/

void hash_iter_destroy(hash_iter_t* iter){	
	list_iter_destroy(iter->list_iter);
	free(iter);
}

hash_iter_t *hash_iter_create(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	
	if(!iter) {
		return NULL;
	}
	
	iter->hash = hash;	
	iter->current_pos = 0;
	iter->iterated = 0;
	iter->list_iter = list_iter_create(iter->hash->table[iter->current_pos]);
	
	if(!iter->list_iter){
		free(iter);
		return NULL;
	}
	
	hash_iter_next_primero(iter);
	return iter;
}

bool hash_iter_at_end(const hash_iter_t *iter){
	return (iter->iterated == iter->hash->items);
}

const char *hash_iter_get_current(const hash_iter_t *iter){
	if(hash_iter_at_end(iter)) {
		return NULL;
	}
	
	return ((hash_field_t*)list_iter_get_current(iter->list_iter))->key;
}

bool hash_iter_next(hash_iter_t *iter){

	if(hash_iter_at_end(iter)) {
		return false;
	}
	
	if(!list_iter_continue(iter->list_iter)){
		iter->current_pos += 1;
		
		if(iter->current_pos == iter->hash->size){
			iter->iterated +=1;
			return false;
		}
			
		list_iter_destroy(iter->list_iter);
		iter->list_iter = list_iter_create(iter->hash->table[iter->current_pos]);
		
		if(!iter->list_iter) {
			return false;
		}
	}
	
	while(!list_iter_get_current(iter->list_iter) && !list_iter_at_end(iter->list_iter)){
		list_iter_continue(iter->list_iter);
	}

	if(list_iter_at_end(iter->list_iter)){
	   return hash_iter_next(iter);
	}
	   
	iter->iterated +=1;
	return true;
}