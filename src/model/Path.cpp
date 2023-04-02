#include "../../include/model/Path.hpp"

#include "../../include/interface/LinePainter.hpp"
#include "../../include/model/Junction.hpp"

namespace trafficsimulation::model
{

Path::Path(const uint32_t pathId, const uint32_t length, const Point startPoint,
    const Point endPoint, const std::shared_ptr<Junction> endJunction)
    : pathId_{pathId}
    , length_{length}
    , startPoint_{startPoint}
    , endPoint_{endPoint}
    , shiftOfStartPoint_{Point{static_cast<int16_t>(startPoint_.x - endPoint.x),
        static_cast<int16_t>(startPoint_.y - endPoint.y)}}
    , endJunction_{endJunction}
{
}

Path::~Path() = default;

uint32_t Path::getPathId() const
{
    return pathId_;
}

uint32_t Path::getLength() const
{
    return length_;
}

Point Path::getStartPoint() const
{
    return startPoint_;
}

const std::shared_ptr<Junction> Path::getJunction() const
{
    return std::shared_ptr<Junction>{endJunction_};
}

void Path::setPainter(const std::shared_ptr<interface::LinePainter> painter)
{
    painter_ = painter;
}

Point Path::calculateNewPosition(uint32_t distanceTravelled) const
{
    auto partTravelled = static_cast<float>(distanceTravelled / length_);
    auto newPoint = Point{};
    newPoint.x = startPoint_.x + (shiftOfStartPoint_.x / partTravelled );
    newPoint.y = startPoint_.y + (shiftOfStartPoint_.y / partTravelled );

    return newPoint;
}

void Path::update()
{
    if(painter_ == nullptr)
    {
        // add log
        return;
    }
    painter_->draw(startPoint_, endPoint_);
}

} // trafficsimulation::model
