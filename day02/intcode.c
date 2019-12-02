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

	// 1202 case
	editable_code[1] = 12;
	editable_code[2] = 2;
	run_code(editable_code);
	printf("First value for 1202: %d\n", editable_code[0]);

	// Find 19690720
	for(int noun=0; noun<=99; noun++){
		for(int verb=0; verb<=99; verb++){
			memcpy(editable_code, code, len * sizeof(int));
			editable_code[1] = noun;
			editable_code[2] = verb;
			run_code(editable_code);

			if (editable_code[0] == 19690720){
				printf("100 * noun + verb: %d\n", 100 * noun + verb);
				free(code);
				free(editable_code);
				exit(EXIT_SUCCESS);
			}
		}
	}

	printf("Suitable verb and noun not found.\n");
	free(code);
	free(editable_code);
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
	int operand1_index = code[index + 1];
	int operand2_index = code[index + 2];
	int result_index = code[index + 3];
	switch(code[index]){
		case 1:
			code[result_index] = code[operand1_index] + code[operand2_index];
			break;
		case 2:
			code[result_index] = code[operand1_index] * code[operand2_index];
			break;
	}
	return index + 4;
}
