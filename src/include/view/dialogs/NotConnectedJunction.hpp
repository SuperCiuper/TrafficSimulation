#ifndef NOTCONNECTEDJUNCTION_HPP
#define NOTCONNECTEDJUNCTION_HPP

#include <vector>

#include "../../common/Point.hpp"

namespace trafficsimulation::view::dialogs
{

/*!
 * \brief The Junction struct
 *
 * Used by view (dialog) classes to define passed values easier
 */
struct Junction
{
    uint32_t junctionId;
    common::Point position;
};

/*!
 * \brief The Junction struct
 *
 * Used by view (dialog) classes to define passed values easier
 */
struct NotConnectedJunction
{
    Junction startJunction;
    std::vector<Junction> endJunctions;
};

}

#endif // NOTCONNECTEDJUNCTION_HPP
