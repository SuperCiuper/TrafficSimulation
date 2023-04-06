#include "../include/model/Path.hpp"

#include "../include/interface/LinePainter.hpp"
#include "../include/model/Junction.hpp"

namespace trafficsimulation::model
{

Path::Path(const uint32_t pathId, const uint32_t length, const common::Point startPoint,
    const common::Point endPoint, const std::shared_ptr<Junction> endJunction)
    : pathId_{pathId}
    , length_{length}
    , startPoint_{startPoint}
    , endPoint_{endPoint}
    , shiftOfStartPoint_{common::Point{static_cast<int16_t>(endPoint_.x - startPoint_.x),
        static_cast<int16_t>(endPoint_.y - startPoint_.y)}}
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

common::Point Path::getStartPoint() const
{
    return startPoint_;
}

std::shared_ptr<Junction> Path::getJunction() const
{
    return endJunction_;
}

common::Point Path::calculateNewPosition(uint32_t distanceTravelled) const
{
    auto partTravelled = static_cast<float>(distanceTravelled) / static_cast<float>(length_);

    std::cout << partTravelled << std::endl;
    auto newPoint = common::Point{};
    newPoint.x = startPoint_.x + (shiftOfStartPoint_.x * partTravelled);
    newPoint.y = startPoint_.y + (shiftOfStartPoint_.y * partTravelled);

    return newPoint;
}

void Path::setPainter(interface::LinePainter* const painter)
{
    painter_ = painter;
    painter_->setPoints(startPoint_, endPoint_);
}

void Path::update()
{
    if(painter_ == nullptr)
    {
        // add log
        return;
    }
    painter_->paint();
}

} // trafficsimulation::model
