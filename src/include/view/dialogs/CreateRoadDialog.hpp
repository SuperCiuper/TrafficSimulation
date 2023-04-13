#ifndef CREATEROADDIALOG_HPP
#define CREATEROADDIALOG_HPP

#include <QDialog>

#include "NotConnectedJunction.hpp"

namespace Ui { class CreateRoadDialog; }
namespace trafficsimulation::model{ enum class RoadCondition; }

namespace trafficsimulation::view::dialogs
{

/*!
 * \brief The CreateRoadDialog class
 *
 * Inherits QDialog class.
 * Creates dialog window with input widgets, other classes (controlers) use it to
 * collect values to add Road to simulation.
 */
class CreateRoadDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief CreatePavementDialog
     * \param std::vector<NotConnectedJunction> notConnectedJunctions
     * \param QWidget* parent
     *
     * Sets items in ComboBoxes using notConnectedJunctions.
     * Sets ComboBox index change handling
     */
    explicit CreateRoadDialog(const std::vector<NotConnectedJunction> notConnectedJunctions,
        QWidget *parent);
    ~CreateRoadDialog();

    uint32_t getStartId() const;
    uint32_t getEndId() const;
    uint32_t getLength() const;
    model::RoadCondition getRoadCondition() const;
    uint32_t getSpeedLimit() const;

    bool bothDirections() const;
    bool createPavement() const;

private:
    /*!
     * \brief handleStartJunctionChange
     * \param int startJunctionId
     *
     * Called why new item is chosen in startJunctionComboBox.
     * Sets items in endJunctionComboBox using notConnectedJunctions_.
     */
    void handleStartJunctionChange(int startJunctionId);

    Ui::CreateRoadDialog *ui_;
    const std::vector<NotConnectedJunction> notConnectedJunctions_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEROADDIALOG_HPP
