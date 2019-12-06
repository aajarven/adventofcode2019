#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include"tree.h"

/*
 * Searches for a node with the given content from the binary tree starting at
 * the given root. Descending to left/right child is determined using the given
 * comparator function.
 *
 * If a matching node is found, it is returned. Otherwise a new node with the
 * given content is inserted to the tree and that node is returned.
 */
struct bstree_node search(struct bstree_node* root, void* content,
		int comparator(void* content1, void* content2)) {
	while (true){
		printf("gon compare\n");
		printf("%p\n", root->content);
		int compare_result = comparator(content, root->content);
		printf("%d\n", compare_result);
		if (compare_result == 0){
			return *root;
		} else if (compare_result < 0){
			if (root->left != NULL) {
				root = root->left;
			} else {
				struct bstree_node new = {content, root, NULL, NULL};
				root->left = &new;
				return new;
			}
		} else if (compare_result > 0){
			if (root->right != NULL) {
				root = root->right;
			} else {
				struct bstree_node new = {content, root, NULL, NULL};
				root->right = &new;
				return new;
			}
		}
	}
}

/*
 * Call the given print_function for all nodes in post order.
 */
void post_order_print(struct bstree_node* root, void* print_function(struct bstree_node* root)){
	if (root->left) {
		post_order_print(root->left, print_function);
	}
	if (root->right) {
		post_order_print(root->right, print_function);
	}
	print_function(root);
}
