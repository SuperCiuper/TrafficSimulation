#include "../include/view/PointPainters.hpp"

#include <cstdlib>

#include <QBrush>
#include <QGraphicsScene>
#include <QPainter>

namespace trafficsimulation::view
{

constexpr auto SCENEWIDTH = uint32_t{1300};
constexpr auto SCENEHEIGHT = uint32_t{820};

constexpr auto JUNCTIONZVALUE = qreal{2};
constexpr auto DRIVERZVALUE = qreal{3};
constexpr auto PEDESTRIANZVALUE = qreal{3};
constexpr auto JUNCTIONDIAMETER = uint32_t{36};
constexpr auto DRIVERDIAMETER = uint32_t{10};
constexpr auto PEDESTRIANDIAMETER = uint32_t{7};

namespace
{

QColor randomColor()
{
    auto highValue = std::rand() % 66 + 190;
    auto lowValue = std::rand() % 90;
    auto midValue = std::rand() % (highValue - lowValue) + lowValue;

    auto distribution = std::rand() % 6;

    switch (distribution) {
        case 0:
            return QColor{highValue, midValue, lowValue};
        case 1:
            return QColor{highValue, lowValue, midValue};
        case 2:
            return QColor{midValue, highValue, lowValue};
        case 3:
            return QColor{midValue, lowValue, highValue};
        case 4:
            return QColor{lowValue, highValue, midValue};
        case 5:
            return QColor{lowValue, midValue, highValue};
        default:
            return QColor{Qt::black};
    }
}

}

PointPainter::PointPainter()
    : interface::PointPainter{}
    , QGraphicsItem{}
    , point_{common::Point{-10, -10}}
    , highlight_{false}
{
}

PointPainter::~PointPainter() = default;

void PointPainter::setPoint(const common::Point point, const bool highlight)
{
    point_ = point;
    highlight_ = highlight;
}

void PointPainter::paint()
{
    update();
}

QRectF PointPainter::boundingRect() const
{
    return QRect(0, 0, SCENEWIDTH, SCENEHEIGHT);
}

JunctionPainter::JunctionPainter()
    : PointPainter{}
{
    setZValue(JUNCTIONZVALUE);
}

JunctionPainter::~JunctionPainter() = default;

void JunctionPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen{Qt::darkBlue, 2, Qt::SolidLine});
    painter->setBrush(QBrush{Qt::darkCyan});
    painter->drawEllipse(point_.x - JUNCTIONDIAMETER/2, point_.y - JUNCTIONDIAMETER/2,
        JUNCTIONDIAMETER, JUNCTIONDIAMETER);
}

DriverPainter::DriverPainter()
    : PointPainter{}
    , color_{randomColor()}
{
    setZValue(DRIVERZVALUE);
}

DriverPainter::~DriverPainter() = default;

void DriverPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen{color_, 1, Qt::SolidLine});
    painter->setBrush(QBrush{color_});
    painter->drawEllipse(point_.x - DRIVERDIAMETER/2, point_.y - DRIVERDIAMETER/2,
        DRIVERDIAMETER, DRIVERDIAMETER);
}

PedestrianPainter::PedestrianPainter()
    : PointPainter{}
    , color_{randomColor()}
{
    setZValue(PEDESTRIANZVALUE);
}

PedestrianPainter::~PedestrianPainter() = default;

void PedestrianPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen{color_, 1, Qt::SolidLine});
    painter->setBrush(QBrush{color_});
    painter->drawEllipse(point_.x - PEDESTRIANDIAMETER/2, point_.y - PEDESTRIANDIAMETER/2,
        PEDESTRIANDIAMETER, PEDESTRIANDIAMETER);
}

} // trafficsimulation::view
