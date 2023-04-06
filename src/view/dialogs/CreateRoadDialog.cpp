#include "../../include/view/dialogs/CreateRoadDialog.hpp"
#include "ui_createroaddialog.h"

#include "../../include/model/RoadCondition.hpp"

namespace trafficsimulation::view::dialogs
{

CreateRoadDialog::CreateRoadDialog(const std::vector<NotConnectedJunction> notConnectedJunctions,
    QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::CreateRoadDialog}
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

    for(const auto& roadCondition : model::ALLROADCONDITIONS)
    {
        ui_->roadConditionComboBox->addItem(QString::fromStdString(toString(roadCondition)));
    }
    ui_->roadConditionComboBox->setCurrentIndex(0);
}

CreateRoadDialog::~CreateRoadDialog()
{
    delete ui_;
}

bool CreateRoadDialog::bothDirections() const
{
    return ui_->bothDirectionsCheckBox->isChecked();
}

bool CreateRoadDialog::createPavement() const
{
    return ui_->createCorrespondingPavementCheckBox->isChecked();
}

uint32_t CreateRoadDialog::getStartId() const
{
    return notConnectedJunctions_[ui_->startJunctionComboBox->currentIndex()].startJunction.junctionId;
}

uint32_t CreateRoadDialog::getEndId() const
{
    return notConnectedJunctions_[ui_->startJunctionComboBox->currentIndex()]
        .endJunctions[ui_->endJunctionComboBox->currentIndex()].junctionId;
}

uint32_t CreateRoadDialog::getLength() const
{
    return static_cast<uint32_t>(ui_->lengthSpinBox->value()) * 1000;
}

model::RoadCondition CreateRoadDialog::getRoadCondition() const
{
    return model::RoadCondition{ui_->roadConditionComboBox->currentIndex()};
}

uint32_t CreateRoadDialog::getSpeedLimit() const
{
    return static_cast<uint32_t>(ui_->speedLimitSpinBox->value()) * 10;
}

void CreateRoadDialog::handleStartJunctionChange(int startJunctionId)
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
