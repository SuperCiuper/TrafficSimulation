#ifndef CREATEPEDESTRIANDIALOG_HPP
#define CREATEPEDESTRIANDIALOG_HPP

#include <QDialog>

namespace Ui { class CreatePedestrianDialog; }

namespace trafficsimulation::view::dialogs
{

/*!
 * \brief The CreatePedestrianDialog class
 *
 * Inherits QDialog class.
 * Creates dialog window with input widgets, other classes (controlers) use it to
 * collect values to add Pedestrian to simulation
 */
class CreatePedestrianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePedestrianDialog(QWidget *parent);
    ~CreatePedestrianDialog();

    bool isRandomized() const;
    uint32_t getMaxSpeed() const;

private:
    Ui::CreatePedestrianDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEPEDESTRIANDIALOG_HPP
