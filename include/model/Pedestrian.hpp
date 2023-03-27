#ifndef PEDESTRIAN_HPP
#define PEDESTRIAN_HPP

#include <memory>

#include "MovingObject.hpp"
#include "Path.hpp"

class Pedestrian : public MovingObject
{
public:
    Pedestrian(const std::shared_ptr<Path> pavement);
    Pedestrian(const std::shared_ptr<Path> pavement, const uint32_t maxSpeed);
    virtual ~Pedestrian();

    void move() override;

private:
    void selectNewPath() override;

    const uint32_t maxSpeed_;
    std::shared_ptr<Path> pavement_;
};

#endif // PEDESTRIAN_HPP
