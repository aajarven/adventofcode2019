void run_code(int* code, int input);
int handle_operation(int* code, int index, int input);
int param_count(int* code, int index);
void construct_params(int* code, int* params, int n_params, int index);
int perform_operation(int* code, int* params, int index, int input);
