#ifndef PATHPAINTERS_HPP
#define PATHPAINTERS_HPP

#include <QGraphicsItem>

#include "../interface/LinePainter.hpp"
#include "../common/Point.hpp"

namespace trafficsimulation::view
{

class PathPainter : public interface::LinePainter, public QGraphicsItem
{
public:
    virtual ~PathPainter();

    void setPoints(const common::Point startPoint, const common::Point endPoint) override;
    void paint() override;

    QRectF boundingRect() const override;

protected:
    PathPainter();

    common::Point startPoint_;
    common::Point endPoint_;
};

class PavementPainter : public PathPainter
{
public:
    PavementPainter();
    ~PavementPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class RoadPainter : public PathPainter
{
public:
    RoadPainter();
    ~RoadPainter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

} // trafficsimulation::view

#endif // PATHPAINTERS_HPP
