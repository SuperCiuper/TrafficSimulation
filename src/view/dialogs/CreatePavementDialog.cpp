#include "../../include/view/dialogs/CreatePavementDialog.hpp"
#include "ui_createpavementdialog.h"

namespace trafficsimulation::view::dialogs
{

CreatePavementDialog::CreatePavementDialog(const std::vector<std::pair<uint32_t, common::Point>> junctions,
    QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::CreatePavementDialog}
{
    ui_->setupUi(this);
//    ui_->widthSpinBox->setMinimum(minValue);
//    ui_->widthSpinBox->setMaximum(maxWidth);

//    ui_->heightSpinBox->setMaximum(minValue);
//    ui_->heightSpinBox->setMaximum(maxHeight);
}

CreatePavementDialog::~CreatePavementDialog()
{
    delete ui_;
}

} // trafficsimulation::view::dialogs
