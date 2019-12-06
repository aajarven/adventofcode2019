int orbiter_compare(void* orbiter1, void* orbiter2);
struct orbiter get_orbiter(struct bstree_node root, char* name);
void add_orbiter(struct orbiter orbitee, struct orbiter new_orbiter);
void print_orbiter(struct bstree_node* orbiter_node);
