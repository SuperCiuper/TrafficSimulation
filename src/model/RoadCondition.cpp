#include "../include/model/RoadCondition.hpp"

std::string toString(const trafficsimulation::model::RoadCondition& roadCondition)
{
    switch(roadCondition)
    {
        case trafficsimulation::model::RoadCondition::New: return "New";
        case trafficsimulation::model::RoadCondition::NoPotHoles: return "NoPotHoles";
        case trafficsimulation::model::RoadCondition::SomePotHoles: return "SomePotHoles";
        case trafficsimulation::model::RoadCondition::LotsOfPotHoles: return "LotsOfPotHoles";
        case trafficsimulation::model::RoadCondition::Offroad: return "Offroad";
        default: return "Out of scope";
    }
}
