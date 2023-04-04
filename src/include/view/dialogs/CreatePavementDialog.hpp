#ifndef CREATEPAVEMENTDIALOG_HPP
#define CREATEPAVEMENTDIALOG_HPP

#include <QDialog>

#include "../../common/Point.hpp"

namespace Ui { class CreatePavementDialog; }

namespace trafficsimulation::view::dialogs
{

struct Junction
{
    uint32_t junctionId;
    common::Point position;
};

struct NotConnectedJunction
{
    Junction startJunction;
    std::vector<Junction> endJunctions;
};

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

private:
    void handleStartJunctionChange(int startJunctionId);
    void handleOkClick();

    Ui::CreatePavementDialog *ui_;
    const std::vector<NotConnectedJunction> notConnectedJunctions_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEPAVEMENTDIALOG_HPP
