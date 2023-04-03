#ifndef CREATEPAVEMENTDIALOG_HPP
#define CREATEPAVEMENTDIALOG_HPP

#include <QDialog>

#include "../../common/Point.hpp"

namespace Ui { class CreatePavementDialog; }

namespace trafficsimulation::view::dialogs
{

class CreatePavementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePavementDialog(const std::vector<std::pair<uint32_t, common::Point>> junctions,
        QWidget *parent);
    ~CreatePavementDialog();

    uint32_t getStartId() const;
    uint32_t getEndId() const;

private:
    Ui::CreatePavementDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEPAVEMENTDIALOG_HPP
