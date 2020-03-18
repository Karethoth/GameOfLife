#include "lifegrid.h"

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

LifeGrid::LifeGrid(int size_n) :
    grid_width{size_n},
    grid_height{size_n},
    wrap_grid{false}
{
    cells = std::vector<CELL>(size_n*size_n, DEAD);
    resize_grid(size_n, size_n);
}

LifeGrid::~LifeGrid()
{
}

void LifeGrid::clear_grid()
{
    cells = std::vector<CELL>(grid_width * grid_height, DEAD);
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

/*!
 * \details If any axis is out of bounds, the function
 *          picks the cell on the side opposite of it.
 */
CELL LifeGrid::get_cell(int x, int y) const
{
    if(x < 0)
    {
        x = grid_width - 1;
    }
    else if(x >= grid_width)
    {
        x = 0;
    }

    if(y < 0)
    {
        y = grid_height -1;
    }
    else if(y >= grid_height)
    {
        y = 0;
    }

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
    /*
     * A quadtree would be nice for the larger grids.
     */

    // Make sure there's enough space
    cells_next_generation.resize(cells.size());

    for(int y=0; y < grid_height; y++)
    {
        /* Kernel we will be updating as the grid is traversed through
         * 0 1 2
         * 3 4 5
         * 6 7 8
         */
        CellKernel current_kernel{DEAD};

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
            current_kernel.cells[7] = get_cell(0, y+1);
            current_kernel.cells[8] = get_cell(1, y+1);
        }

        for(int x=0; x < grid_width; x++)
        {
            /*
             *  When grid wrapping is enabled we need to initialize more of
             * the kernel cells when next to the grid borders.
             *
             * get_cell() will help us here, as it wraps the out-of-bound coordinates around.
             */
            if(wrap_grid)
            {
                if(y == 0)
                {
                    // This is the top row
                    current_kernel.cells[0] = get_cell(x-1, y-1);
                    current_kernel.cells[1] = get_cell(x,   y-1);
                    current_kernel.cells[2] = get_cell(x+1, y-1);
                }
                else if(y == grid_height - 1)
                {
                    // This is the bottom row
                    current_kernel.cells[6] = get_cell(x-1, 0);
                    current_kernel.cells[7] = get_cell(x,   0);
                    current_kernel.cells[8] = get_cell(x+1, 0);
                }

                if(x == 0)
                {
                    // This is the first column
                    current_kernel.cells[3] = get_cell(x-1, y);
                    current_kernel.cells[0] = get_cell(x-1, y-1);
                    current_kernel.cells[6] = get_cell(x-1, y+1);
                }

                else if(x == grid_width-1)
                {
                    // This is the last column
                    current_kernel.cells[5] = get_cell(0, y);
                    current_kernel.cells[2] = get_cell(0, y-1);
                    current_kernel.cells[8] = get_cell(0, y+1);
                }
            }

            // Back to the usual rules, which are not affected whether or not the grid wraps

            // If this isn't the rightmost column of the grid, fill
            // the right kernel column with whatever data is available
            if(x < grid_width - 1)
            {
                if(y > 0)
                {
                    // Not the bottom row, so we can grab a cell from there
                    current_kernel.cells[2] = get_cell(x+1, y-1);
                }

                current_kernel.cells[5] = get_cell(x+1, y);

                if(y < grid_height - 1)
                {
                    // Not the top row, so we can grab a cell from there
                    current_kernel.cells[8] = get_cell(x+1, y+1);
                }
            }

            // Update the next generation
            const auto new_state = current_kernel.compute_state();
            set_next_generation_cell(x, y, new_state);

            // Move kernel contents left by one
            current_kernel.step_right();
        }
    }
    cells = cells_next_generation;
}

void LifeGrid::set_wrap_grid(bool wrap)
{
    wrap_grid = wrap;
}

int LifeGrid::get_grid_width() const
{
    return grid_width;
}

int LifeGrid::get_grid_height() const
{
    return grid_height;
}
