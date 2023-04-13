#ifndef CREATEJUNCTIONDIALOG_HPP
#define CREATEJUNCTIONDIALOG_HPP

#include <QDialog>

#include "../../common/Point.hpp"

namespace Ui { class CreateJunctionDialog; }

namespace trafficsimulation::view::dialogs
{

/*!
 * \brief The CreateJunctionDialog class
 *
 * Inherits QDialog class.
 * Creates dialog window with input widgets, other classes (controlers) use it to
 * collect values to add Junction to simulation
 */
class CreateJunctionDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief CreateJunctionDialog
     * \param uint32_t minValue
     * \param uint32_t maxWidth
     * \param uint32_t maxHeight
     * \param QWidget* parent
     *
     * Sets minimum and maximum value of SpinBoxes and sets random value from that range.
     */
    explicit CreateJunctionDialog(const uint32_t minValue, const uint32_t maxWidth,
        const uint32_t maxHeight, QWidget *parent = nullptr);
    ~CreateJunctionDialog();

    common::Point getPoint() const;

private:
    Ui::CreateJunctionDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEJUNCTIONDIALOG_HPP
