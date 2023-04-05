#ifndef ROADCONDITION_HPP
#define ROADCONDITION_HPP

#include <string>

namespace trafficsimulation::model
{

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
