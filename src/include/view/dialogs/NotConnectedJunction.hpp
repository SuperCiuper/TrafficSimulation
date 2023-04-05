#ifndef NOTCONNECTEDJUNCTION_HPP
#define NOTCONNECTEDJUNCTION_HPP

#include <vector>

#include "../../common/Point.hpp"

namespace trafficsimulation::view::dialogs
{

struct Junction
{
    uint32_t junctionId;
    common::Point position;
};

struct NotConnectedJunction
{
    Junction startJunction;
    std::vector<Junction> endJunctions;
};

}

#endif // NOTCONNECTEDJUNCTION_HPP
