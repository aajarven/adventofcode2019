#include<stdlib.h>
#include"linkedlist.h"

/*
 * Add a new node containing the given content to the linked list defined by
 * the head node.
 */
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

/*
 * Create a new linked list, the only node of which holds the given content.
 */
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

/*
 * Return the index of the node containing content in the linked list starting
 * from the given root. If the content is not found, returns 0.
 */
int index_in_list(struct llist_node* root, void* content){
	int index = 0;
	while(root != NULL) {
		if (root->content == content){
			return index;
		}
		root = root->child;
		index++;
	}

	// not found
	return -1;
}
