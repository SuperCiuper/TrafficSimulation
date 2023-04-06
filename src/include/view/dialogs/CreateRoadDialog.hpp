#ifndef CREATEROADDIALOG_HPP
#define CREATEROADDIALOG_HPP

#include <QDialog>

#include "NotConnectedJunction.hpp"

namespace Ui { class CreateRoadDialog; }
namespace trafficsimulation::model{ enum class RoadCondition; }

namespace trafficsimulation::view::dialogs
{

class CreateRoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoadDialog(const std::vector<NotConnectedJunction> notConnectedJunctions,
        QWidget *parent);
    ~CreateRoadDialog();

    bool bothDirections() const;

    uint32_t getStartId() const;
    uint32_t getEndId() const;
    uint32_t getLength() const;
    model::RoadCondition getRoadCondition() const;
    uint32_t getSpeedLimit() const;

private:
    void handleStartJunctionChange(int startJunctionId);

    Ui::CreateRoadDialog *ui_;
    const std::vector<NotConnectedJunction> notConnectedJunctions_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEROADDIALOG_HPP
