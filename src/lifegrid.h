#ifndef LIFEGRID_H
#define LIFEGRID_H

#include "cellkernel.h"

#include <vector>


class LifeGrid
{
  public:
    LifeGrid();
    virtual ~LifeGrid();

    int grid_width;
    int grid_height;

    void resize_grid(int new_width, int new_height);
    void create_glider();
    void next_generation();

  protected:
    void set_cell(const int x, const int y, const CELL state);
    CELL get_cell(const int x, const int y) const;
    size_t coord_to_index(int x, int y) const;

    std::vector<CELL> cells;

  private:
    std::vector<CELL> cells_next_generation;
    void set_next_generation_cell(const int x, const int y, const CELL state);
};

#endif // LIFEGRID_H
