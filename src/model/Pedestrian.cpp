#include "../include/model/Pedestrian.hpp"

#include <cstdlib>
#include <ctime>

#include "../include/model/Junction.hpp"
#include "../include/model/Path.hpp"

namespace trafficsimulation::model
{

Pedestrian::Pedestrian(const std::shared_ptr<Path> pavement, const uint32_t maxSpeed)
    : MovingObject{}
    , pavement_{pavement}
    , maxSpeed_{maxSpeed}
{
}

Pedestrian::~Pedestrian() = default;

void Pedestrian::move()
{
    auto step = maxSpeed_ - std::rand() % (maxSpeed_ / 3);

    doStep(step);
    position_ = pavement_->calculateNewPosition(distanceTravelled_);
}

void Pedestrian::doStep(uint32_t step)
{
    if(pavement_->getLength() >= distanceTravelled_ + step)
    {
        distanceTravelled_ += step;
        return;
    }
    if(pavement_->getJunction()->isGreenLight(pavement_->getPathId()))
    {
        step = distanceTravelled_ + step - pavement_->getLength();
        position_ = pavement_->calculateNewPosition(pavement_->getLength());

        selectNewPath();
        doStep(step);
        return;
    }
    distanceTravelled_ = pavement_->getLength();
}

void Pedestrian::selectNewPath()
{
    distanceTravelled_ = 0;
    const auto junction = pavement_->getJunction();

    if(destinationId_ == junction->getId())
    {
        destinationId_ = NO_DESTINATION;
    }

    auto newPavement = std::shared_ptr<Path>{};
    if(destinationId_ != NO_DESTINATION)
    {
        newPavement = junction->getFastestPavement(destinationId_);
    }
    else
    {
        const auto& pavements = junction->getOutgoingPavements();
        newPavement = pavements[std::rand() % std::size(pavements)].lock();
    }

    if(newPavement->getStartPoint() == position_)
    {
        pavement_ = newPavement;
        return;
    }
    pavement_ = junction->createTemporaryPavement(
        pavement_->calculateNewPosition(pavement_->getLength()), newPavement);
}

} // trafficsimulation::model





