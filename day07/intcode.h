void run_code(int* code, int input[2], int* output);
int handle_operation(int* code, int index, int input[2], int* input_index, int* output);
int param_count(int* code, int index);
void construct_params(int* code, int* params, int n_params, int index);
int perform_operation(int* code, int* params, int index, int input[2], int* input_index, int* output);
