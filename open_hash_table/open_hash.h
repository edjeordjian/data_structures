#ifndef HASH_H
#define HASH_H
#include <stdbool.h>
#include <stddef.h>

/*
Hash table ("Dictionary") with open addressing. 
Only strings are allowed as keys.  
Needs the list to work.
*/

/*******************************************************************
 * Structures				
 ******************************************************************/

struct hash;
struct hash_iter;

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destroy_data_t)(void *);

/*******************************************************************
 * Primitives				
 ******************************************************************/

/*Hash table*/

/*Creates a new empty hash table.*/
hash_t *hash_create(hash_destroy_data_t destroy_data);

/* Stores a new element in the hash table.
If the given key exists in the hash table, it is replaced.
Returns false if there was an error.*/
bool hash_store(hash_t *hash, const char *key, void *data);

/*Removes an element from the hash table and returns its data.*/
void *hash_remove(hash_t *hash, const char *key);

/*Returns the data of the element in the hash  table associated with
the given key. */
void *hash_get(const hash_t *hash, const char *key);

/*Returns 'true' if the given key is in the hash table.*/
bool hash_is_included(const hash_t *hash, const char *key);

/*Returns the number of elements in the hash table.*/
size_t hash_size(const hash_t *hash);

/*Destroys the hash table and every element on it (whit the provided data 
destroying function) */
void hash_destroy(hash_t *hash);

/*Iterator*/

/*Creates a new hash table iterator*/
hash_iter_t *hash_iter_create(const hash_t *hash);

/*Moves the iterator to the next element in the hash table.*/
bool hash_iter_next(hash_iter_t *iter);

/*Returns the current key.*/ 
const char *hash_iter_get_current(const hash_iter_t *iter);

/*Returns true if the iterator is at the end of the hash table
(cannot move forward).*/
bool hash_iter_at_end(const hash_iter_t *iter);

/*Destroys the iterator.*/
void hash_iter_destroy(hash_iter_t* iter);

#endif // HASH_H
