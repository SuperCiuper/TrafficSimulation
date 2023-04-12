#ifndef LINEPAINTER_HPP
#define LINEPAINTER_HPP

#include "../common/Point.hpp"

namespace trafficsimulation::interface
{

/*!
 * \brief The LinePainter abstract class
 *
 * Has minimal interface that is required by model classes to paint line
 */
class LinePainter
{
public:
    virtual ~LinePainter() = default;

    /*!
     * \brief setPoints
     * \param common::Point
     * \param common::Point
     *
     * Sets start and end points of path that will be printed
     */
    virtual void setPoints(const common::Point startPoint, const common::Point endPoint) = 0;
    /*!
     * \brief paint
     *
     * Paints line
     */
    virtual void paint() = 0;

protected:
    LinePainter() = default;
};

} // trafficsimulation::interface

#endif // LINEPAINTER_HPP
