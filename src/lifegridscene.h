#ifndef LIFEGRIDSCENE_H
#define LIFEGRIDSCENE_H

#include "cellkernel.h"
#include "lifegrid.h"

#include <QGraphicsScene>
#include <QPaintEvent>

#include <memory>
#include <vector>
#include <thread>


/*!
 * \brief When dragging, should we animate or kill the cells
 */
enum PaintMode : unsigned char
{
    MAKE_DEAD,
    MAKE_ALIVE
};

/*!
 * \brief The interface between the user and LifeGrid
 * \details Handles GUI events that affect how the grid is modified and shown
 */
class LifeGridScene : public LifeGrid, public QGraphicsScene
{
  public:
    explicit LifeGridScene(QObject *_parent = nullptr);
    virtual ~LifeGridScene() override;

    /*!
     * \brief The zoom level. Can be adjusted with the mouse wheel.
     */
    int zoom;

    /*!
     * \brief The X-axis offset.
     */
    float offset_x;

    /*!
     * \brief The Y-axis offset.
     */
    float offset_y;

    /*!
     * \brief Run the simulation automatically or stop it
     * \param ruun Whether to run or stop
     */
    void run(bool run);

    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    /*!
     * \brief Called when exiting
     * \details Will return only after update_thread is stopped if the simulation is running
     */
    void stop_and_wait_for_thread();

    /*!
     * \brief Set the simulation speed
     * \param updates_per_second How many times in a second should we step
     */
    void set_speed(int updates_per_second);


    /*!
     * \brief Is painting enabled
     * \param enabled Can the user paint?
     */
    void toggle_painting_enabled(bool enabled);

  private:
    /*!
     * \brief Renders the grid
     * \param painter Passed in by Qt upon an update
     */
    void draw_grid(QPainter *painter, const QRectF &rect) const;

    /*!
     * \brief Calculates the relevant grid position when an UI event happens
     * \param scene_pos A position given by the Qt
     * \return The translated position in grid space
     */
    QPoint scene_pos_to_grid_pos(const QPointF &scene_pos) const;

    /*!
     * \brief When dragging, should the cell be animated or killed?
     */
    PaintMode paint_mode;

    /*!
     * \brief Is the user painting cells?
     * \details True when the user drags mouse around the grid, whilse holding the left mouse button down
     */
    bool is_painting_cells;


    /*!
     * \brief Is the user dragging the view?
     * \details True when the user drags mouse around the grid, while holding the right mouse button down
     */
    bool is_dragging_view;

    /*!
     * \brief Simulation speed. Updates per second.
     */
    int speed;

    /*!
     * \brief Is the simulation running?
     */
    bool is_running;

    /*!
     * \brief The update thread, used when the simulation is running on its own
     */
    std::thread update_thread;


    /*!
     * \brief Can the user paint cells?
     */
    bool is_painting_enabled;
};

#endif // LIFEGRIDSCENE_H
