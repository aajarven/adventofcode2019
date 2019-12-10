struct llist_node{
	void* content;
	struct llist_node* child;
};

void llist_append(struct llist_node* head, void* new_node_content);
struct llist_node* create_llist(void* content);
void free_llist(struct llist_node* head);
int index_in_list(struct llist_node* root, void* content);
