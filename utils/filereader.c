#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
 * Read integers separated with a separator char into the given array.
 */
void read_csv_ints(char* filename, int len, int* arr, char separator){
	FILE *fp = fopen(filename, "r");
	char format[] = "%d";
	strncat(format, &separator, 1);

	int i=0;
	while(i<len){
		fscanf(fp, format, &arr[i++]);
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
		fscanf(fp, "%*s\n");
		len++;
	}
	fclose(fp);
	return len;
}

/*
 * Return the number of integer fields separated with the given separator.
 */
int count_fields(char* filename, char separator){
	FILE *fp = fopen(filename, "r");
	char format[] = "%*d";
	strncat(format, &separator, 1);

	int len = 0;
	while(!feof(fp)){
		fscanf(fp, format);
		len++;
	}
	fclose(fp);
	return len;
}

/*
 * Read a the lines from a text file into the given 2D char array. Modifies the
 * array in place.
 */
void read_lines(char* filename, char* arr, int max_line_length, int max_lines){
	FILE *fp = fopen(filename, "r");
	int line = 0;
	while(fgets(&(arr[line * max_line_length]), max_line_length, fp) != NULL && ++line < max_lines){
	}
	fclose(fp);
}
