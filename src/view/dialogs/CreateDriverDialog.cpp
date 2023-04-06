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

bool CreateDriverDialog::isVehicleRandomized() const
{
    return ui_->randomizeVehicleCheckBox->isChecked();
}

uint32_t CreateDriverDialog::getMaxAcceleration() const
{
    return ui_->maxAccelerationSpinBox->value();
}

uint32_t CreateDriverDialog::getMaxDeceleration() const
{
    return ui_->maxDecelerationSpinBox->value();
}

bool CreateDriverDialog::isDriverRandomized() const
{
    return ui_->randomizeDriverCheckBox->isChecked();
}

float CreateDriverDialog::getAccelerationRate() const
{
    return static_cast<float>(ui_->accelerationRateDoubleSpinBox->value());
}

uint32_t CreateDriverDialog::getMinDistanceToVehicle() const
{
    return static_cast<uint32_t>(ui_->accelerationRateDoubleSpinBox->value() * 1000);
}

uint32_t CreateDriverDialog::getMaxSpeedOverLimit() const
{
    return ui_->maxSpeedOverLimitSpinBox->value();
}

float CreateDriverDialog::getRoadConditionSpeedModifier() const
{
    return static_cast<float>(ui_->roadConditionSpeedModifierDoubleSpinBox->value());
}

} // trafficsimulation::view::dialogs
