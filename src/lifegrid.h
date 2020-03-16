#ifndef LIFEGRID_H
#define LIFEGRID_H

#include <QGraphicsScene>
#include <QPaintEvent>

#include <memory>
#include <vector>

enum CELL : unsigned char {
    DEAD,
    ALIVE
};

using CellKernel = CELL[9];


class LifeGrid : public QGraphicsScene
{
  public:
    explicit LifeGrid(QObject *_parent = nullptr);
    virtual ~LifeGrid() override;

    int zoom;

    float offset_x;
    float offset_y;

    int grid_width;
    int grid_height;

    void drawForeground(QPainter *painter, const QRectF &rect) override;

    void resize_grid(int new_width, int new_height);
    void create_glider();

  private:
    void draw_grid(QPainter *painter) const;
    size_t coord_to_index(int x, int y);
    void set_cell(const int x, const int y, const CELL state);
    CELL get_cell(const int x, const int y);

    std::vector<CELL> cells;
};

#endif // LIFEGRID_H
