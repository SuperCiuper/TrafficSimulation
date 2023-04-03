#include "../include/view/PointPainters.hpp"

#include <cstdlib>

#include <QBrush>
#include <QPainter>

namespace trafficsimulation::view
{

constexpr auto JUNCTIONRADIUS = uint32_t{36};
constexpr auto DRIVERRADIUS = uint32_t{3};
constexpr auto PEDESTRIANRADIUS = uint32_t{2};

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
    : point_{common::Point{-10, -10}}
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
    return QRect(0, 0, 1300, 820);
}

JunctionPainter::JunctionPainter()
    : PointPainter{}
{
}

JunctionPainter::~JunctionPainter() = default;

void JunctionPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // auto pen = QPen{Qt::blue, 2, Qt::SolidLine};
    // auto brush = QBrush{Qt::green};
    painter->setPen(QPen{Qt::blue, 2, Qt::SolidLine});
    painter->setBrush(QBrush{Qt::green});
    painter->drawEllipse(point_.x - JUNCTIONRADIUS/2, point_.y - JUNCTIONRADIUS/2,
        JUNCTIONRADIUS, JUNCTIONRADIUS);
}

DriverPainter::DriverPainter()
    : PointPainter{}
    , color_{randomColor()}
{
}

DriverPainter::~DriverPainter() = default;

void DriverPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // auto pen = QPen{Qt::blue, 2, Qt::SolidLine};
    // auto brush = QBrush{Qt::green};
    painter->setPen(QPen{color_, 1, Qt::SolidLine});
    painter->setBrush(QBrush{color_});
    painter->drawEllipse(point_.x - DRIVERRADIUS/2, point_.y - DRIVERRADIUS/2,
        DRIVERRADIUS, DRIVERRADIUS);
}

PedestrianPainter::PedestrianPainter()
    : PointPainter{}
    , color_{randomColor()}
{
}

PedestrianPainter::~PedestrianPainter() = default;

void PedestrianPainter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // auto pen = QPen{Qt::blue, 2, Qt::SolidLine};
    // auto brush = QBrush{Qt::green};
    painter->setPen(QPen{color_, 1, Qt::SolidLine});
    painter->setBrush(QBrush{color_});
    painter->drawEllipse(point_.x - PEDESTRIANRADIUS/2, point_.y - PEDESTRIANRADIUS/2,
        PEDESTRIANRADIUS, PEDESTRIANRADIUS);
}

} // trafficsimulation::view
