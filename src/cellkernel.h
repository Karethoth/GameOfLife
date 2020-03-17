#ifndef CELLKERNEL_H
#define CELLKERNEL_H

enum CELL : unsigned char
{
    DEAD,
    ALIVE
};

struct CellKernel
{
    CELL cells[9]{DEAD};

    void step_right();
    CELL compute_state();
};

#endif // CELLKERNEL_H
