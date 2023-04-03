#include "../../include/view/PathPainters.hpp"

#include <cstdlib>

#include <QBrush>
#include <QPainter>

namespace trafficsimulation::view
{

PathPainter::PathPainter()
    : startPoint_{model::Point{-10, -10}}
    , endPoint_{model::Point{-10, -10}}
{
}

PathPainter::~PathPainter() = default;

void PathPainter::setPoints(const model::Point startPoint, const model::Point endPoint)
{
    startPoint_ = startPoint;
    endPoint_ = endPoint;
}

void PathPainter::paint()
{
    update();
}

QRectF PathPainter::boundingRect() const
{
    return QRect(0, 0, 1300, 820);
}

PavementPainter::PavementPainter()
    : PathPainter{}
{
}

PavementPainter::~PavementPainter() = default;

void PavementPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // auto pen = QPen{Qt::blue, 2, Qt::SolidLine};
    // auto brush = QBrush{Qt::green};
    painter->setPen(QPen{Qt::gray, 2, Qt::SolidLine});
    painter->drawLine(startPoint_.x, startPoint_.y, endPoint_.x, endPoint_.y);
}

RoadPainter::RoadPainter()
    : PathPainter{}
{
}

RoadPainter::~RoadPainter() = default;

void RoadPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // auto pen = QPen{Qt::blue, 2, Qt::SolidLine};
    // auto brush = QBrush{Qt::green};
    painter->setPen(QPen{Qt::black, 2, Qt::SolidLine});
    painter->drawLine(startPoint_.x, startPoint_.y, endPoint_.x, endPoint_.y);
}

} // trafficsimulation::view
