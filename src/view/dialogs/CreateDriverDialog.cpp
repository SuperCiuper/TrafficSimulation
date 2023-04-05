#include "../../include/view/dialogs/CreateDriverDialog.hpp"
#include "ui_createdriverdialog.h"

namespace trafficsimulation::view::dialogs
{

CreateDriverDialog::CreateDriverDialog(QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::CreateDriverDialog}
{
    ui_->setupUi(this);
}

CreateDriverDialog::~CreateDriverDialog()
{
    delete ui_;
}




} // trafficsimulation::view::dialogs
