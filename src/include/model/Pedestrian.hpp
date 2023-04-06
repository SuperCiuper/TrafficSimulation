#ifndef PEDESTRIAN_HPP
#define PEDESTRIAN_HPP

#include <memory>

#include "MovingObject.hpp"


namespace trafficsimulation::model
{

class Path;

class Pedestrian : public MovingObject
{
public:
    Pedestrian(const std::shared_ptr<Path> pavement, const uint32_t maxSpeed);
    virtual ~Pedestrian();

    void move() override;

private:
    void doStep(uint32_t step);
    void selectNewPath();

    std::shared_ptr<Path> pavement_;
    const uint32_t maxSpeed_; /* 30 - 120 */
};

} // trafficsimulation::model


#endif // PEDESTRIAN_HPP
