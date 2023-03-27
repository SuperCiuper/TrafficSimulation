#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include <string>

#include "Point.hpp"

class MovingObject
{
public:
    MovingObject();
    virtual ~MovingObject();

    const std::string& getColorHex() const;
    Point getPosition() const;
    uint32_t getDestination() const;

    void setDestination(const uint32_t newDestination);

    virtual void move() = 0;

protected:
    virtual void selectNewPath() = 0;

    Point position_;
    uint32_t destinationId_;
    uint32_t distanceTravelled_;
    uint32_t distanceToJunction_;

private:
    const std::string colorHex_;
};

#endif // MOVINGOBJECT_HPP
