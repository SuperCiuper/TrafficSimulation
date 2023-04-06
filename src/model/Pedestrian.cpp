#include "../include/model/Pedestrian.hpp"

#include <cstdlib>
#include <ctime>

#include "../include/model/Junction.hpp"
#include "../include/model/Path.hpp"

#include <iostream>

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
    std::cout<< " move " << std::endl;
    auto step = maxSpeed_ - std::rand() % (maxSpeed_ / 3);

    doStep(step);
    position_ = pavement_->calculateNewPosition(distanceTravelled_);
}

void Pedestrian::doStep(uint32_t step)
{
    std::cout<< " doStep " << std::endl;
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
    std::cout<< " selectNewPath 1" << std::endl;

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
        std::cout<< " selectNewPath 1.1" << std::endl;
        const auto& pavements = junction->getOutgoingPavements();
        std::cout<< std::size(pavements) << std::endl;
        newPavement = pavements[std::rand() % std::size(pavements)];
        std::cout<< " selectNewPath 1.2" << std::endl;
    }
    std::cout<< " selectNewPath 2" << std::endl;

    if(newPavement->getStartPoint() == position_)
    {
        pavement_ = newPavement;
        return;
    }
    std::cout<< " selectNewPath 3" << std::endl;
    pavement_ = junction->createTemporaryPavement(
        pavement_->calculateNewPosition(pavement_->getLength()), newPavement);
    std::cout<< " selectNewPath 4" << std::endl;
}

} // trafficsimulation::model





