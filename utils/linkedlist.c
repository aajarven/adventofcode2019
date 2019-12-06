#include<stdlib.h>
#include"linkedlist.h"

void llist_append(struct llist_node head, void* new_node_content){
	struct llist_node current = head;
	while (current.child != NULL) {
		current = *current.child;
	}
	struct llist_node new = {new_node_content, NULL};
	current.child = &new;
}
