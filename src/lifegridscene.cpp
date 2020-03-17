#include "lifegridscene.h"

#include <iostream>
#include <numeric>
#include <type_traits>
#include <thread>
#include <chrono>

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

LifeGridScene::LifeGridScene(QObject *_parent) :
    QGraphicsScene(_parent),
    zoom{20},
    offset_x{0.f},
    offset_y{0.f},
    paint_mode{MAKE_ALIVE},
    is_dragging_view{false},
    speed{1},
    is_running{false},
    is_painting_enabled{true}
{
    resize_grid(10,10);
    create_glider();
}

LifeGridScene::~LifeGridScene()
{
}

QPoint LifeGridScene::scene_pos_to_grid_pos(const QPointF &scene_pos) const
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

    // The -2.f adjustments are to fix the slight alignment error caused by cell borders and float to int conversion
    float grid_x = (scene_x - min_x - 2.f) / cell_width;
    float grid_y = (scene_y - min_y - 2.f) / cell_height;

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

void LifeGridScene::run(bool run)
{
    is_running = run;

    if(is_running)
    {
        update_thread = std::thread([&]()
        {
            while(this->is_running)
            {
                this->next_generation();
                this->update();

                const auto delay = std::chrono::milliseconds( 1000 / this->speed);
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        });
    }
    else {
        update_thread.join();
    }
}

void LifeGridScene::stop_and_wait_for_thread()
{
    if(update_thread.joinable())
    {
        is_running = false;
        update_thread.join();
    }
}

void LifeGridScene::set_speed(int updates_per_second)
{
    speed = updates_per_second;
}

void LifeGridScene::toggle_painting_enabled(bool enabled)
{
    is_painting_enabled = enabled;
}

void LifeGridScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    draw_grid(painter);
}

void LifeGridScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(is_painting_cells)
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

void LifeGridScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        const auto pos = scene_pos_to_grid_pos(event->scenePos());
        const bool is_valid = pos.x() >= 0 && pos.x() < grid_width &&
                              pos.y() >= 0 && pos.y() < grid_height;
        if(!is_valid || !is_painting_enabled)
        {
            return;
        }

        is_painting_cells = true;

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

void LifeGridScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        is_painting_cells = false;
    }
    else if(event->button() == Qt::MouseButton::RightButton)
    {
        is_dragging_view = false;
    }
}

void LifeGridScene::wheelEvent(QGraphicsSceneWheelEvent *event)
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

void LifeGridScene::draw_grid(QPainter *painter) const
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
        for(int x = 0; x < grid_width; x++)
        {
            const int current_x = to_int(min_x + cell_width * x);
            CELL cell = cells[cell_index++];
            if (cell == ALIVE)
            {
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
