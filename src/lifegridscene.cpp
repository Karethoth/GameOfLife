#include "lifegridscene.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

#include <type_traits>
#include <numeric>
#include <thread>
#include <chrono>



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
    LifeGrid{14},
    QGraphicsScene(_parent),
    zoom{20},
    offset_x{0.f},
    offset_y{0.f},
    paint_mode{MAKE_ALIVE},
    is_painting_cells{false},
    is_dragging_view{false},
    speed{1},
    is_running{false},
    is_painting_enabled{true}
{
}

LifeGridScene::~LifeGridScene()
{
}

QPoint LifeGridScene::scene_pos_to_grid_pos(const QPointF &scene_pos) const
{
    const float grid_total_width  = this->grid_width  * this->zoom;
    const float grid_total_height = this->grid_height * this->zoom;

    const float min_x = 0.f - grid_total_width  / 2.f + offset_x;
    const float min_y = 0.f - grid_total_height / 2.f + offset_y;

    const float max_x = grid_total_width  / 2.f + offset_x;
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

                const auto delay = std::chrono::milliseconds(1000 / this->speed);
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        });
    }
    else
    {
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
    draw_grid(painter, rect);
}

void LifeGridScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(is_painting_cells)
    {
        const auto pos = scene_pos_to_grid_pos(event->scenePos());

        // Check if the mouse is hovering over the grid
        const bool is_valid = pos.x() >= 0 && pos.x() < grid_width &&
                              pos.y() >= 0 && pos.y() < grid_height;
        if(!is_valid)
        {
            return;
        }

        // Check if the cell needs to be updated
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

        // Ignore if the mouse is outside of the grid, or if painting is disabled
        const bool is_valid = pos.x() >= 0 && pos.x() < grid_width &&
                              pos.y() >= 0 && pos.y() < grid_height;
        if(!is_valid || !is_painting_enabled)
        {
            return;
        }

        is_painting_cells = true;

        // Save paint_mode reverse to the cell's current state
        // and update the cell
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
    const auto old_zoom = zoom;
    zoom += event->delta() / 20.f;

    if(zoom < 2)
    {
        zoom = 2;
    }
    else if(zoom > 100)
    {
        zoom = 100;
    }

    // If the zoom was changed, scale the view offsets accordingly.
    // This will keep the grid "still" when zooming in and out.
    if(zoom != old_zoom)
    {
        const auto zoom_ratio = static_cast<float>(zoom) / (old_zoom);
        offset_x *= zoom_ratio;
        offset_y *= zoom_ratio;
    }


    this->update();

}

void LifeGridScene::draw_grid(QPainter *painter, const QRectF &rect) const
{
    painter->setPen(QPen(Qt::black));

    const float grid_total_width  = this->grid_width  * this->zoom;
    const float grid_total_height = this->grid_height * this->zoom;

    // The center of the canvas acts as the origin, so the start position needs to be moved left and up accordingly.
    // Also, the view offset is summed in afterwards.
    const float min_x = 0.f - grid_total_width  / 2.f + offset_x;
    const float min_y = 0.f - grid_total_height / 2.f + offset_y;

    const float max_x = grid_total_width  / 2.f + offset_x;
    const float max_y = grid_total_height / 2.f + offset_y;

    const float cell_width  = zoom;
    const float cell_height = zoom;

    // Grab the displayed area
    const float scene_width  = static_cast<float>(rect.width());
    const float scene_height = static_cast<float>(rect.height());

    // A safety margin to render content slightly outside of the display
    const float display_margin  = 2 * zoom;

    // Calculate limits around the visible area.
    // These are to limit what should be rendered and what should be ignored.
    const float displayed_min_x = 0 - scene_width  / 2 - display_margin;
    const float displayed_min_y = 0 - scene_height / 2 - display_margin;
    const float displayed_max_x = displayed_min_x + scene_width  + display_margin * 2;
    const float displayed_max_y = displayed_min_y + scene_height + display_margin * 2;


    // Draw the live cells
    painter->setBrush(QBrush(Qt::BrushStyle::SolidPattern));
    size_t cell_index = 0;
    for(int y = 0; y < grid_height; y++)
    {
        const int current_y = to_int(min_y + cell_height * y);

        // If this row isn't displayed, skip it
        if(current_y < displayed_min_y)
        {
            cell_index += static_cast<size_t>(grid_width);
            continue;
        }
        else if(current_y > displayed_max_y)
        {
            break;
        }

        for(int x = 0; x < grid_width; x++)
        {
            const int current_x = to_int(min_x + cell_width * x);

            // If this column isn't displayed, skip it
            if(current_x < displayed_min_x)
            {
                cell_index += 1;
                continue;
            }
            else if(current_x > displayed_max_x)
            {
                cell_index += 1;
                continue;
            }

            CELL cell = cells[cell_index++];
            if (cell == ALIVE)
            {
               painter->drawRect(current_x, current_y, to_int(cell_width), to_int(cell_height));
            }
        }
    }

    // Draw the grid
    painter->setPen(QColor(127, 127, 127, 127));
    for(int x = 0; x <= grid_width; x++)
    {
        const int line_x = to_int(min_x + cell_width * x);

        // If this column isn't displayed, skip it
        if(line_x < displayed_min_x)
        {
            continue;
        }
        else if(line_x > displayed_max_x)
        {
            break;
        }

        painter->drawLine(
            line_x,
            to_int(std::max(min_y, displayed_min_y)),
            line_x,
            to_int(std::min(max_y, displayed_max_y))
        );
    }

    for(int y = 0; y <= grid_height; y++)
    {
        const int line_y = to_int(min_y + cell_height * y);

        // If this row isn't displayed, skip it
        if(line_y < displayed_min_y)
        {
            continue;
        }
        else if(line_y > displayed_max_y)
        {
            break;
        }

        painter->drawLine(
            to_int(std::max(min_x, displayed_min_x)),
            line_y,
            to_int(std::min(max_x, displayed_max_x)),
            line_y
        );
    }
}
