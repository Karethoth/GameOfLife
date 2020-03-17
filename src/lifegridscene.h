#ifndef LIFEGRIDSCENE_H
#define LIFEGRIDSCENE_H

#include "cellkernel.h"
#include "lifegrid.h"

#include <QGraphicsScene>
#include <QPaintEvent>

#include <memory>
#include <vector>


enum PaintMode : unsigned char
{
    MAKE_DEAD,
    MAKE_ALIVE
};

class LifeGridScene : public LifeGrid, public QGraphicsScene
{
  public:
    explicit LifeGridScene(QObject *_parent = nullptr);
    virtual ~LifeGridScene() override;

    int zoom;
    float offset_x;
    float offset_y;

    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

  private:
    void draw_grid(QPainter *painter) const;
    QPoint scene_pos_to_grid_pos(const QPointF &scene_pos) const;

    PaintMode paint_mode;
    bool is_painting_cells;
    bool is_dragging_view;
};

#endif // LIFEGRIDSCENE_H
