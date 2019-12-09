#include<stdlib.h>
#include"linkedlist.h"

void llist_append(struct llist_node* head, void* new_node_content){
	struct llist_node* current = head;
	while (current->child != NULL) {
		current = current->child;
	}
	struct llist_node* new = (struct llist_node*) malloc(sizeof(struct llist_node));
	new->content = new_node_content;
	new->child = NULL;
	current->child = new;
}

struct llist_node* create_llist(void* content){
	struct llist_node* new_llist = (struct llist_node*) malloc(sizeof(struct llist_node));
	new_llist->content = content;
	new_llist->child = NULL;
	return new_llist;
}

/*
 * Free all linked list elements. Does not affect the contents.
 */
void free_llist(struct llist_node* head){
	if (head->child != NULL){
		free_llist(head->child);
	}
	free(head);
}
