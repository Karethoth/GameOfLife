#include "cellkernel.h"

#include <numeric>

CellKernel::CellKernel(CELL state_for_all) : cells{state_for_all}
{
}
CellKernel::CellKernel( std::array<CELL,9> initial_cells ) : cells(initial_cells)
{
}

void CellKernel::step_right()
{
    /*
     *  0 1 2    1 2 X
     *  3 4 5 -> 4 5 X
     *  6 7 8    7 8 X
     */

    cells[0] = cells[1];
    cells[1] = cells[2];
    cells[2] = DEAD;

    cells[3] = cells[4];
    cells[4] = cells[5];
    cells[5] = DEAD;

    cells[6] = cells[7];
    cells[7] = cells[8];
    cells[8] = DEAD;
}

CELL CellKernel::compute_state()
{
    /*
     * Calculate the total number of neighbour cells alive.
     *
     * To account for the current state of the grid cell ([4]),
     * start from -1 if the cell is alive at the moment.
     */
    const CELL current_state = cells[4];

    int initial_neighbours= current_state == ALIVE ? -1 : 0;
    const auto neighbours = std::accumulate(cells.begin(), cells.end(), initial_neighbours);

    if (current_state == ALIVE)
    {
        if(neighbours < 2)
        {
            return DEAD;
        }

        else if(neighbours > 3)
        {
            return DEAD;
        }

        return ALIVE;
    }
    else
    {
        if(neighbours == 3)
        {
            return ALIVE;
        }
        return DEAD;
    }
}
