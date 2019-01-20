#ifndef HASH_H
#define HASH_H
#include <stdbool.h>
#include <stddef.h>

/*
Hash table ("Dictionary") with closed addressing. 
Only strings are allowed as keys.
*/

/*******************************************************************
 *Structures				
 ******************************************************************/

struct hash;
struct hash_iter;
typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destroy_data_t)(void *);

/*******************************************************************
 *Primitives			
 ******************************************************************/

/*Hash table*/

/*Creates a new hash table.*/
hash_t *hash_create(hash_destroy_data_t destroy_data);

/*Stores a new element in the hash table. If the key already exists , is 
replaced. Returns false in the case of an error.*/
bool hash_store(hash_t *hash, const char *key, void *data);

/*Removes an element from the hash table, and returns its data.*/
void *hash_remove(hash_t *hash, const char *key);

/*Returns the data associated with the given key.*/
void *hash_get(const hash_t *hash, const char *key);

/*Returns true if the key is in the hash table.*/
bool hash_is_in(const hash_t *hash, const char *key);

/*Returns the number of elements in the hash table.*/
size_t hash_size(const hash_t *hash);

/*Destroys the hash table.*/
void hash_destroy(hash_t *hash);

/*Iterator*/

/*Creates an iterator.*/
hash_iter_t *hash_iter_create(const hash_t *hash);

/*Moves the iterator to the next element in the hash.
Returns false if it is not possible to move forward*/
bool hash_iter_next(hash_iter_t *iter);

/*Returns the key of the current element being iterated.*/
const char *hash_iter_get_current(const hash_iter_t *iter);

/*Returns true if the iterator won't move any further.*/
bool hash_iter_at_end(const hash_iter_t *iter);

/*Destroys the iterator*/
void hash_iter_destroy(hash_iter_t* iter);

#endif // HASH_H
