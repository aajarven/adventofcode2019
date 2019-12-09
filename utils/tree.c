#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include"tree.h"

#include"../day06/orbit.h"
/*
 * Searches for a node with the given content from the binary tree starting at
 * the given root. Descending to left/right child is determined using the given
 * comparator function.
 *
 * If a matching node is found, it is returned. Otherwise a new node with the
 * given content is inserted to the tree and that node is returned.
 */
struct bstree_node* search(struct bstree_node* root, void* content,
		int comparator(void* content1, void* content2)) {
	while (true){
		int compare_result = comparator(content, root->content);
		if (compare_result == 0){
			return root;
		} else if (compare_result < 0){
			if (root->left != NULL) {
				root = root->left;
			} else {
				struct bstree_node* new = create_node(content, root);
				root->left = new;
				return new;
			}
		} else if (compare_result > 0){
			if (root->right != NULL) {
				root = root->right;
			} else {
				struct bstree_node* new = create_node(content, root);
				root->right = new;
				return new;
			}
		}
	}
}

/*
 * Allocate memory for a new node and populate it with the given content and
 * parent. Return pointer to the created node.
 */
struct bstree_node* create_node(void* content, struct bstree_node* parent){
	struct bstree_node* new = (struct bstree_node*) malloc(sizeof(struct bstree_node));
	new->content = content;
	new->parent = parent;
	new->left = NULL;
	new->right = NULL;
	return new;
}

/*
 * Call the given print_function for all nodes in post order.
 */
void post_order_print(struct bstree_node* root, void print_function(struct bstree_node* root)){
	if (root->left) {
		post_order_print(root->left, print_function);
	}
	if (root->right) {
		post_order_print(root->right, print_function);
	}
	print_function(root);
}

/*
 * Free all children of the given node. If free_contents is true, also frees
 * their contents.
 */
void free_btree(struct bstree_node* root, bool free_contents){
	if (root->left != NULL){
		free_btree(root->left, free_contents);
	}
	if (root->right != NULL) {
		free_btree(root->right, free_contents);
	}
	if (free_contents) {
		free(root->content);
	}
	free(root);
}
