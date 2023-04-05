#ifndef CREATEPEDESTRIANDIALOG_HPP
#define CREATEPEDESTRIANDIALOG_HPP

#include <QDialog>

namespace Ui { class CreatePedestrianDialog; }

namespace trafficsimulation::view::dialogs
{

class CreatePedestrianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePedestrianDialog(QWidget *parent);
    ~CreatePedestrianDialog();

    uint32_t getMaxSpeed() const;

private:
    Ui::CreatePedestrianDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEPEDESTRIANDIALOG_HPP
