#define GRIDSIZE 2000

int get_grid_value(int grid[GRIDSIZE][GRIDSIZE], int x, int y);
void set_grid_value(int grid[GRIDSIZE][GRIDSIZE], int x, int y, int value);
void increment_grid_value(int grid[GRIDSIZE][GRIDSIZE], int x, int y);
int transform_to_grid(int coordinate);
