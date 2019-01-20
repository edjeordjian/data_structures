#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "bst.h"

char* strdup(const char* s);

/*******************************************************************
 * Structures			
 ******************************************************************/

typedef struct bst_node{
	struct bst_node* left;
	struct bst_node* right;
	char* key;
	void* data;
}bst_node_t;

struct bst{
	bst_node_t* root;
	bst_compare_key_t cmp; 
	bst_destroy_data_t destroy_data;
	size_t items; 
};

struct bst_iter {
	stack_t* nodes;
};

/*******************************************************************
 * Auxiliary Functions		
 ******************************************************************/

/*Creates a new node.*/
bst_node_t* bst_node_create(const char* key, void* data){
	bst_node_t* node = malloc(sizeof(bst_node_t));

	if(!node){ 
		return NULL;
	}
	
	node->key = malloc(sizeof(char) * (strlen(key)+1)); //\0
	
	if(!node->key){	
		free(node);
		return NULL;
	}
	
	strcpy(node->key, key);
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

/*Stores a new node in the bst.*/
bool bst_node_store(bst_node_t* node, bst_t* bst, const char* key, void* data){
	if(bst->cmp(node->key, key) == 0){
		void* old_data = node->data;
		node->data = data;

		if(bst->destroy_data) {
			bst->destroy_data(old_data);
		}

		bst->items-=1; //Because the storing function adds one.
	}

	else if(bst->cmp(key, node->key)<0){
		
		if(node->left){
			return bst_node_store(node->left, bst, key, data);
		}
		
		else{
			node->left = bst_node_create(key, data);
			if(!node->left) {
				return false;
			}
		}
	}
	
	else{
		
		if(node->right){
			return bst_node_store(node->right, bst, key, data);
		}
		
		else{
			node->right = bst_node_create(key, data);

			if(!node->right) {
				return false;
			}
		}
	}
	
	return true;
}

/* Searches for a node with the given key, starting from the given node.*/
bst_node_t* bst_node_search(bst_compare_key_t cmp, bst_node_t* node, const char* key, bool get_father, bst_node_t* father){
	if(cmp(node->key, key) == 0){
		if(get_father){ 
			return father;
		}

		return node;
	}
	
	if(cmp(key, node->key) < 0){		
		if(node->left){
			return bst_node_search(cmp, node->left, key, get_father, node);
		}
		
		return NULL;
	}
	
	else{
		if(node->right){
			return bst_node_search(cmp, node->right, key, get_father, node);
		}

		return NULL;
	}
}

/*Returns the leftmost node, starting from the given node.*/
bst_node_t* bst_node_min(bst_node_t* node){
	while(node->left){
		node = node -> left;
	}

	return node;
}

/*Returns the rightmost node, starting from the given node.*/
bst_node_t* bst_node_max(bst_node_t* node){
	while(node->right){
		node = node -> right;
	}

	return node;
}

/* Swaps two nodes (interchanging their keys and data).
Returns false in the case of an error.*/
bool bst_node_swap(bst_node_t* node_a, bst_node_t* node_b){
	/*key swap*/
	char* key_aux = strdup(node_a->key);
	free(node_a->key);
	node_a->key = strdup(node_b->key);
	free(node_b->key);
	node_b->key = strdup(key_aux);
	free(key_aux);
	
	/*value swap*/
	void* value_aux = NULL;
	value_aux = node_a->data;
	node_a->data = node_b->data;
	node_b->data = value_aux;
	return true;
}

/* Removes the node with the given key, searching from the given node.*/
void* bst_node_remove(bst_node_t* node, bool is_left, bst_node_t* father, bst_t* bst, const char* key);

void bst_node_remove_leaf(bool is_left, bst_node_t* father){
	if(is_left) {
		father->left = NULL;
	}

	else {
		father->right = NULL;
	}
}

void bst_node_remove_1_son(bst_node_t* node, bool is_left, bst_node_t* father){
	if(node->right && !node->left){

		if(is_left) {
			father->left = node->right;
		}

		else {
			father->right = node->right;
		}

		return;
	}
		
	if (!node->right && node->left){

		if(is_left) {
			father->left = node->left;
		}

		else {
			father->right = node->left;
		}
	}
}

void* bst_node_remove_2_sons(bst_node_t* node, bool is_left, bst_node_t* father, bst_t* bst){
	bool esleft = false;
	bst_node_t* node_aux = bst_node_min(node->right);
	bst_node_t* father_aux = bst_node_search(bst->cmp, bst->root, node_aux->key, true, NULL);
			
	if(!bst_node_swap(node, node_aux)) {
		return NULL;
	}

	if(bst->cmp(father_aux->left->key,node_aux->key)==0) {
		esleft = true;
	}	

	return bst_node_remove(node_aux, esleft, father_aux, bst, node_aux->key);
}

void* bst_node_remove(bst_node_t* node, bool is_left, bst_node_t* father, bst_t* bst, const char* key){	
	if(bst->cmp(node->key, key) == 0){
		void* data = node->data;
		
		//root
		if(!father){	
			if(!node->left && !node->right){
				bst->root = NULL;
			}
				
			else{
				bst_node_t* node_aux = NULL;

				if(node->right){					
					node_aux = bst_node_min(node->right);
					
					if(!bst_node_swap(node, node_aux)) {
						return NULL;
					}

					bst->root = node;
					return bst_node_remove(node->right, false, node, bst, node_aux->key);
				}

				node_aux = bst_node_max(node->left);
				
				if(!bst_node_swap(node, node_aux)){ 
					return NULL;
				}

				bst->root = node;
				return bst_node_remove(node->left, true, node, bst, node_aux->key); 
			}
		}
		
		else if(!node->right && !node->left){
			bst_node_remove_leaf(is_left, father);
		}

		else if(!node->right || !node->left){
			bst_node_remove_1_son(node, is_left, father);
		}
		
		else{
			return bst_node_remove_2_sons(node, is_left, father, bst);
		}
		
		free(node->key);
		free(node);
		return data;
	}
	
	else if(bst->cmp(key, node->key)<0)	{
		if(node->left){ 
			return bst_node_remove(node->left, true, node, bst, key);
		}

		return NULL;
	}
	
	else{
		if(node->right) {
			return bst_node_remove(node->right, false, node, bst, key);
		}
		return NULL;
	}
}

/* Auxiliary function for destroying the bst.*/
void bst_destroy_aux(bst_node_t* node, bst_destroy_data_t destroy_data){
	if(node->left){
		bst_destroy_aux(node->left, destroy_data);
	}
	
	if(node->right){
		bst_destroy_aux(node->right, destroy_data);
	}
	
	if(destroy_data){
		destroy_data(node->data);
	}

	free(node->key);
	free(node);
}

/*Auxiliary function for the iteration of the bst.
Returns false if the iteration must be stopped.*/
bool bst_right_aux(bst_node_t* node, bool visit(const char *, void *, void *), void *extra){
	if(!node) {
		return true;
	}

	if(bst_right_aux(node->left, visit, extra)){	
		if(!visit(node->key, node->data, extra)) {
			return false;
		}
		
		return bst_right_aux(node->right, visit, extra);
	}
	
	return false;
}

/* Stacks every left son of the given node.
Returns false in case of an error.*/
bool stack_left(stack_t* stack, bst_node_t* node) {
	bst_node_t* current = node;	

	while(current){	
		if(!stack_push(stack, current)) {
			return false;
		}

		current = current->left;
	}

	return true;
}

/*******************************************************************
 * Primitives	
 ******************************************************************/

/*bst*/

bst_t* bst_create(bst_compare_key_t cmp, bst_destroy_data_t destroy_data){
	bst_t* bst = malloc(sizeof(bst_t));
	
	if(!bst) {
		return NULL;
	}
	
	bst->root = NULL;
	bst->cmp = cmp;
	bst->destroy_data = destroy_data;
	bst->items = 0;
	return bst;
}

bool bst_store(bst_t *bst, const char *key, void *data){	
	if(!bst->root){
		bst->root = bst_node_create(key, data);

		if(!bst->root){
			return false;
		}
	}
	
	else if(!bst_node_store(bst->root, bst, key, data)){ 
		return false;
	}

	bst->items+=1;
	return true;
}

void *bst_remove(bst_t *bst, const char *key){
	if(!bst->root) {
		return NULL;
	}
	
	bst_node_t* node = bst_node_search(bst->cmp, bst->root, key, false, NULL);
	
	if(!node) {
		return NULL;
	}

	bst_node_t* father = bst_node_search(bst->cmp, bst->root, key, true, NULL);
	bool is_left = false;
	
	if(father){
		if(father->left){
			if(bst->cmp(father->left->key, node->key)==0){
				is_left = true;
			}
		}
	}
	
	void* value = bst_node_remove(node, is_left, father, bst, key);
	bst->items-=1;
	return value;
}

void *bst_get(const bst_t *bst, const char *key){
	if(!bst->root) {
		return NULL;
	}

	bst_node_t* node = bst_node_search(bst->cmp, bst->root, key, false, NULL);
	
	if(!node) {
		return NULL;
	}

	return node->data;
}

bool bst_contains(const bst_t *bst, const char *key){
	if(!bst->root) {
		return false;
	}

	return bst_node_search(bst->cmp, bst->root, key, false, NULL);
}

size_t bst_size(bst_t *bst){
	return bst->items;
}

void bst_destroy(bst_t *bst){
	if(bst->root) {
		bst_destroy_aux(bst->root, bst->destroy_data);
	}
		
	free(bst);
}

/*Inner Iterator*/

void bst_visit(bst_t *bst, bool visit(const char *, void *, void *), void *extra){
	bst_right_aux(bst->root, visit, extra);
}

/*Outer iterator*/

bst_iter_t *bst_iter_create(const bst_t *bst){
	bst_iter_t* iter = malloc(sizeof(iter));
	
	if(!iter) {
		return NULL;
	}
	
	stack_t* nodes = stack_create();

	if(!nodes){ 
		free(iter);
		return NULL;
	}
	
	iter->nodes = nodes;

	if(!stack_left(iter->nodes, bst->root)){
		bst_iter_destroy(iter);
		return NULL;
	}
	
	return iter;
}

bool bst_iter_next(bst_iter_t *iter){
	if(stack_is_empty(iter->nodes)){
		return false;
	}

	bst_node_t* poped = stack_pop(iter->nodes);
	
	if(poped->right){
		return stack_left(iter->nodes,poped->right);
	}
	
	return true;
}

const char *bst_iter_get_current(const bst_iter_t *iter){
	bst_node_t* current = stack_top(iter->nodes);
	
	if(!current) {
		return NULL;
	}

	return current->key;
}

bool bst_iter_at_end(const bst_iter_t *iter){
	return stack_is_empty(iter->nodes);
}

void bst_iter_destroy(bst_iter_t* iter){
	stack_destroy(iter->nodes);
	free(iter);
}