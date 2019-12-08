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
struct bstree_node search(struct bstree_node* root, void* content,
		int comparator(void* content1, void* content2)) {
	printf("looking for a place for %s\n", ((struct orbiter*) content)->name);
	while (true){
		printf("inspecting %s.\n", ((struct orbiter*) root->content)->name);
		int compare_result = comparator(content, root->content);
		printf("compare retult: %d\n", compare_result);
		if (compare_result == 0){
			printf("match found\n");
			return *root;
		} else if (compare_result < 0){
			if (root->left != NULL) {
				printf("going left\n");
				root = root->left;
			} else {
				printf("creating new node as the left child\n");
				struct bstree_node* new = create_node(content, root);
				root->left = new;
				printf("%p\n", root->left);
				return *new;
			}
		} else if (compare_result > 0){
			if (root->right != NULL) {
				printf("going right\n");
				root = root->right;
			} else {
				printf("creating new node as the right child\n");
				struct bstree_node* new = create_node(content, root);
				root->right = new;
				printf("%p\n", root->right);
				return *new;
			}
		}
	}
}

struct bstree_node* create_node(void* content, struct bstree_node* parent){
	struct bstree_node* new = (struct bstree_node*) malloc(sizeof(struct bstree_node));
	new->content = content;
	new->parent = parent;
	return new;
}

/*
 * Call the given print_function for all nodes in post order.
 */
void post_order_print(struct bstree_node* root, void print_function(struct bstree_node* root)){
	if (root->left) {
		printf("has left\n");
		post_order_print(root->left, print_function);
	} else {
		printf("no left\n");
	}
	if (root->right) {
		printf("has right\n");
		post_order_print(root->right, print_function);
	} else {
		printf("no right\n");
	}
	print_function(root);
}
