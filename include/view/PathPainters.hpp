#ifndef PATHPAINTERS_HPP
#define PATHPAINTERS_HPP

#include <QGraphicsItem>

#include "../interface/LinePainter.hpp"
#include "../model/Point.hpp"

namespace trafficsimulation::view
{

class PathPainter : public interface::LinePainter, public QGraphicsItem
{
public:
    virtual ~PathPainter();

    void setPoints(const model::Point startPoint, const model::Point endPoint);
    void paint();

    QRectF boundingRect() const;

protected:
    PathPainter();

    model::Point startPoint_;
    model::Point endPoint_;
};

class PavementPainter : public PathPainter
{
public:
    PavementPainter();
    ~PavementPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class RoadPainter : public PathPainter
{
public:
    RoadPainter();
    ~RoadPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

} // trafficsimulation::view

#endif // PATHPAINTERS_HPP
