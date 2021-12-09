#ifndef CELLS_H_INCLUDED
#define CELLS_H_INCLUDED

// The simulation contains a grid of cells, 
// each of which are between "alive" and "dead", like in Conway's Game Of Life.
// The simulation runs in steps, in which each cell value is recalculated according to a formula.
// This formula only considers the values of the adjacent cells.
// According to how the parameters below are configurated,
// interesting visual patters arise.

// value between 0.0f and 1.0f indicating the "life" of a cell.
// a value of 0.0f is rendered black, a value of 1.0f is rendered white.
typedef float CellLive;

// the unique index of a cell within a simulation - no two cells have the same index.
typedef long unsigned CellIndex;

// total amount of cells in the simulation
extern int cellCount;
// width and height of the cell grid
extern int cellGridW, cellGridH;

// two-dimensional grid of all cell values.
// this is a pointer to an array of length == cellCount == cellGridW * cellGridH
// to access the value of a cell with coordinates (x,y) use cell_index(x, y)
extern CellLive *cellLife;

// parameters of step formula
// in each step for each cell the formula first calculates the average value of all adjacent cells

// the life of a cell depends on the average value of its adjacent cells.
// this parameter sets the ideal average value a cell can have
extern float cell_bestAvg;

// the maximum difference of the current average value to cell_bestAvg that a cell can tolerate.
// if the difference between the current average and cell_bestAvg is two large,
// the cell will lose life. otherwise the cell gains life.
extern float cell_best_range;

// the amount of life a cell loses multiplied with
//     the difference between its current neighbor average and the best neighbor average
// can be negative
extern float cellDieSpeed;

// if any cell's value is below cell_minLife, its value will be set to 0
extern float cell_minLife;

// amount of life given to each cell in each step disregarding the average value of adjacent cells
extern float cellLifeBoost;

// if set to 0, diagonal neighbors do not influence each other. otherwise they do
extern int cell_diagNeighbors;

// given the coordinates of a cell, calculates its index
static inline CellIndex cell_index(int x, int y)
{
    if (x < 0 || x >= cellGridW || y < 0 || y >= cellGridH)
        return -1;
	return x * cellGridH + y;
}

// allocates and initializes memory for the cell grid
void cells_alloc(int a_cellGridW, int a_cellGridH);

// resizes the cell grid and reallocates its memory
void cells_realloc(int a_cellGridW, int a_cellGridH);

// deallocates the memory of the cell grid
void cells_dealloc(void);

// sets all cell values to 0. may be called multiple times unlike cells_dealloc()
void cells_clean(void);

// set each cell in a region of the grid to a random life value
// if full is zero, this region is chosen randomly
// if full is non-zero, this region is the whole grid
void cells_gen(unsigned seed, int full);

// set parameters of the simulation
void cells_logic_set(float a_bestAvg, float a_best_range, float a_cellDieSpeed, float a_minLife, float a_cellLifeBoost, int a_diagNeighbors);

// randomize parameters of the simulation
void cells_logic_rand(unsigned seed);

// save parameters to given filename
void cells_logic_save(char const *filename);

// load parameters from given filename
void cells_logic_load(char const *filename);

// run one step of the simulation and calculate the values of each cell in the grid
void cells_logic(void);

#endif
