#include "lifegrid.h"

#include <iostream>
#include <type_traits>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>


// Helper for the float to int casting
template <
    typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
int to_int(T f)
{
    return static_cast<int>(f);
}


LifeGrid::LifeGrid(QObject *_parent) :
    QGraphicsScene(_parent),
    zoom{20},
    offset_x{0.f},
    offset_y{0.f},
    grid_width{10},
    grid_height{10},
    cells{10 * 10, DEAD},
    paint_mode{MAKE_ALIVE},
    is_dragging_view{false}
{
    resize_grid(5, 5);
    create_glider();
}

LifeGrid::~LifeGrid()
{
}

size_t LifeGrid::coord_to_index(int x, int y) const
{
    if(x >= grid_width) {
        x = grid_width - 1;
    }

    if(y >= grid_height) {
        y = grid_height - 1;
    }

    if(x < 0 || y < 0) {
        return 0;
    }

    return (static_cast<size_t>(grid_width) * static_cast<size_t>(y)) + static_cast<size_t>(x);
}


void LifeGrid::resize_grid(int new_width, int new_height)
{
    if(new_width <= 0) {
        new_width = 1;
    }
    if(new_height <= 0) {
        new_height = 1;
    }

    const size_t total_cells = static_cast<size_t>(new_width * new_height);
    const size_t total_cells_before = static_cast<size_t>(grid_width) * static_cast<size_t>(grid_height);
    if (total_cells < total_cells_before)
    {
        grid_width = new_width;
        grid_height = new_height;
        // TODO: Copy old contents into the new grid
        cells.resize(total_cells);
    }
    else
    {
        // TODO: Copy old contents into the new grid
        cells = std::vector<CELL>{total_cells, DEAD};
        grid_width = new_width;
        grid_height = new_height;
    }
}


void LifeGrid::set_cell(const int x, const int y, const CELL state)
{
    size_t index = coord_to_index(x, y);
    cells[index] = state;
}

CELL LifeGrid::get_cell(const int x, const int y) const
{
    // TODO: if outside of bounds, display error in status
    size_t index = coord_to_index(x, y);
    return cells[index];
}

QPoint LifeGrid::scene_pos_to_grid_pos(const QPointF &scene_pos) const
{
    const float grid_total_width = this->grid_width * this->zoom;
    const float grid_total_height = this->grid_height * this->zoom;

    const float min_x = 0.f - grid_total_width  / 2.f + offset_x;
    const float min_y = 0.f - grid_total_height / 2.f + offset_y;

    const float max_x = grid_total_width / 2.f + offset_x;
    const float max_y = grid_total_height / 2.f + offset_y;

    const float cell_width  = (max_x - min_x) / grid_width;
    const float cell_height = (max_y - min_y) / grid_height;

    const float scene_x = static_cast<float>(scene_pos.x());
    const float scene_y = static_cast<float>(scene_pos.y());

    float grid_x = (scene_x - min_x) / cell_width;
    float grid_y = (scene_y - min_y) / cell_height;

    if (grid_x < 0.f)
    {
        grid_x += -1.f;
    }
    if (grid_y < 0.f)
    {
        grid_y += -1.f;
    }

    return QPoint(to_int(grid_x), to_int(grid_y));
}

/*
 * Creates the glider one cell away from the top left corner
 */
void LifeGrid::create_glider()
{
    // Don't even try if the grid is too small
    if (grid_width < 4 || grid_height < 4)
    {
        return;
    }

    /*
     *  X
     *   X
     * XXX
     */
    set_cell(2, 1, ALIVE);
    set_cell(3, 2, ALIVE);
    set_cell(1, 3, ALIVE);
    set_cell(2, 3, ALIVE);
    set_cell(3, 3, ALIVE);
}

void LifeGrid::drawForeground(QPainter *painter, const QRectF &rect)
{
    draw_grid(painter);
}

void LifeGrid::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        const auto pos = scene_pos_to_grid_pos(event->scenePos());
        const bool is_valid = pos.x() >= 0 && pos.x() < grid_width &&
                              pos.y() >= 0 && pos.y() < grid_height;
        if(!is_valid)
        {
            return;
        }

        const auto cell = get_cell(pos.x(), pos.y());
        const auto target_state = paint_mode == MAKE_ALIVE ? ALIVE : DEAD;
        if(cell == target_state)
        {
            return;
        }

        set_cell(pos.x(), pos.y(), target_state);
        this->update();
    }

    if(is_dragging_view)
    {
        const auto position_diff = event->scenePos() - event->lastScenePos();
        offset_x += static_cast<float>(position_diff.x());
        offset_y += static_cast<float>(position_diff.y());
        this->update();
    }

}

void LifeGrid::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        const auto pos = scene_pos_to_grid_pos(event->scenePos());
        const bool is_valid = pos.x() >= 0 && pos.x() < grid_width &&
                              pos.y() >= 0 && pos.y() < grid_height;
        if(!is_valid)
        {
            return;
        }

        const auto cell = get_cell(pos.x(), pos.y());
        const auto target_state = cell == ALIVE ? DEAD : ALIVE;
        paint_mode = target_state == ALIVE ? MAKE_ALIVE : MAKE_DEAD;
        set_cell(pos.x(), pos.y(), target_state);

        this->update();
    }
    else if(event->button() == Qt::MouseButton::RightButton)
    {
        is_dragging_view = true;
    }
}

void LifeGrid::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::RightButton)
    {
        is_dragging_view = false;
    }
}

void LifeGrid::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    zoom += event->delta() / 20.f;
    if(zoom < 2)
    {
        zoom = 2;
    }
    else if(zoom > 100)
    {
        zoom = 100;
    }

    // TODO: Center around hovered position
    this->update();

}

void LifeGrid::draw_grid(QPainter *painter) const
{
    painter->setPen(QPen(Qt::black));

    // TODO: Only render the visible portion of the grid

    const float grid_total_width = this->grid_width * this->zoom;
    const float grid_total_height = this->grid_height * this->zoom;

    // The center of the canvas acts as the origin, so the start position needs to be moved left and up accordingly.
    // Also, the view offset is summed in afterwards.
    const float min_x = 0.f - grid_total_width  / 2.f + offset_x;
    const float min_y = 0.f - grid_total_height / 2.f + offset_y;

    const float max_x = grid_total_width / 2.f + offset_x;
    const float max_y = grid_total_height / 2.f + offset_y;

    const float cell_width  = (max_x - min_x) / grid_width;
    const float cell_height = (max_y - min_y) / grid_height;


    painter->setBrush(QBrush(Qt::BrushStyle::SolidPattern));
    size_t cell_index = 0;
    for(int y = 0; y < grid_height; y++)
    {
        const int current_y = to_int(min_y + cell_height * y);
        for(int x = 0; x < grid_width; x++) {
            const int current_x = to_int(min_x + cell_width * x);
            CELL cell = cells[cell_index++];
            if (cell == ALIVE) {
               painter->drawRect(current_x, current_y, to_int(cell_width), to_int(cell_height));
            }
        }
    }

    painter->setPen(QColor(127, 127, 127, 127));

    for(int x = 0; x <= grid_width; x++)
    {
        const int line_x = to_int(min_x + cell_width * x);
        painter->drawLine(line_x, to_int(min_y), line_x, to_int(max_y));
    }

    for(int y = 0; y <= grid_height; y++)
    {
        const int line_y = to_int(min_y + cell_height * y);
        painter->drawLine(to_int(min_x), line_y, to_int(max_x), line_y);
    }
}
