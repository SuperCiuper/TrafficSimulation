#include "path.hpp"
#include<QDebug>

Path::Path(const QPoint startPoint, const QPoint endPoint, QColor color, int offset)
    : startPoint_{startPoint}
    , endPoint_{endPoint}
    , defaultColor_{color}
    , color_{color}
    , offsetX_{offset}
    , offsetY_{offset}
{
    qInfo() << startPoint.x() << " " << startPoint_.x() << "   " << endPoint.x() << " " << endPoint_.x();

    setAcceptHoverEvents(true);
    if(startPoint.y() > endPoint.y())
    {
        offsetX_ *= -1;
    }
    if(startPoint.x() < endPoint.x())
    {
        offsetY_ *= -1;
    }
}

QRectF Path::boundingRect() const
{
    return QRect(0, 0, 1300, 820);
}

void Path::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(color_ == defaultColor_)
    {
        color_ = Qt::red;
    }
    else
    {
        color_ = defaultColor_;
    }

    qInfo() << startPoint_.x() << "   " << endPoint_.x();

    painter->setPen(color_);
    painter->drawLine(startPoint_.x() + offsetX_, startPoint_.y() + offsetY_,
                      endPoint_.x() + offsetX_, endPoint_.y() + offsetY_);
}

void Path::setSelected(bool selected)
{
    if(selected)
    {
        color_ = Qt::red;
    }
    else
    {
        color_ = defaultColor_;
    }
}

void Path::advance(int phase)
{
    if(!phase)
    {
        return;
    }
    update();
}

