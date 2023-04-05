#include "../include/view/PathPainters.hpp"

#include <cstdlib>

#include <QBrush>
#include <QGraphicsScene>
#include <QPainter>

#include <iostream>

namespace trafficsimulation::view
{

constexpr auto SCENEWIDTH = uint32_t{1300};
constexpr auto SCENEHEIGHT = uint32_t{820};

constexpr auto PATHZVALUE = qreal{0};
constexpr auto PAVEMENTPENWIDTH = uint32_t{2};
constexpr auto ROADPENWIDTH = uint32_t{4};

PathPainter::PathPainter()
    : startPoint_{common::Point{-10, -10}}
    , endPoint_{common::Point{-10, -10}}
{
    setZValue(PATHZVALUE);
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
    scene()->update();
}

QRectF PathPainter::boundingRect() const
{
    return QRect(0, 0, SCENEWIDTH, SCENEHEIGHT);
}

PavementPainter::PavementPainter()
    : PathPainter{}
{
}

PavementPainter::~PavementPainter() = default;

void PavementPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    std::cout << " PavementPainter paint " << std::endl;

    painter->setPen(QPen{Qt::gray, PAVEMENTPENWIDTH, Qt::SolidLine});
    painter->drawLine(startPoint_.x, startPoint_.y, endPoint_.x, endPoint_.y);
}

RoadPainter::RoadPainter()
    : PathPainter{}
{
}

RoadPainter::~RoadPainter() = default;

void RoadPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen{Qt::black, ROADPENWIDTH, Qt::SolidLine});
    painter->drawLine(startPoint_.x, startPoint_.y, endPoint_.x, endPoint_.y);
}

} // trafficsimulation::view
