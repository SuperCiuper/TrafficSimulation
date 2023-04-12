#ifndef POINTPAINTERS_HPP
#define POINTPAINTERS_HPP

#include <QColor>
#include <QGraphicsItem>

#include "../interface/PointPainter.hpp"
#include "../common/Point.hpp"

namespace trafficsimulation::view
{

class PointPainter : public interface::PointPainter, public QGraphicsItem
{
public:
    virtual ~PointPainter();

    void setPoint(const common::Point point) override;
    void paint() override;

    QRectF boundingRect() const override;

protected:
    PointPainter();

    common::Point point_;
};

class JunctionPainter : public PointPainter
{
public:
    JunctionPainter();
    ~JunctionPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class DriverPainter : public PointPainter
{
public:
    DriverPainter();
    ~DriverPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor color_;
};

class PedestrianPainter : public PointPainter
{
public:
    PedestrianPainter();
    ~PedestrianPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor color_;
};

} // trafficsimulation::view

#endif // POINTPAINTERS_HPP
