struct llist_node{
	void* content;
	struct llist_node* child;
};

void llist_append(struct llist_node* head, void* new_node_content);
struct llist_node* create_llist(void* content);
