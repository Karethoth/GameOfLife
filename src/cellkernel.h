#ifndef CELLKERNEL_H
#define CELLKERNEL_H

#include <array>

/*!
 * @brief The state of the cell, either DEAD or ALIVE
 */
enum CELL : unsigned char
{
    DEAD,
    ALIVE
};

/*!
 *  \brief 3x3 matrix. Encapsulates current cell and all of its neighbours.
 *  \details Provides common methods step_right() and compute_state()
 */
struct CellKernel
{
    std::array<CELL,9> cells;

    /*!
     * \brief The default constructor.
     * \param state_for_all The initial state for all cells, defaults to DEAD.
     */
    CellKernel(CELL state_for_all=DEAD);

    /*!
     * \brief Construct kernel with a std array
     * \param initial_cells Makes passing an initializer list as the argument easier.
     *        The cells will be initialized accordingly.
     */
    CellKernel(std::array<CELL,9> initial_cells);

    /*!
     * \brief Moves all columns left by one.
     * \details The rightmost column cells will be set as DEAD.
     * <pre>
     * 0 1 2    1 2 X
     * 3 4 5 -> 4 5 X
     * 6 7 8    7 8 X
     * </pre>
     */
    void step_right();

    /*!
     * \brief Computes the new state for a cell.
     * \details This is the core of the game logic. It implements the core rules
     *          regarding whether the cell should be alive or dead.
     * \return The new cell state, based on the state of the kernel.
     */
    CELL compute_state();
};

#endif // CELLKERNEL_H
