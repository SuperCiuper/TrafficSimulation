#ifndef ROADCONDITION_HPP
#define ROADCONDITION_HPP

#include <string>

namespace trafficsimulation::model
{

/*!
 * \brief The RoadCondition enum
 *
 * Possible road conditions, used to calculate max speed that driver can go with on road
 */
enum class RoadCondition
{
    New,
    NoPotHoles,
    SomePotHoles,
    LotsOfPotHoles,
    Offroad
};

constexpr std::initializer_list<RoadCondition> ALLROADCONDITIONS = {RoadCondition::New,
    RoadCondition::NoPotHoles, RoadCondition::SomePotHoles, RoadCondition::LotsOfPotHoles,
    RoadCondition::Offroad};

} // trafficsimulation::model

std::string toString(const trafficsimulation::model::RoadCondition& roadCondition);

#endif // ROADCONDITION_HPP
