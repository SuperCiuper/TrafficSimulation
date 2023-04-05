#ifndef CREATEDRIVERDIALOG_HPP
#define CREATEDRIVERDIALOG_HPP

#include <QDialog>

namespace Ui { class CreateDriverDialog; }

namespace trafficsimulation::view::dialogs
{

class CreateDriverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDriverDialog(QWidget *parent);
    ~CreateDriverDialog();

private:
    Ui::CreateDriverDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEDRIVERDIALOG_HPP
