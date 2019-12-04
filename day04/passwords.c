#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../utils/filereader.h"
#include"passwords.h"

#define PASSLEN 6

int main(int argc, char **argv){
	char* usage = "Usage:\n    passwords inputfile.txt triplets.\n"
			   	  "Allowed values for triplets are 'true' (when triplets, quadruplets "
			      "etc. are allowed) and 'false' (when they are not).\n";
	if (argc < 3){
		fprintf(stderr, "Insufficient arguments.\n%s", usage);
		exit(EXIT_FAILURE);
	}

	char* triple_flag = argv[2];
	bool allow_triplets;
	if (strcmp(triple_flag, "true") == 0){
		allow_triplets = true;
	} else if (strcmp(triple_flag, "false") == 0){
		allow_triplets = false;
	} else {
		fprintf(stderr, "Invalid argument.\n%s", usage);
		exit(EXIT_FAILURE);
	}

	// initialize the limits
	int* limits = malloc(2 * sizeof(int));
	read_int_array(argv[1], 2, limits);
	int* password_candidate = malloc(PASSLEN * sizeof(int));
	int* password_max = malloc(PASSLEN * sizeof(int));
	set_password(password_candidate, limits[0]);
	set_password(password_max, limits[1]);
	
	// loop through the password interval and count valid passwords
	int password_count = 0;
	do {
		password_count += password_valid(password_candidate, allow_triplets);
	} while (increment_password(password_candidate, password_max));

	printf("%d valid passwords found.\n", password_count);

	free(password_candidate);
	free(password_max);	
	exit(EXIT_SUCCESS);
}

/*
 * Increment the value of the given password. Returns true if incrementing
 * succeeded, i.e. the password did not exceed the maximum value.
 */
bool increment_password(int* password, int* max){
	int index = PASSLEN - 1;
	while(index >= 0){
		if (increment_index(password, index)){
			index--;
		} else {
			return password_leq(password, max);
		}
	}

	return false;
}

/*
 * Check whether password represents a number that is less or equal than the
 * number represented by the given maximum.
 */
bool password_leq(int* password, int* max){
	for (int i=0; i<PASSLEN; i++){
		if (password[i] > max[i]) {
			return false;
		} else if (password[i] < max[i]) {
			return true;
		}
	}
	return true;
}

/*
 * Increment value in the given index of the password.
 *
 * Does not affect the values in other indices of the password, but does spin
 * the counter from 9 to 0. Return value tells whether this 9 -> 0 change has
 * occurred.
 */
bool increment_index(int* password, int index){
	if (password[index] < 9) {
		password[index]++;
		return false;
	} else {
		password[index] = 0;
		return true;
	}
}

/*
 * Returns true for valid passwords.
 */
bool password_valid(int* password, bool allow_triplets){
	return contains_double(password, allow_triplets) && no_decreasing(password);
}

/*
 * Return true for passwords that contain two adjacent numbers that are equal.
 */
bool contains_double(int* password, bool allow_triplets){
	int i=0;
	while(i<PASSLEN){
		i++;
		int previous = password[i-1];
		if (previous == password[i]) {
			if (allow_triplets) {
				return true;
			} else {
				if (i == (PASSLEN - 1)) {
					return true;
				} else if (password[i+1] != password[i]) {
					return true;
				} else {
					// skip forward until this block of equal numbers has been
					// passed: a password such as 111223 is ok and should not
					// be deemed invalid based on the first triplet.
					while (password[i] == previous && i < PASSLEN){
						i++;
					}
				}
			}
		}
	}
	return false;
}

/*
 * Return true for passwords that satisfy the condition "Going from left to
 * right, the digits never decrease" and false for others.
 */
bool no_decreasing(int* password){
	int previous = password[0];
	for (int i=1; i<PASSLEN; i++){
		if (previous > password[i]){
			return false;
		}
		previous = password[i];
	}
	return true;
}

/*
 * Set the given password array to contain the given int value (most
 * significant digit in the first index).
 */
void set_password(int* password, int value){
	for(int i=PASSLEN-1; i>=0; i--){
		password[i] = value % 10;
		value = value/10;
	}
}

void print_password(int* password){
	for(int i=0; i<PASSLEN; i++){
		printf("%d", password[i]);
	}
	printf("\n");
}
