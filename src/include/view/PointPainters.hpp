#ifndef POINTPAINTERS_HPP
#define POINTPAINTERS_HPP

#include <QColor>
#include <QGraphicsItem>

#include "../interface/PointPainter.hpp"
#include "../common/Point.hpp"

namespace trafficsimulation::view
{

/*!
 * \brief The PointPainter abstract class
 *
 * Inherits interface::PointPainter abstract class and QGraphicsItem class.
 * Paints single point in simulation.
 */
class PointPainter : public interface::PointPainter, public QGraphicsItem
{
public:
    virtual ~PointPainter();

    /*!
     * \brief setPoints
     * \param common::Point startPoint
     * \param common::Point endPoint
     *
     * Overrides and implements interface::PointPainter::setPoints method
     */
    void setPoint(const common::Point point) override;
    /*!
     * \brief paint
     *
     * Overrides and implements interface::PointPainter::paint method
     */
    void paint() override;

    QRectF boundingRect() const override;

protected:
    PointPainter();

    common::Point point_;
};

/*!
 * \brief The JunctionPainter class
 *
 * Inherits PointPainter abstract class.
 * Paints Junction object in simulation.
 */
class JunctionPainter : public PointPainter
{
public:
    JunctionPainter();
    ~JunctionPainter();

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
 * \brief The DriverPainter class
 *
 * Inherits PointPainter abstract class.
 * Paints Driver object in simulation.
 */
class DriverPainter : public PointPainter
{
public:
    DriverPainter();
    ~DriverPainter();

    /*!
     * \brief paint
     * \param QPainter *painter
     * \param QStyleOptionGraphicsItem *option
     * \param QWidget *widget
     *
     * Implementation of QGraphicsItem::paint method
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor color_;
};

/*!
 * \brief The PedestrianPainter class
 *
 * Inherits PointPainter abstract class.
 * Paints Pedestrian object in simulation.
 */
class PedestrianPainter : public PointPainter
{
public:
    PedestrianPainter();
    ~PedestrianPainter();

    /*!
     * \brief paint
     * \param QPainter *painter
     * \param QStyleOptionGraphicsItem *option
     * \param QWidget *widget
     *
     * Implementation of QGraphicsItem::paint method
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor color_;
};

} // trafficsimulation::view

#endif // POINTPAINTERS_HPP
