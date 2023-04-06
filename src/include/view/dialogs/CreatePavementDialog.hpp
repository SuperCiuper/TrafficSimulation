#ifndef CREATEPAVEMENTDIALOG_HPP
#define CREATEPAVEMENTDIALOG_HPP

#include <QDialog>

#include "NotConnectedJunction.hpp"

namespace Ui { class CreatePavementDialog; }

namespace trafficsimulation::view::dialogs
{

class CreatePavementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePavementDialog(const std::vector<NotConnectedJunction> notConnectedJunctions,
        QWidget *parent);
    ~CreatePavementDialog();

    uint32_t getStartId() const;
    uint32_t getEndId() const;
    uint32_t getLength() const;

    bool bothDirections() const;

private:
    void handleStartJunctionChange(int startJunctionId);

    Ui::CreatePavementDialog *ui_;
    const std::vector<NotConnectedJunction> notConnectedJunctions_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEPAVEMENTDIALOG_HPP
