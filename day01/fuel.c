#include<stdio.h>
#include<stdlib.h>
#include"../utils/filereader.h"

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Input file missing. Give the input file as a "
				"command line argument.\n");
		exit(EXIT_FAILURE);
	}

	int len = count_lines(argv[1]);
	int total_fuel = 0;
	int *masses = malloc(len * sizeof(int));
	read_int_array(argv[1], len, masses);
	for(int i=0; i<len; i++){
		total_fuel += masses[i]/3 - 2;
	}
	printf("%d\n", total_fuel);
	exit(EXIT_SUCCESS);
}
