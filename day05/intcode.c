#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"intcode.h"
#include"../utils/filereader.h"

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Input file missing. Give the input file as a "
				"command line argument.\n");
		exit(EXIT_FAILURE);
	}

	int len = count_fields(argv[1], ',') - 1;
	int *code = malloc(len * sizeof(int));
	read_csv_ints(argv[1], len, code, ',');
	int *editable_code = malloc(len * sizeof(int));
	memcpy(editable_code, code, len * sizeof(int));

	run_code(code);

	free(code);
	exit(EXIT_FAILURE);
}


/*
 * Run the intcode in the given code array, editing it in place.
 */
void run_code(int* code){
	int index = 0;
	while(code[index] != 99){
		index = handle_operation(code, index);
	}
}

int handle_operation(int* code, int index){
	int opcode = code[index] % 10;
	int n_params;
	switch(code[index]){
		case 1:
		case 2:
			n_params = 2;
			break;
		case 3:
		case 4:
			n_params = 1;
			break;
		default:
			fprintf(stderr, "Illegal opcode %d\n", opcode);
			exit(EXIT_FAILURE);
	}

	int* parameter_modes = malloc(n_params * sizeof(int));
	int modes = code[index] / 100;
	for(int i=0; i<n_params; i++){
		parameter_modes[i] = modes % 10;
		modes /= 10;
	}

//	int operand1_index = code[index + 1];
//	int operand2_index = code[index + 2];
//	int result_index = code[index + 3];
//	switch(code[index]){
//		case 1:
//			code[result_index] = code[operand1_index] + code[operand2_index];
//			break;
//		case 2:
//			code[result_index] = code[operand1_index] * code[operand2_index];
//			break;
//	}
//	return index + 4;
}
