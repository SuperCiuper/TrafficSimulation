#ifndef CREATEROADDIALOG_HPP
#define CREATEROADDIALOG_HPP

#include <QDialog>

#include "../../common/Point.hpp"

namespace Ui { class CreateRoadDialog; }

namespace trafficsimulation::view::dialogs
{

class CreateRoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoadDialog(const std::vector<std::pair<uint32_t, common::Point>> junctions,
        QWidget *parent);
    ~CreateRoadDialog();

    uint32_t getStartId() const;
    uint32_t getEndId() const;

private:
    Ui::CreateRoadDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEROADDIALOG_HPP
