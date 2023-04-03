#ifndef POINTPAINTERS_HPP
#define POINTPAINTERS_HPP

#include <QColor>
#include <QGraphicsItem>

#include "../interface/PointPainter.hpp"
#include "../model/Point.hpp"

namespace trafficsimulation::view
{

class PointPainter : public interface::PointPainter, public QGraphicsItem
{
public:
    virtual ~PointPainter();

    void setPoint(const model::Point point, const bool highlight = false);
    void paint();

    QRectF boundingRect() const;

protected:
    PointPainter();

    model::Point point_;
    bool highlight_;
};

class JunctionPainter : public PointPainter
{
public:
    JunctionPainter();
    ~JunctionPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class DriverPainter : public PointPainter
{
public:
    DriverPainter();
    ~DriverPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QColor color_;
};

class PedestrianPainter : public PointPainter
{
public:
    PedestrianPainter();
    ~PedestrianPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QColor color_;
};

} // trafficsimulation::view

#endif // POINTPAINTERS_HPP
