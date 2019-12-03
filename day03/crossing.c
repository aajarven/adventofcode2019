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
	
	int closest_crossing_x = 0;
	int closest_crossing_y = 0;
	int closest_crossing_distance = GRIDSIZE * 2;

	char direction;
	int length;
	for (int line=0; line<wire_count; line++) {
		char* line_ptr = &lines[line][0];
		int current_x = 0;
		int current_y = 0;

		int wire = line + 1; // avoid having zeroeth wire == no wire
		while(true) {
			sscanf(line_ptr, "%c", &direction);
			line_ptr++;
			
			sscanf(line_ptr, "%d,", &length);
			line_ptr += (int) floor(log10((double) length)) + 1;

			int dx = 0;
			int dy = 0;
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

			for(int i=0; i < length; i++){
				// advance along the wire
				current_x += dx;
				current_y += dy;

				// check for collisions
				int current_value = get_grid_value(grid, current_x, current_y);
				if (current_value && current_value != wire){
					int current_centre_distance = manhattan_from_centre(current_x, current_y);

					// update the best found if necessary
					if (current_centre_distance < closest_crossing_distance){
						closest_crossing_x = current_x;
						closest_crossing_y = current_y;
						closest_crossing_distance = current_centre_distance;
					}
				} else {
					// if no collision, just set the wire value
					set_grid_value(grid, current_x, current_y, wire);
				}
			}

			if (*line_ptr == '\0' || *line_ptr == '\n') {
				break;
			} else {
				line_ptr++;
			}
		}
	}
	
	printf("Closest crossing: (%d, %d): %d\n", closest_crossing_x, closest_crossing_y, closest_crossing_distance);
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
