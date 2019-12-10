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

	struct orbiter* orbiter_you = NULL;
	struct orbiter* orbiter_san = NULL;

	for (int i=0; i<n_orbits; i++){
		char* line = lines[i];
		strip_newline(line);
		char* central_name = strtok(line, ")");
		char* orbiter_name = strtok(NULL, ")");

		struct orbiter* central = get_orbiter(root, central_name);
		struct orbiter* orbiter = get_orbiter(root, orbiter_name);
		add_orbiter(central, orbiter);

		if (strcmp(orbiter_name, "YOU") == 0){
			orbiter_you = orbiter;
		} else if (strcmp(orbiter_name, "SAN") == 0){
			orbiter_san = orbiter;
		}
	}

	printf("Cumulative orbiters: %d\n", count_orbiters(com, 0)); 

	struct llist_node* route_you = route_to_node(com, orbiter_you);
	struct llist_node* route_san = route_to_node(com, orbiter_san);
	printf("Transfers to santa: %d\n", count_transfers(route_you, route_san) - 2);

	free_llist(route_you);
	free_llist(route_san);
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

/*
 * Print the status of an orbiter: either who it orbits or that it is still.
 */
void print_orbiter(struct bstree_node* orbiter_node){
	struct orbiter* o = (struct orbiter*) orbiter_node->content;
	if (o->central) {
		printf("%s orbits %s\n", o->name, o->central->name);
	} else {
		printf("%s is still\n", o->name);
	}
}

/*
 * Prints a list of orbiters for a central body.
 */
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

/*
 * Allocate memory and initializes a new orbiter with the given name.
 */
struct orbiter* create_orbiter(char* name){
	struct orbiter* new_orbiter = (struct orbiter*) malloc(sizeof(struct orbiter));
	new_orbiter->name = name;
	new_orbiter->central = NULL;
	new_orbiter->orbiters = NULL;
	new_orbiter->orbiter_count = 0;
	return new_orbiter;
}

/*
 * Free all orbiters directly or indirectly orbiting the given central.
 */
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

/*
 * Return the cumulative number of direct and indirect orbiters under the given
 * central body, located at the given starting_level orbits below the COM in
 * the orbit tree.
 */
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

/*
 * Count transfers needed to get from n1 to n2. Note that this includes
 * transfers from n1 to its central and from the central of n2 to n2.
 */
int count_transfers(struct llist_node* n1, struct llist_node* n2){
	int transfers1 = 0;
	while(n1 != NULL){
		int transfers2 = index_in_list(n2, n1->content);
		if (transfers2 >= 0) {
			return transfers1 + transfers2;
		}
		n1 = n1->child;
		transfers1++;
	}
	return -1;
}

/*
 * Print the name of the orbiter (the given pointer has to be castable to
 * struct orbiter*).
 */
void print_orbiter_name(void* o){
	printf("%s\n", ((struct orbiter*) o)->name);
}

/*
 * Returns the route from source to destination as a linked list. The route
 * must be findable by traversing the orbiter tree up from the leaves towards
 * the root. If no route is found, returns NULL. Otherwise the linked list
 * contains all nodes on the path from source to destination, including the
 * ends.
 */
struct llist_node* route_to_node(struct orbiter* destination, struct orbiter* source){
	if (destination == source){
		struct llist_node* route = create_llist(destination);
		return route;
	}
	
	struct llist_node* child = destination->orbiters;
	while (child != NULL){
		struct llist_node* route = route_to_node(child->content, source);
		if (route != NULL) {
			llist_append(route, destination);
			return route;
		}
		child = child->child;
	}

	return NULL;
}

