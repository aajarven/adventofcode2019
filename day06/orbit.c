#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../utils/filereader.h"
#include"../utils/linkedlist.h"
#include"../utils/tree.h"
#include"orbit.h"

#define LINELEN 8

struct orbiter{
	char* name;
	struct orbiter* central;
	struct llist_node* orbitees;
};

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Input file missing. Give the input file as a "
				"command line argument.\n");
		exit(EXIT_FAILURE);
	}

	int n_orbits = count_lines(argv[1]);
	char lines[n_orbits][LINELEN];
	read_lines(argv[1], lines[0], LINELEN, n_orbits);

	struct bstree_node root = {&"COM", NULL, NULL, NULL};
	for (int i=0; i<n_orbits; i++){
		char* line = lines[i];
		printf("%s\n", line);
		char* orbitee_name = strtok(line, ")");
		char* orbiter_name = strtok(NULL, ")");
		printf("read line where %s orbits %s\n", orbiter_name, orbitee_name);	

		struct orbiter orbitee = get_orbiter(root, orbitee_name);
		struct orbiter orbiter = get_orbiter(root, orbiter_name);
		add_orbiter(orbitee, orbiter);
	}
	print_orbiter(&root);

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
	printf("inside comparator\n");
	struct orbiter* o1 = (struct orbiter*) orbiter1;
	struct orbiter* o2 = (struct orbiter*) orbiter2;
	printf("comparin %s", o1->name);
	printf(" with %s\n", o2->name); // kaatuu =(
	printf("%d\n", o2->name == NULL);
	printf("compare result %d\n", strcmp(o1->name, o2->name));
	return 0;
	//return strcmp(o1->name, o2->name);
}

/*
 * Get a new/existing node for the orbiter with the given name.
 */
struct orbiter get_orbiter(struct bstree_node root, char* name){
	struct orbiter new_orbiter = {name, NULL, NULL};
	struct bstree_node orbiter_node = search(&root, &new_orbiter, orbiter_compare);
	return (*(struct orbiter*) orbiter_node.content);
}

void add_orbiter(struct orbiter orbitee, struct orbiter new_orbiter){
	if (orbitee.orbitees == NULL) {
		struct llist_node new_list = {&new_orbiter, NULL};
		orbitee.orbitees = &new_list;
	} else {
		llist_append(*orbitee.orbitees, &new_orbiter);
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
