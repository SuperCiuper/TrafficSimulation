#ifndef PATHPAINTERS_HPP
#define PATHPAINTERS_HPP

#include <QGraphicsItem>

#include "../interface/LinePainter.hpp"
#include "../common/Point.hpp"

namespace trafficsimulation::view
{

/*!
 * \brief The PathPainter abstract class
 *
 * Inherits interface::LinePainter abstract class and QGraphicsItem class.
 * Paints line in simulation.
 */
class PathPainter : public interface::LinePainter, public QGraphicsItem
{
public:
    virtual ~PathPainter();

    /*!
     * \brief setPoints
     * \param common::Point startPoint
     * \param common::Point endPoint
     *
     * Overrides and implements interface::LinePainter::setPoints method
     */
    void setPoints(const common::Point startPoint, const common::Point endPoint) override;
    /*!
     * \brief paint
     *
     * Overrides and implements interface::LinePainter::paint method
     */
    void paint() override;

    QRectF boundingRect() const override;

protected:
    PathPainter();

    common::Point startPoint_;
    common::Point endPoint_;
};

/*!
 * \brief The PavementPainter class
 *
 * Inherits PathPainter abstract class.
 * Paints Pavement object in simulation.
 */
class PavementPainter : public PathPainter
{
public:
    PavementPainter();
    ~PavementPainter();

    /*!
     * \brief paint
     * \param QPainter *painter
     * \param QStyleOptionGraphicsItem *option
     * \param QWidget *widget
     *
     * Implementation of QGraphicsItem::paint method
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

/*!
 * \brief The RoadPainter class
 *
 * Inherits PathPainter abstract class.
 * Paints Road object in simulation.
 */
class RoadPainter : public PathPainter
{
public:
    RoadPainter();
    ~RoadPainter();

    /*!
     * \brief paint
     * \param QPainter *painter
     * \param QStyleOptionGraphicsItem *option
     * \param QWidget *widget
     *
     * Implementation of QGraphicsItem::paint method
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

} // trafficsimulation::view

#endif // PATHPAINTERS_HPP
