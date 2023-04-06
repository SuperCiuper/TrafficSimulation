#ifndef PATH_HPP
#define PATH_HPP

#include <memory>

#include "../common/Point.hpp"

namespace trafficsimulation::interface{ class LinePainter; }

namespace trafficsimulation::model
{

class Junction;

class Path
{
public:
    Path(const uint32_t pathId, const uint32_t length, const common::Point startPoint,
        const common::Point endPoint, const std::shared_ptr<Junction> endJunction);
    virtual ~Path();

    uint32_t getPathId() const;
    uint32_t getLength() const;
    common::Point getStartPoint() const;
    std::shared_ptr<Junction> getJunction() const;

    common::Point calculateNewPosition(uint32_t distanceTravelled) const;

    void setPainter(interface::LinePainter* const painter);
    void update();

private:
    const uint32_t pathId_;
    const uint32_t length_;
    const common::Point startPoint_;
    const common::Point endPoint_;
    const common::Point shiftOfStartPoint_;
    const std::shared_ptr<Junction> endJunction_;

    interface::LinePainter* painter_;
};

} // trafficsimulation::model

#endif // PATH_HPP
