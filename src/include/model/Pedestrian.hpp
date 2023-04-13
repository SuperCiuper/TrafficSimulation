#ifndef PEDESTRIAN_HPP
#define PEDESTRIAN_HPP

#include <memory>

#include "MovingObject.hpp"


namespace trafficsimulation::model
{

class Path;

/*!
 * \brief The Pedestrian class
 *
 * Inherits and extends MovingObject abstract class.
 * Calculates pedestrian behavior in simulation. Moves on Path objects
 */
class Pedestrian : public MovingObject
{
public:
    /*!
     * \brief Constructor
     * \param std::shared_ptr<Path> pavement
     * \param uint32_t maxSpeed
     */
    Pedestrian(const std::shared_ptr<Path> pavement, const uint32_t maxSpeed);
    virtual ~Pedestrian();

private:
    /*!
     * \brief move
     *
     * Overrides MovingObject::move()
     * Calculates distanceTravelled_ and position_
     */
    void move() override;
    /*!
     * \brief doStep
     * \param uint32_t step
     *
     * Increases distanceTravelled_ of Pedestrian by step or if Junction was
     * reached enters it or stops at the end of the pavement_
     */
    void doStep(uint32_t step);
    /*!
     * \brief selectNewPath
     *
     * Randomly selects Path from outgoing pavements from Juction that is at the end of
     * pavement_ or if destination is set selects fastest route
     */
    void selectNewPath();

    std::shared_ptr<Path> pavement_;
    const uint32_t maxSpeed_; /* 30 - 120 */
};

} // trafficsimulation::model


#endif // PEDESTRIAN_HPP
