#include "lifegrid.h"

#include <QPainter>

LifeGrid::LifeGrid(QObject *_parent) : QGraphicsScene(_parent)
{
}

LifeGrid::~LifeGrid()
{
}

void LifeGrid::drawForeground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(Qt::black));
    painter->drawLine(-100,-100,100,100);
}
