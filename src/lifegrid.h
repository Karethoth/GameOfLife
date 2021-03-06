#ifndef LIFEGRID_H
#define LIFEGRID_H

#include "cellkernel.h"

#include <vector>

/*!
 * \brief Class for basic management of the whole Game of Life grid
 * \details Handles the basic modifications of the grid
 */
class LifeGrid
{
  public:

    /*!
     * \brief Grid constructor
     * \details  Will create a size_n * size_n grid
     * \param size_n The length of a side for a square grid. Must be 3 or more
     */
    LifeGrid(int size_n=5);
    virtual ~LifeGrid();

    /*!
     * \brief Kills all cells in the grid
     */
    void clear_grid();

    /*!
     * \brief Resizes the grid
     * \param new_width The new width of the grid
     * \param new_height The new height of the grid
     */
    void resize_grid(int new_width, int new_height);

    /*!
     * \brief Creates the famous glider in the top-left corner
     */
    void create_glider();

    /*!
     * \brief Updates the whole grid into the next generation
     */
    void next_generation();

    /*!
     * \brief Sets the cell value accordingly
     * \param x The column to set. The 1st column is 0
     * \param y The row to set. The 1st row is 0
     * \param state The wanted state of the cell
     */
    void set_cell(const int x, const int y, const CELL state);

    /*!
     * \brief Fetch the state of a certain cell
     * \details Will wrap coordinates around as necessary to keep them in-bounds
     * \param x The wanted column. The 1st column is 0
     * \param y The wanted row. The 1st row is 0
     * \return
     */
    CELL get_cell(int x, int y) const;

    /*!
     * \brief Set grid wrap
     * \param wrap Should the grid wrap around itself?
     */
    void set_wrap_grid(bool wrap);

    /*!
     * \brief Grab grid width
     * \return Current grid width
     */
    int get_grid_width() const;

    /*!
     * \brief Grab grid height
     * \return Current grid height
     */
    int get_grid_height() const;

  protected:
    /*!
     * \brief Calculates the real index for the given coordinate
     * \param x The column. The 1st column is 0
     * \param y The row. The 1st column is 0
     * \return `size_t` The real cell index
     */
    size_t coord_to_index(int x, int y) const;

    /*!
     * \brief The width of the grid
     */
    int grid_width;

    /*!
     * \brief The height of the grid
     */
    int grid_height;

    /*!
     * \brief The current grid state
     */
    std::vector<CELL> cells;

  private:
    /*!
     * \brief The next state of the grid.
     * \details Kept here to prevent unnecessary memory allocation and deallocation.
     */
    std::vector<CELL> cells_next_generation;

    /*!
     * \brief Modify the cell in next generation
     * \details Should only be called by next_generation().
     * \param x The column. The 1st column is 0.
     * \param y The row. The 1st row is 0.
     * \param state The wanted state of the cell.
     */
    void set_next_generation_cell(const int x, const int y, const CELL state);

    /*!
     * \brief Should the grid wrap around itself?
     */
    bool wrap_grid;
};

#endif // LIFEGRID_H
