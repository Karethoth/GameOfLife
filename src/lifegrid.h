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

enum PaintMode : unsigned char {
    MAKE_DEAD,
    MAKE_ALIVE
};

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
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    void resize_grid(int new_width, int new_height);
    void create_glider();

  private:
    void draw_grid(QPainter *painter) const;
    size_t coord_to_index(int x, int y) const;
    QPoint scene_pos_to_grid_pos(const QPointF &scene_pos) const;

    void set_cell(const int x, const int y, const CELL state);
    CELL get_cell(const int x, const int y) const;

    std::vector<CELL> cells;
    PaintMode paint_mode;
    bool is_dragging_view;
};

#endif // LIFEGRID_H
