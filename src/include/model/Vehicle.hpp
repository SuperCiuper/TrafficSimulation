#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <memory>

namespace trafficsimulation::model
{

class Driver;

/*!
 * \brief The Vehicle class
 *
 * Holds pointers to vehicle ahead and behind, is frien of Driver class.
 */
class Vehicle
{
public:
    /*!
     * \brief Constructor
     * \param uint32_t maxAcceleration
     * \param uint32_t maxDeceleration
     */
    Vehicle(const uint32_t maxAcceleration, const uint32_t maxDeceleration);
    ~Vehicle();

    void setVehicleAhead(Vehicle* const vehicleAhead);
    void setVehicleBehind(Vehicle* const vehicleBehind);

private:
    const uint32_t maxAcceleration_; /* 5 - 8 */
    const uint32_t maxDeceleration_; /* 14 - 24 */
    uint32_t speed_;
    uint32_t distanceTravelled_;
    Vehicle* vehicleAhead_;
    Vehicle* vehicleBehind_;

    friend class Driver;
};

} // trafficsimulation::model

#endif // VEHICLE_HPP
