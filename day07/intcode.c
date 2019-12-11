#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"intcode.h"
#include"../utils/filereader.h"

#define DEBUG 0
#define PHASE_ORDERS (5*4*3*2*1)
#define PHASE_LEN 5

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Input file or missing.\n"
				"Usage:\n intcode inputfile.txt\n");
		exit(EXIT_FAILURE);
	}

	int len = count_fields(argv[1], ',') - 1;
	int *code = malloc(len * sizeof(int));
	read_csv_ints(argv[1], len, code, ',');

	int* phase_list = all_phases(); 	
	if (DEBUG) {
		for(int line=0; line<PHASE_ORDERS; line++){
			for(int pos=0; pos<PHASE_LEN; pos++){
				printf("%d ", phase_list[line*PHASE_LEN + pos]);
			}
			printf("\n");
		}
	}

	int best_output = INT_MIN;
	int* output = malloc(sizeof(int));
	int *editable_code = malloc(len * sizeof(int));
	for (int phase_order_index=0;
			phase_order_index<PHASE_ORDERS;
			phase_order_index++){
		*output = 0;
		for (int amplifier=0; amplifier<PHASE_LEN; amplifier++){
			memcpy(editable_code, code, len * sizeof(int));
			int input[2] = {phase_list[phase_order_index*PHASE_LEN + amplifier], *output};
			run_code(code, input, output);
		}
		if (*output > best_output){
			best_output = *output;
		}
	}

	printf("Best possible thruster signal: %d\n", best_output);

	free(code);
	free(output);
	free(editable_code);
	exit(EXIT_SUCCESS);
}


/*
 * Run the intcode in the given code array, editing it in place. Output is
 * written to the location given by the output pointer. The input is a two
 * member array of integers containing both inputs for the program.
 */
void run_code(int* code, int input[2], int* output){
	int index = 0;
	int input_index = 0;
	while(code[index] != 99){
		index = handle_operation(code, index, input, &input_index, output);
	}
}

/*
 * Perform a single operation determined by opcode at index. If output is
 * produced, it is written to the location given by the output pointer.
 */
int handle_operation(int* code, int index, int input[2], int* input_index, int* output){
	int n_params = param_count(code, index);
	int* params = malloc(n_params * sizeof(int));
	construct_params(code, params, n_params, index);
	int new_pointer = perform_operation(code, params, index, input, input_index, output);

	free(params);
	if (new_pointer >= 0) {
		return new_pointer;
	} else {
		return index + n_params + 1;
	}
}

/*
 * Return the number of parameters used by the operation at the given index.
 */
int param_count(int* code, int index){
	int opcode = code[index] % 10;
	if (DEBUG){
		printf("full opcode: %d\n", code[index]);
	}
	switch(opcode){
		case 1:
		case 2:
			return 3;
		case 3:
		case 4:
			return 1;
		case 5:
		case 6:
			return 2;
		case 7:
		case 8:
			return 3;
		default:
			fprintf(stderr, "Illegal opcode %d\n", opcode);
			exit(EXIT_FAILURE);
	}
}

/*
 * Fill the given params array with the parameters (either positions or values
 * depending on the parameter modes) for the opcode at the given index.
 */
void construct_params(int* code, int* params, int n_params, int index){
	int* parameter_modes = malloc(n_params * sizeof(int));
	int opcode = code[index] % 10;
	int modes = code[index] / 100;
	if (DEBUG){
		printf("parameter modes:");
	}
	for(int i=0; i<n_params; i++){
		parameter_modes[i] = modes % 10;
		if (DEBUG){
			printf(" %d", parameter_modes[i]);
		}
		modes /= 10;
	}
	if (DEBUG){
		printf("\n");
	}

	for (int i=0; i<n_params; i++){
		if (i == n_params - 1 && opcode != 4 && opcode != 5 && opcode != 6) {
			params[i] = code[index + i + 1]; // output parameter
			if (DEBUG) {
				printf("output goes to index %d\n", params[i]);
			}
		} else if (parameter_modes[i] == 0){
			params[i] = code[code[index + i + 1]];
			if (DEBUG){
				printf("parameter %d fetched from index %d.\n", params[i], code[index + i + 1]);
			}
		} else if (parameter_modes[i] == 1){
			params[i] = code[index + i + 1];
			if (DEBUG){
				printf("literal parameter %d got.\n", params[i]);
			}
		} else {
			fprintf(stderr, "Illegal parameter mode %d encountered.\n", parameter_modes[i]);
			exit(EXIT_FAILURE);
		}
	}
	free(parameter_modes);
}

/*
 * Edit the code array / print output according to the operation at index. If
 * instruction pointer is to be moved, returns the new position, else returns
 * -1. The output is written to the location given by the output pointer.
 */
int perform_operation(int* code, int* params, int index, int input[2], int* input_index, int* output){
	switch(code[index] % 10){
		case 1:
			code[params[2]] = params[0] + params[1];
			if (DEBUG){
				printf("writing %d to index %d\n", code[params[2]], params[2]);
			}
			return -1;
		case 2:
			code[params[2]] = params[0] * params[1];
			if (DEBUG){
				printf("writing %d to index %d\n", code[params[2]], params[2]);
			}
			return -1;
		case 3:
			code[params[0]] = input[*input_index];
			if (DEBUG){
				printf("wrote input %d to index %d.\n", input[*input_index], params[0]);
			}
			*input_index = *input_index + 1;
			return -1;
		case 4:
			*output = params[0];
			return -1;
		case 5:
			if (params[0]){
				return params[1];
			} else {
				return -1;
			}
		case 6:
			if (!params[0]){
				return params[1];
			} else {
				return -1;
			}
		case 7:
			if (params[0] < params[1]) {
				code[params[2]] = 1;
			} else {
				code[params[2]] = 0;
			}
			return -1;
		case 8:
			if (params[0] == params[1]) {
				code[params[2]] = 1;
			} else {
				code[params[2]] = 0;
			}
			return -1;
		default:
			fprintf(stderr, "Illegal opcode %d encountered.\n", code[index]);
			exit(EXIT_FAILURE);
	}
}

int* all_phases(){
	int* phases = malloc(PHASE_ORDERS * PHASE_LEN * sizeof(int));
	// the first permutation
	for (int i=0; i<PHASE_LEN; i++){
		phases[i] = i;
	}
	int output_index = 0;
	save_permutations(phases, 0, PHASE_LEN, &output_index);

//	int phase_index = 1;
//	while(phase_index < PHASE_ORDERS * PHASE_LEN) {
//		memcpy(&phases[phase_index * PHASE_LEN],
//				&phases[(phase_index-1)*PHASE_LEN],
//				PHASE_LEN*sizeof(int));
//		
//	}
	return phases;
}

void save_permutations(int* source, int start_index, int length, int* output){
	if (start_index == length-1){  // the permutation is ready, save it
		for (int i=0; i<length; i++){
			source[*output + i] = source[i];
		}
		*output += length;
	} else { // not ready, add a digit
		for(int i=start_index; i<length; i++){
			swap(source, start_index, i);
			save_permutations(source, start_index+1, length, output);			
			swap(source, start_index, i); // undo the swapping to avoid changing the first element
		}
	}
}

/*
 * Swaps the contents in indices i1 and i2 of the given array.
 */
void swap(int* arr, int i1, int i2){
	int tmp = arr[i1];
	arr[i1] = arr[i2];
	arr[i2] = tmp;
}
