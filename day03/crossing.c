#include<limits.h>
#include<math.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include"crossing.h"
#include"../utils/filereader.h"

#define GRIDSIZE 15000
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
	
	// one of the wires calculated distance along the wire in positive numbers,
	// another one in negative ones
	int wire_increments[] = {1, -1};
	int closest_crossing_manhattan = INT_MAX;
	int closest_crossing_wiredist = INT_MAX;

	char direction;
	int length;
	// log the wires into the grid and do crossing checks
	for (int line=0; line<wire_count; line++) {
		// current position on the string representing the wire
		char* line_ptr = &lines[line][0];
		// cartesian coordinates of the end of the wire
		int current_x = 0;
		int current_y = 0;
		// distance from the centre along the wire
		int wire_increment = wire_increments[line];
		int current_wire_value = 0;

		// follow the wire
		while(true) {
			// read direction char and determine x and y movement
			int dx = 0;
			int dy = 0;
			sscanf(line_ptr, "%c", &direction);
			line_ptr++;
			switch (direction){
				case 'U':
					dy = -1;
					break;
				case 'D':
					dy = 1;
					break;
				case 'L':
					dx = -1;
					break;
				case 'R':
					dx = 1;
					break;
				default:
					fprintf(stderr, "Invalid direction char %c\n", direction);
					exit(EXIT_FAILURE);
			}
			
			// read movement distance
			sscanf(line_ptr, "%d,", &length);
			line_ptr += (int) floor(log10((double) length)) + 1;

			// log the movement in the grid and check crossings
			for(int i=0; i < length; i++){
				// advance along the wire
				current_x += dx;
				current_y += dy;
				current_wire_value += wire_increment;

				// check for collisions
				int grid_value = get_grid_value(grid, current_x, current_y);
				if (wire_signs_differ(current_wire_value, grid_value)) {
					// Manhattan checks
					int current_manhattan_distance = manhattan_from_centre(current_x, current_y);
					if (current_manhattan_distance < closest_crossing_manhattan){
						closest_crossing_manhattan = current_manhattan_distance;
					}

					// wire distance
					int current_wire_distance = abs(grid_value - current_wire_value);
					if (current_wire_distance < closest_crossing_wiredist){
						closest_crossing_wiredist = current_wire_distance;
					}
				} else {
					// if no collision, just set the wire value
					set_grid_value(grid, current_x, current_y, current_wire_value);
				}
			}

			if (*line_ptr == '\0' || *line_ptr == '\n') {
				break;
			} else {
				line_ptr++;
			}
		}
	}

	printf("Closest manhattan crossing: %d\n", closest_crossing_manhattan);
	printf("Closest wiredistance crossing: %d\n", closest_crossing_wiredist);

	free(grid);
	exit(EXIT_SUCCESS);
}


/*
 * The grid is handled in coordinates where the point (0, 0) is at the centre
 * (rounded down when needed). This function returns the value in the given
 * position.
 */
int get_grid_value(int* grid, int x, int y){
	return grid[transform_to_grid(y) * GRIDSIZE + transform_to_grid(x)];
}

/*
 * Set the value in the cell (x, y) of the grid,
 */
void set_grid_value(int* grid, int x, int y, int value){
	grid[transform_to_grid(y) * GRIDSIZE + transform_to_grid(x)] = value;
}

/*
 * Transform the given cartesian coordinate to the corresponding coordinate on
 * the grid. The grid is square, so the transformation is the same for x and y
 * coordinates.
 */ 
int transform_to_grid(int coordinate){
	return coordinate + GRIDCENTER;
}

/*
 * Calculate the manhattan distance of the given coordinates from the centre of
 * the grid.
 */
int manhattan_from_centre(int x, int y){
	return abs(x) + abs(y);
}

/*
 * Check whether wire1 and wire2 cross based on the wire distances (one wire
 * has negative numbers and the other has positive).
 */
bool wire_signs_differ(int wire1, int wire2){
	return (wire1 < 0 && wire2 > 0) || (wire1 > 0 && wire2 < 0);
}
