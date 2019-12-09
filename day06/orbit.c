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
		char* central_name = strtok(line, ")");
		char* orbiter_name = strtok(NULL, ")");

		struct orbiter* central = get_orbiter(&root, central_name);
		struct orbiter* orbiter = get_orbiter(&root, orbiter_name);
		printf("orbiter name: %s\n", orbiter->name);
		printf("orbiter list of the central: %p\n", central->orbiters);
		add_orbiter(central, orbiter);
		printf("orbiter list of the central: %p\n", central->orbiters);
		printf("\n");
	}
	printf("gon print\n");
	print_orbiters((struct orbiter*) root.content);

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
	// TODO this leaks
	return (struct orbiter*) orbiter_node->content;
}

void add_orbiter(struct orbiter* central, struct orbiter* new_orbiter){
	if (central->orbiters == NULL) {
		central->orbiters = create_llist(&new_orbiter);
	} else {
		llist_append(central->orbiters, &new_orbiter);
	new_orbiter->central = central;
	print_orbiters(central);
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
	return new_orbiter;
}
