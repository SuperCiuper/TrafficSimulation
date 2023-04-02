#ifndef PATH_HPP
#define PATH_HPP

#include <memory>

#include "Point.hpp"

namespace trafficsimulation::interface{ class LinePainter; }

namespace trafficsimulation::model
{

class Junction;

class Path
{
public:
    Path(const uint32_t pathId, const uint32_t length, const Point startPoint,
        const Point endPoint, const std::shared_ptr<Junction> endJunction);
    virtual ~Path();

    uint32_t getPathId() const;
    uint32_t getLength() const;
    Point getStartPoint() const;
    const std::shared_ptr<Junction> getJunction() const;

    Point calculateNewPosition(uint32_t distanceTravelled) const;

    void setPainter(const std::shared_ptr<interface::LinePainter> painter);
    void update();

private:
    const uint32_t pathId_;
    const uint32_t length_;
    const Point startPoint_;
    const Point endPoint_;
    const Point shiftOfStartPoint_;
    const std::weak_ptr<Junction> endJunction_;

    std::shared_ptr<interface::LinePainter> painter_;
};

} // trafficsimulation::model

#endif // PATH_HPP
