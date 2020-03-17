#include "lifegrid.h"

#include <iostream>
#include <numeric>
#include <type_traits>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>


// Helper for the float to int casting
template <
    typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
int to_int(T f)
{
    return static_cast<int>(f);
}

LifeGrid::LifeGrid() :
    grid_width{10},
    grid_height{10},
    cells{10 * 10, DEAD}
{
    resize_grid(10, 10);
}

LifeGrid::~LifeGrid()
{
}

size_t LifeGrid::coord_to_index(int x, int y) const
{
    if(x >= grid_width)
    {
        x = grid_width - 1;
    }

    if(y >= grid_height)
    {
        y = grid_height - 1;
    }

    if(x < 0 || y < 0)
    {
        return 0;
    }

    return (static_cast<size_t>(grid_width) * static_cast<size_t>(y)) + static_cast<size_t>(x);
}


void LifeGrid::resize_grid(int new_width, int new_height)
{
    if(new_width <= 0)
    {
        new_width = 1;
    }
    if(new_height <= 0)
    {
        new_height = 1;
    }

    const size_t total_cells = static_cast<size_t>(new_width * new_height);
    const size_t total_cells_before = static_cast<size_t>(grid_width) * static_cast<size_t>(grid_height);
    if (total_cells < total_cells_before)
    {
        grid_width = new_width;
        grid_height = new_height;
        // TODO: Copy old contents into the new grid
        cells.resize(total_cells);
    }
    else
    {
        // TODO: Copy old contents into the new grid
        cells = std::vector<CELL>{total_cells, DEAD};
        grid_width = new_width;
        grid_height = new_height;
    }
}


void LifeGrid::set_cell(const int x, const int y, const CELL state)
{
    size_t index = coord_to_index(x, y);
    cells[index] = state;
}

void LifeGrid::set_next_generation_cell(const int x, const int y, const CELL state)
{
    size_t index = coord_to_index(x, y);
    cells_next_generation[index] = state;
}

CELL LifeGrid::get_cell(const int x, const int y) const
{
    // TODO: if outside of bounds, display error in status
    size_t index = coord_to_index(x, y);
    return cells[index];
}

/*
 * Creates the glider one cell away from the top left corner
 */
void LifeGrid::create_glider()
{
    // Don't even try if the grid is too small
    if (grid_width < 4 || grid_height < 4)
    {
        return;
    }

    /*
     *  X
     *   X
     * XXX
     */
    set_cell(2, 1, ALIVE);
    set_cell(3, 2, ALIVE);
    set_cell(1, 3, ALIVE);
    set_cell(2, 3, ALIVE);
    set_cell(3, 3, ALIVE);
}

void LifeGrid::next_generation()
{
    cells_next_generation.resize(cells.size());

    /* Kernel we will be updating as the grid is traversed through
     * 0 1 2
     * 3 4 5
     * 6 7 8
     */

    // TODO: Handle special cases for grid sizes smaller than 3x3

    int index = 0;
    for(int y=0; y < grid_height; y++)
    {
        CellKernel current_kernel{DEAD};
        // We know that the left column is dead, so there is no need to populate it.
        // (Unless we want to wrap the grid around.)

        // If we're not on the top-row, we can grab the cells from the row above
        if(y > 0)
        {
            current_kernel.cells[1] = get_cell(0, y-1);
            current_kernel.cells[2] = get_cell(1, y-1);
        }

        current_kernel.cells[4] = get_cell(0, y);
        current_kernel.cells[5] = get_cell(1, y);

        // If we're not on the bottom row, we can grab the cells from the row below
        if(y < grid_height - 1)
        {
            const auto cell1 = get_cell(0, y+1);
            const auto cell2 = get_cell(1, y+1);
            current_kernel.cells[7] = get_cell(0, y+1);
            current_kernel.cells[8] = get_cell(1, y+1);
        }

        for(int x=0; x < grid_width; x++)
        {
            // If this isn't the rightmost column of the grid fill
            // the rigt kernel column, pretty much the same as above
            if(x < grid_width - 1)
            {
                if(y > 0)
                {
                    current_kernel.cells[2] = get_cell(x+1, y-1);
                }

                current_kernel.cells[5] = get_cell(x+1, y);

                if(y < grid_height - 1)
                {
                    current_kernel.cells[8] = get_cell(x+1, y+1);
                }
            }

            // Update the next generation
            const auto new_state = current_kernel.compute_state();
            set_next_generation_cell(x, y, new_state);

            // Move kernel contents left by one
            current_kernel.step_right();

            index++;
        }
    }
    cells = cells_next_generation;
}
