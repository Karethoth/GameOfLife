#ifndef CELLKERNEL_H
#define CELLKERNEL_H

#include <array>

enum CELL : unsigned char
{
    DEAD,
    ALIVE
};

struct CellKernel
{
    std::array<CELL,9> cells{DEAD};

    CellKernel(CELL state_for_all=DEAD);
    CellKernel(std::array<CELL,9> initial_cells);

    void step_right();
    CELL compute_state();
};

#endif // CELLKERNEL_H
