#include<stdio.h>
#include<stdlib.h>
#include"filereader.h"

/*
 * Reads all numbers from a file and returns them in an array.
 */
void read_int_array(char* filename, int len, int* arr){
	FILE *fp = fopen(filename, "r");

	int i=0;
	while(i < len){
		fscanf(fp, "%d", &arr[i++]);
	}
	fclose(fp);
}

/*
 * Return the number of lines in a file
 */
int count_lines(char* filename){
	FILE *fp = fopen(filename, "r");
	int len = 0;
	while(!feof(fp)){
		fscanf(fp, "%*d\n");
		len++;
	}
	fclose(fp);
	return len;
}
