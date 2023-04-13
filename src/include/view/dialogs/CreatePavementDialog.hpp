#ifndef CREATEPAVEMENTDIALOG_HPP
#define CREATEPAVEMENTDIALOG_HPP

#include <QDialog>

#include "NotConnectedJunction.hpp"

namespace Ui { class CreatePavementDialog; }

namespace trafficsimulation::view::dialogs
{

/*!
 * \brief The CreatePavementDialog class
 *
 * Inherits QDialog class.
 * Creates dialog window with input widgets, other classes (controlers) use it to
 * collect values to add Pavement to simulation.
 */
class CreatePavementDialog : public QDialog
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
    explicit CreatePavementDialog(const std::vector<NotConnectedJunction> notConnectedJunctions,
        QWidget *parent);
    ~CreatePavementDialog();

    uint32_t getStartId() const;
    uint32_t getEndId() const;
    uint32_t getLength() const;

    bool bothDirections() const;

private:
    /*!
     * \brief handleStartJunctionChange
     * \param int startJunctionId
     *
     * Called why new item is chosen in startJunctionComboBox.
     * Sets items in endJunctionComboBox using notConnectedJunctions_.
     */
    void handleStartJunctionChange(int startJunctionId);

    Ui::CreatePavementDialog *ui_;
    const std::vector<NotConnectedJunction> notConnectedJunctions_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEPAVEMENTDIALOG_HPP
