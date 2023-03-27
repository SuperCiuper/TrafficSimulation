#ifndef PATH_HPP
#define PATH_HPP

#include <memory>

#include "Point.hpp"

class Junction;

class Path
{
public:
    Path(const uint32_t pathId, const uint32_t length_, const Point startPoint,
        const Point endPoint, const std::shared_ptr<Junction> endJunction);
    virtual ~Path();

    uint32_t getPathId() const;
    uint32_t getLengthBeforeJunction() const;
    Point getStartPoint() const;
    const std::shared_ptr<Junction>& getJunction() const;
    Point calculateNewPosition(uint32_t distanceTravelled) const;

private:
    const uint32_t pathId_;
    const uint32_t length_;
    const uint32_t lengthBeforeJunction_;
    const Point startPoint_;
    const Point endPoint_;
    const std::shared_ptr<Junction> endJunction_;
};

#endif // PATH_HPP
