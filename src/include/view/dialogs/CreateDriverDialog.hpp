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

    bool isVehicleRandomized() const;
    uint32_t getMaxAcceleration() const;
    uint32_t getMaxDeceleration() const;

    bool isDriverRandomized() const;
    float getAccelerationRate() const;
    uint32_t getMinDistanceToVehicle() const;
    uint32_t getMaxSpeedOverLimit() const;
    float getRoadConditionSpeedModifier() const;

private:
    Ui::CreateDriverDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEDRIVERDIALOG_HPP
