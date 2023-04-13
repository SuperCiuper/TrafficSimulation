#ifndef PATH_HPP
#define PATH_HPP

#include <memory>

#include "../common/Point.hpp"

namespace trafficsimulation::interface{ class LinePainter; }

namespace trafficsimulation::model
{

class Junction;

/*!
 * \brief The Path class
 *
 * Is basic path between Junction objects. Used by Pedestrian objects
 */
class Path
{
public:
    /*!
     * \brief Constructor
     * \param uint32_t pathId
     * \param uint32_t length
     * \param common::Point startPoint
     * \param common::Point endPoint
     * \param std::shared_ptr<Junction> endJunction
     */
    Path(const uint32_t pathId, const uint32_t length, const common::Point startPoint,
        const common::Point endPoint, const std::shared_ptr<Junction> endJunction);
    virtual ~Path();

    uint32_t getPathId() const;
    uint32_t getLength() const;
    common::Point getStartPoint() const;
    std::shared_ptr<Junction> getJunction() const;

    /*!
     * \brief calculateNewPosition
     * \param uint32_t distanceTravelled
     * \return common::Point - position on path
     *
     * Calculates position of object that travelled distanceTravelled on path
     */
    common::Point calculateNewPosition(uint32_t distanceTravelled) const;

    void setPainter(interface::LinePainter* const painter);
    /*!
     * \brief update
     *
     * If painter is set paints object
     */
    void update();

private:
    const uint32_t pathId_;
    const uint32_t length_;
    const common::Point startPoint_;
    const common::Point endPoint_;
    const common::Point shiftOfStartPoint_;
    const std::shared_ptr<Junction> endJunction_;

    interface::LinePainter* painter_;
};

} // trafficsimulation::model

#endif // PATH_HPP
