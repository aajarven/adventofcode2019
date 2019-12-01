#include<stdio.h>
#include<stdlib.h>
#include"fuel.h"
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
		total_fuel += fuel(masses[i]);
	}
	printf("Fuel for modules: %d\n", total_fuel);

	total_fuel = 0;
	for(int i=0; i<len; i++){
		int module_fuel = fuel(masses[i]);
		int total_metafuel = 0;
		int new_metafuel = fuel(module_fuel);
		while(new_metafuel > 0){
			total_metafuel += new_metafuel;
			new_metafuel = fuel(new_metafuel);
		}
		total_fuel += module_fuel + total_metafuel;
	}
	printf("Fuel for modules and fuel: %d\n", total_fuel);
	exit(EXIT_SUCCESS);
}

int fuel(int mass){
	return mass/3 - 2;
}
