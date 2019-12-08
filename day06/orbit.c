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

	struct orbiter com = {"COM", NULL, NULL};
	struct bstree_node root = {&com, NULL, NULL, NULL};
	for (int i=0; i<n_orbits; i++){
		char* line = lines[i];
		strip_newline(line);
		printf("read line %s\n", line);
		char* orbitee_name = strtok(line, ")");
		printf("orbitee: %s\n", orbitee_name);
		char* orbiter_name = strtok(NULL, ")");
		printf("orbiter: %s\n", orbiter_name);

		struct orbiter orbitee = get_orbiter(root, orbitee_name);
		struct orbiter orbiter = get_orbiter(root, orbiter_name);
		add_orbiter(orbitee, orbiter);
		printf("%p %p\n", root.left, root.right);
		printf("\n");
	}
	print_orbiters(com);

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
struct orbiter get_orbiter(struct bstree_node root, char* name){
	struct orbiter* new_orbiter = (struct orbiter*) malloc(sizeof(struct orbiter));
	printf("name for the new orbiter: %s\n", name);
	new_orbiter->name = name;
	struct bstree_node orbiter_node = search(&root, new_orbiter, orbiter_compare);
	printf("got orbiter node: ");
	print_orbiter(&orbiter_node);
	return (*(struct orbiter*) orbiter_node.content);
}

void add_orbiter(struct orbiter orbitee, struct orbiter new_orbiter){
	if (orbitee.orbiters == NULL) {
		struct llist_node* new_list = (struct llist_node*) malloc(sizeof(struct llist_node));
		new_list->content = &new_orbiter;
		orbitee.orbiters = new_list;
	} else {
		llist_append(*orbitee.orbiters, &new_orbiter);
	}
}

void print_orbiter(struct bstree_node* orbiter_node){
	struct orbiter* o = (struct orbiter*) orbiter_node->content;
	if (o->central) {
		printf("%s orbits %s\n", o->name, o->central->name);
	} else {
		printf("%s is still\n", o->name);
	}
}

void print_orbiters(struct orbiter o){
	if (o.orbiters == NULL){
		printf("%s has no orbiters\n", o.name);
	} else {
		printf("%s has the following orbiters:", o.name);
		struct llist_node* n = o.orbiters;
		while (n != NULL) {
			printf(" %s", ((struct orbiter*) n->content)->name);
			n = n->child;
		}
		printf("\n");
	}
}

