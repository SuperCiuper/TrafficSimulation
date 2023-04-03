#include "../include/model/MovingObject.hpp"

#include "../include/interface/PointPainter.hpp"

namespace trafficsimulation::model
{

MovingObject::MovingObject()
    : position_{common::Point{}}
    , destinationId_{NO_DESTINATION}
    , distanceTravelled_{0}
    , painter_{nullptr}
{
    position_.x = 0xFF;
    position_.y = 0xFF;
}

MovingObject::~MovingObject() = default;

uint32_t MovingObject::getDestination() const
{
    return destinationId_;
}

void MovingObject::setDestination(const uint32_t newDestinationId)
{
    destinationId_ = newDestinationId;
}

void MovingObject::setPainter(std::unique_ptr<interface::PointPainter> painter)
{
    painter_ = std::move(painter);
}

void MovingObject::update()
{
    move();

    if(painter_ == nullptr)
    {
        // add log
        return;
    }
    painter_->setPoint(position_);
    painter_->paint();
}

} // trafficsimulation::model
