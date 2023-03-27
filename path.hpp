#ifndef PATH_HPP
#define PATH_HPP

#include <QGraphicsItem >
#include <QPen>
#include <QPainter>

class Path : public QGraphicsItem
{

public:
    Path(const QPoint startPoint, const QPoint endPoint, QColor color, int offset);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setSelected(bool selected);

protected:
    void advance(int phase);

private:
    const QPoint startPoint_;
    const QPoint endPoint_;
    const QColor defaultColor_;
    QColor color_;
    int offsetX_;
    int offsetY_;
};

#endif // PATH_HPP
