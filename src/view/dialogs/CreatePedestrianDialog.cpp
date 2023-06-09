#include "../../include/view/dialogs/CreatePedestrianDialog.hpp"
#include "ui_createpedestriandialog.h"

namespace trafficsimulation::view::dialogs
{

CreatePedestrianDialog::CreatePedestrianDialog(QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::CreatePedestrianDialog}
{
    ui_->setupUi(this);

}

CreatePedestrianDialog::~CreatePedestrianDialog()
{
    delete ui_;
}

bool CreatePedestrianDialog::isRandomized() const
{
    return ui_->randomizePedestrianCheckBox->isChecked();
}

uint32_t CreatePedestrianDialog::getMaxSpeed() const
{
    return static_cast<uint32_t>(ui_->maxSpeedDoubleSpinBox->value() * 10);
}

} // trafficsimulation::view::dialogs
