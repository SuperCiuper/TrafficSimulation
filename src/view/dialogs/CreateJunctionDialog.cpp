#include "../../include/view/dialogs/CreateJunctionDialog.hpp"
#include "ui_createjunctiondialog.h"

namespace trafficsimulation::view::dialogs
{

CreateJunctionDialog::CreateJunctionDialog(const uint32_t minValue, const uint32_t maxWidth,
    const uint32_t maxHeight, QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::CreateJunctionDialog}
{
    ui_->setupUi(this);
    ui_->widthSpinBox->setMinimum(minValue);
    ui_->widthSpinBox->setMaximum(maxWidth);
    ui_->widthSpinBox->setValue((minValue + maxWidth) / 2);

    ui_->heightSpinBox->setMaximum(minValue);
    ui_->heightSpinBox->setMaximum(maxHeight);
    ui_->heightSpinBox->setValue((minValue + maxHeight) / 2);
}

CreateJunctionDialog::~CreateJunctionDialog()
{
    delete ui_;
}

common::Point CreateJunctionDialog::getPoint() const
{
    return common::Point{static_cast<int16_t>(ui_->widthSpinBox->value()),
        static_cast<int16_t>(ui_->heightSpinBox->value())};
}

} // trafficsimulation::view::dialogs
