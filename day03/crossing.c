#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"crossing.h"
#include"../utils/filereader.h"

#define GRIDCENTER (GRIDSIZE / 2)
#define MAX_STR_LEN 1500

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Input file missing. Give the input file as a "
				"command line argument.\n");
		exit(EXIT_FAILURE);
	}
	
	// Initialize an empty grid
	int* grid = malloc(GRIDSIZE * GRIDSIZE * sizeof(int));
	for (int i=0; i<GRIDSIZE; i++){
		for (int j=0; j<GRIDSIZE; j++){
			grid[i * GRIDSIZE + j] = 0;
		}
	}

	int wire_count = 2;
	char lines[2][MAX_STR_LEN];
	read_lines(argv[1], lines[0], MAX_STR_LEN, wire_count);
	
	char direction;
	int length;
	char* line_ptr = &lines[0][0];
//	int current_x = 0;
//	int current_y = 0;
	printf("rivi: %s\n", line_ptr);
	printf("whiling\n");
	while(sscanf(line_ptr, "%c", &direction)){
		printf("read direction %c\n", direction);
		line_ptr++;
		sscanf(line_ptr, "%d,", &length);
		printf("read length %d\n", length);
		int numlength = ceil(log10((double) length));
		printf("moving the pointer forward %d\n", numlength);
		line_ptr += numlength + 1;
		int dx = 0;
		int dy = 0;
		switch (direction){
			case 'U':
				dy = 1;
				break;
			case 'D':
				dy = -1;
				break;
			case 'L':
				dx = -1;
				break;
			case 'R':
				dx = 1;
				break;
			default:
				fprintf(stderr, "Invalid direction char %c\n%d", direction, dx+dy);
				exit(EXIT_FAILURE);
		}
//		printf("%c%d\n", direction, length);
	}
}


/*
 * The grid is handled in coordinates where the point (0, 0) is at the centre
 * (rounded down when needed). This function returns the value in the given
 * position.
 */
int get_grid_value(int grid[GRIDSIZE][GRIDSIZE], int x, int y){
	return grid[transform_to_grid(y)][transform_to_grid(x)];
}

/*
 * Set the value in the cell (x, y) of the grid,
 */
void set_grid_value(int grid[GRIDSIZE][GRIDSIZE], int x, int y, int value){
	grid[transform_to_grid(y)][transform_to_grid(x)] = value;
}

/*
 * Increment the value in the cell (x, y) of the grid.
 */
void increment_grid_value(int grid[GRIDSIZE][GRIDSIZE], int x, int y){
	grid[transform_to_grid(y)][transform_to_grid(x)]++;
}

/*
 * Transform the given cartesian coordinate to the corresponding coordinate on
 * the grid. The grid is square, so the transformation is the same for x and y
 * coordinates.
 */ 
int transform_to_grid(int coordinate){
	return coordinate + GRIDCENTER;
}
