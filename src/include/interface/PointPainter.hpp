#ifndef POINTPAINTER_HPP
#define POINTPAINTER_HPP

#include "../common/Point.hpp"

namespace trafficsimulation::interface
{

/*!
 * \brief The PointPainter abstract class
 *
 * Has minimal interface that is required by model classes to paint point
 */
class PointPainter
{
public:
    virtual ~PointPainter() = default;

    /*!
     * \brief setPoints
     * \param common::Point
     *
     * Pure virtual function. Sets point that will be printed
     */
    virtual void setPoint(const common::Point point) = 0;
    /*!
     * \brief paint
     *
     * Pure virtual function. Paints point
     */
    virtual void paint() = 0;

protected:
    PointPainter() = default;
};

} // trafficsimulation::interface

#endif // POINTPAINTER_HPP
