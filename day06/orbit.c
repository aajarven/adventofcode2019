#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../utils/filereader.h"
#include"../utils/linkedlist.h"
#include"../utils/stringutils.h"
#include"../utils/tree.h"
#include"orbit.h"

#define LINELEN 9

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Input file missing. Give the input file as a "
				"command line argument.\n");
		exit(EXIT_FAILURE);
	}

	int n_orbits = count_lines(argv[1]);
	char lines[n_orbits][LINELEN];
	read_lines(argv[1], lines[0], LINELEN, n_orbits);

	struct orbiter* com = create_orbiter("COM");
	struct bstree_node* root = (struct bstree_node*) malloc(sizeof(struct bstree_node));
	root->content = com;
	root->left = NULL;
	root->right = NULL;
	root->parent = NULL;

	for (int i=0; i<n_orbits; i++){
		char* line = lines[i];
		strip_newline(line);
		char* central_name = strtok(line, ")");
		char* orbiter_name = strtok(NULL, ")");

		struct orbiter* central = get_orbiter(root, central_name);
		struct orbiter* orbiter = get_orbiter(root, orbiter_name);
		add_orbiter(central, orbiter);
	}

	printf("Cumulative orbiters: %d\n", count_orbiters(com, 0)); 

	free_orbiters(com);
	free_btree(root, true);
	exit(EXIT_SUCCESS);
}

/*
 * Compare orbiters based on their names. Any other information is not
 * considered, i.e. two orbiters with the same name but different parents are
 * equal.
 *
 * Even though the orbiter pointers are passed as void*, they must be castable
 * to struct orbiter*.
 */
int orbiter_compare(void* orbiter1, void* orbiter2){
	struct orbiter* o1 = (struct orbiter*) orbiter1;
	struct orbiter* o2 = (struct orbiter*) orbiter2;
	return strcmp(o1->name, o2->name);
}

/*
 * Get a new/existing node for the orbiter with the given name.
 */
struct orbiter* get_orbiter(struct bstree_node* root, char* name){
	struct orbiter* new_orbiter = create_orbiter(name);
	struct bstree_node* orbiter_node = search(root, new_orbiter, orbiter_compare);
	if (orbiter_node->content != new_orbiter){
		free(new_orbiter);
	}
	return (struct orbiter*) orbiter_node->content;
}

/*
 * Add a new orbiter for the given central object. If the central did not have
 * orbiters beforehand, a new linked list is created with the given orbiter as
 * the only member. Otherwise the new orbiter is just appended to the end of
 * the old linked list.
 */
void add_orbiter(struct orbiter* central, struct orbiter* new_orbiter){
	if (central->orbiters == NULL) {
		central->orbiters = create_llist(new_orbiter);
	} else {
		llist_append(central->orbiters, new_orbiter);
	}
	new_orbiter->central = central;
}

void print_orbiter(struct bstree_node* orbiter_node){
	struct orbiter* o = (struct orbiter*) orbiter_node->content;
	if (o->central) {
		printf("%s orbits %s\n", o->name, o->central->name);
	} else {
		printf("%s is still\n", o->name);
	}
}

void print_orbiters(struct orbiter* o){
	if (o->orbiters == NULL){
		printf("%s has no orbiters\n", o->name);
	} else {
		printf("%s has the following orbiters:", o->name);
		struct llist_node* n = o->orbiters;
		struct orbiter* current_orbiter = (struct orbiter*) n->content;
		printf("%s\n", current_orbiter->name);
		while (n != NULL) {
			printf(" %s", ((struct orbiter*) n->content)->name);
			n = n->child;
		}
		printf("\n");
	}
}

struct orbiter* create_orbiter(char* name){
	struct orbiter* new_orbiter = (struct orbiter*) malloc(sizeof(struct orbiter));
	new_orbiter->name = name;
	new_orbiter->central = NULL;
	new_orbiter->orbiters = NULL;
	new_orbiter->orbiter_count = 0;
	return new_orbiter;
}

void free_orbiters(struct orbiter* central){
	struct llist_node* orbiter_node = central->orbiters;
	while (orbiter_node != NULL) {
		free_orbiters((struct orbiter*) orbiter_node->content);
		orbiter_node = orbiter_node->child;
	}
	if (central->orbiters != NULL) {
		free_llist(central->orbiters);
	}
}

int count_orbiters(struct orbiter* central, int starting_level){
	if (central->orbiters == NULL){
		return starting_level;
	} else {
		int orbit_count = 0;
		struct llist_node* node = central->orbiters;
		while(node != NULL){
			orbit_count += count_orbiters((struct orbiter*) node->content, starting_level + 1);
			node = node->child;
		}
		return orbit_count + starting_level;
	}
}
