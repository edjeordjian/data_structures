#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdbool.h>

/*******************************************************************
 * Structures
 ******************************************************************/
 
typedef struct list list_t;
typedef struct list_iter list_iter_t;

/*******************************************************************
 * Primitives
 ******************************************************************/

/*Inner iterator*/

/*Applies 'visit' to every element in the list, while the result
of the function is 'true'.
The result of the iteration is stored in 'extra' (the last argument), 
if specified (not NULL).*/
void list_iterate(list_t *list, bool visit(void *data, void *extra), void *extra);

/*Outer iterator*/

/*Creates a new iterator.*/
list_iter_t *list_iter_create(list_t *list);

/*Moves the iterator to the next element in the list.
Returns false is the iterator cannot be moved forward.*/
bool list_iter_continue(list_iter_t *iter);

/*Returns the current element of the iterator.*/
void *list_iter_get_current(const list_iter_t *iter);

/*Returns true if the iterator is at the end of the list 
(it cannot be moved any further).*/
bool list_iter_at_end(const list_iter_t *iter);

/*Destroys the iterator.*/
void list_iter_destroy(list_iter_t *iter);

/*Inserts the given data into the list, in the current position of 
the iterator. Returns 'false' in case of an error.*/
bool list_iter_insert(list_iter_t *iter, void *data);

/*Removes the element of the list in the current position of
the iterator.*/
void *list_iter_remove(list_iter_t *iter);

/*List*/

/*Creates a new empty list.*/
list_t *list_create(void);

/*Returns true if the list is empty.*/
bool list_is_empty(const list_t *list);

/*Adds a new element at the beginning of the list. 
Returns false in case of an error.*/
bool list_add_first(list_t *list, void *data);

/*Adds a new element at the end of the list.
Returns false in case of an error.*/
bool list_add_last(list_t *list, void *data);

/*Removes and returns the first element of the list.*/
void *list_remove_first(list_t *list);

/*Returns the first element of the list.*/
void *list_get_first(const list_t *list);

/*Returns the last element of the list.*/
void *list_get_last(const list_t* list);

/*Returns the size of the list.*/
size_t list_get_size(const list_t *list);

/*Destroys the list. If a destroy_data function is specified (not NULL), 
that function will be applied to every element in the list 
before being destroyed (which can be useful, for example, if memory 
was allocated to create the stored data).*/
void list_destroy(list_t *list, void destroy_data(void *));

#endif // LIST_H