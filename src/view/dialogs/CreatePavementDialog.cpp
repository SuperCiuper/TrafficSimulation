#include "../../include/view/dialogs/CreatePavementDialog.hpp"
#include "ui_createpavementdialog.h"

#include <QString>

namespace trafficsimulation::view::dialogs
{

CreatePavementDialog::CreatePavementDialog(const std::vector<NotConnectedJunction> notConnectedJunctions,
    QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::CreatePavementDialog}
    , notConnectedJunctions_{notConnectedJunctions}
{
    ui_->setupUi(this);

    connect(ui_->startJunctionComboBox, &QComboBox::currentIndexChanged,
        this, [this](int index){ handleStartJunctionChange(index); });

    for(const auto& notConnectedJunction : notConnectedJunctions_)
    {
        auto text = QString{};
        text.append("Position:  X: ").append(QString::number(notConnectedJunction.startJunction.position.x))
            .append(", Y: ").append(QString::number(notConnectedJunction.startJunction.position.y));

        ui_->startJunctionComboBox->addItem(text);
    }
    ui_->startJunctionComboBox->setCurrentIndex(0);
}

CreatePavementDialog::~CreatePavementDialog()
{
    delete ui_;
}

bool CreatePavementDialog::bothDirections() const
{
    return ui_->bothDirectionsCheckBox->isChecked();
}

uint32_t CreatePavementDialog::getStartId() const
{
    return notConnectedJunctions_[ui_->startJunctionComboBox->currentIndex()].startJunction.junctionId;
}

uint32_t CreatePavementDialog::getEndId() const
{
    return notConnectedJunctions_[ui_->startJunctionComboBox->currentIndex()]
        .endJunctions[ui_->endJunctionComboBox->currentIndex()].junctionId;
}

uint32_t CreatePavementDialog::getLength() const
{
    return static_cast<uint32_t>(ui_->lengthSpinBox->value()) * 1000;
}

void CreatePavementDialog::handleStartJunctionChange(int startJunctionId)
{
    ui_->endJunctionComboBox->clear();

    auto junctions = notConnectedJunctions_[ui_->startJunctionComboBox->currentIndex()].endJunctions;
    for(const auto& junction : junctions)
    {
        auto text = QString{};
        text.append("Position:  X: ").append(QString::number(junction.position.x))
            .append(", Y: ").append(QString::number(junction.position.y));

        ui_->endJunctionComboBox->addItem(text);
    }
    ui_->endJunctionComboBox->setCurrentIndex(0);
}

} // trafficsimulation::view::dialogs
