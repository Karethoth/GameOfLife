#ifndef LIFEGRID_H
#define LIFEGRID_H

#include "cellkernel.h"

#include <vector>

/*!
 * \brief Class for basic management of the whole GoL grid
 * \details Handles the basic modification of the grid
 */
class LifeGrid
{
  public:

    /*!
     * \brief Grid constructor
     * \param size_n Square grid size. Will create size_n * size_n grid.
     */
    LifeGrid(int size_n=5);
    virtual ~LifeGrid();

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
     * \param x The wanted column. The 1st column is 0
     * \param y The wanted row. The 1st row is 0
     * \return
     */
    CELL get_cell(const int x, const int y) const;

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
};

#endif // LIFEGRID_H
