#ifndef BST_H
#define BST_H
#include <stdbool.h>
#include <string.h>

/*
Binary Search Tree.
(needs the stack to work).
*/

/*******************************************************************
 * Structures					
 ******************************************************************/

typedef struct bst bst_t;
typedef int (*bst_compare_key_t) (const char *, const char *); //Comparison function
typedef void (*bst_destroy_data_t) (void *); // Destructor
typedef struct bst_iter bst_iter_t; //Outer iterator

/*******************************************************************
 * Primitives				
 ******************************************************************/

/*Creates a new empty bst.*/
bst_t* bst_create(bst_compare_key_t cmp, bst_destroy_data_t destroy_data);

/* Stores a new element in the bst.
If the specified key is already in use, it is replaced.
Returns false in the case of an error.*/
bool bst_store(bst_t *bst, const char *key, void *data);

/* Removes an element from the bst, and returns its data.*/
void *bst_remove(bst_t *bst, const char *key);

/*Returns the data associated with the given key.*/
void *bst_get(const bst_t *bst, const char *key);

/*Returns true if the key exists in the bst. */
bool bst_contains(const bst_t *bst, const char *key);

/*Returns the number of elements in the bst.*/
size_t bst_size(bst_t *bst);

/* Destroys the bst, applying to every element of the
bst the specified destroying function */
void bst_destroy(bst_t *bst);

/*Inner iterator*/

/* Applies the function 'visit' to every element in the bst,
while that function returns true. If 'extra' argument is specified
(not NULL), the result of the iteration is saved on it.*/
void bst_visit(bst_t *bst, bool visit(const char *, void *, void *), void *extra);

/*Outer iterator*/

/* Creates a new iterator*/
bst_iter_t *bst_iter_create(const bst_t *bst);

/* Moves the iterator to the next element in the bst.
Returns false if moving forward is not possible.*/
bool bst_iter_next(bst_iter_t *iter);

/* Returns the key associated with iterator's current element.*/
const char *bst_iter_get_current(const bst_iter_t *iter);

/* Returns true if the iterator is at the end of the bst (it
cannot move any further).*/
bool bst_iter_at_end(const bst_iter_t *iter);

/*Destroys the iterator.*/
void bst_iter_destroy(bst_iter_t* iter);

#endif // BST_H
