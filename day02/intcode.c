#include<stdio.h>
#include<stdlib.h>
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

	code[1] = 12;
	code[2] = 2;

	int index = 0;
	while(code[index] != 99){
		index = handle_operation(code, index);
	}
	
	printf("First value: %d\n", code[0]);
	exit(EXIT_SUCCESS);
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
