#ifndef LIFEGRID_H
#define LIFEGRID_H

#include <QGraphicsScene>
#include <QPaintEvent>

class LifeGrid : public QGraphicsScene
{
  public:
    LifeGrid(QObject *_parent = nullptr);
    virtual ~LifeGrid() override;

    float zoom = 1.f;

    float offset_x = 0.f;
    float offset_y = 0.f;

    void drawForeground(QPainter *painter, const QRectF &rect) override;
};

#endif // LIFEGRID_H
