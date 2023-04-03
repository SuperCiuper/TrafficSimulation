#include "../../include/view/dialogs/CreateRoadDialog.hpp"
#include "ui_createroaddialog.h"

namespace trafficsimulation::view::dialogs
{

CreateRoadDialog::CreateRoadDialog(const std::vector<std::pair<uint32_t, common::Point>> junctions,
    QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::CreateRoadDialog}
{
    ui_->setupUi(this);
//    ui_->widthSpinBox->setMinimum(minValue);
//    ui_->widthSpinBox->setMaximum(maxWidth);

//    ui_->heightSpinBox->setMaximum(minValue);
//    ui_->heightSpinBox->setMaximum(maxHeight);
}

CreateRoadDialog::~CreateRoadDialog()
{
    delete ui_;
}

} // trafficsimulation::view::dialogs
