struct bstree_node{
	void* content;
	struct bstree_node* parent;
	struct bstree_node* left;
	struct bstree_node* right;
};

struct bstree_node search(struct bstree_node* root, void* content,
		int comparator(void* content1, void* content2));
void post_order_print(struct bstree_node* root, void* print_function(struct bstree_node* root));
