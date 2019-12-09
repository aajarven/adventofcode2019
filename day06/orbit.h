struct orbiter{
	char* name;
	struct orbiter* central;
	struct llist_node* orbiters;
};

int orbiter_compare(void* orbiter1, void* orbiter2);
struct orbiter* get_orbiter(struct bstree_node* root, char* name);
void add_orbiter(struct orbiter* orbitee, struct orbiter* new_orbiter);
void print_orbiter(struct bstree_node* orbiter_node);
void print_orbiters(struct orbiter* o);
struct orbiter* create_orbiter(char* name);
void free_orbiters(struct orbiter* central);
