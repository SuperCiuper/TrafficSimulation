#include "../include/view/PathPainters.hpp"

#include <cstdlib>

#include <QBrush>
#include <QPainter>

#include <iostream>

namespace trafficsimulation::view
{

PathPainter::PathPainter()
    : startPoint_{common::Point{-10, -10}}
    , endPoint_{common::Point{-10, -10}}
{
}

PathPainter::~PathPainter() = default;

void PathPainter::setPoints(const common::Point startPoint, const common::Point endPoint)
{
    startPoint_ = startPoint;
    endPoint_ = endPoint;
}

void PathPainter::paint()
{
    std::cout << " PathPainter paint " << std::endl;
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
    std::cout << " PavementPainter paint " << std::endl;

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
    painter->setPen(QPen{Qt::black, 4, Qt::SolidLine});
    painter->drawLine(startPoint_.x, startPoint_.y, endPoint_.x, endPoint_.y);
}

} // trafficsimulation::view
