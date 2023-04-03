#ifndef CREATEJUNCTIONDIALOG_HPP
#define CREATEJUNCTIONDIALOG_HPP

#include <QDialog>

#include "../../common/Point.hpp"

namespace Ui { class CreateJunctionDialog; }

namespace trafficsimulation::view::dialogs
{

class CreateJunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateJunctionDialog(const uint32_t minValue, const uint32_t maxWidth,
        const uint32_t maxHeight, QWidget *parent = nullptr);
    ~CreateJunctionDialog();

    common::Point getPoint() const;

private:
    Ui::CreateJunctionDialog *ui_;
};

} // trafficsimulation::view::dialogs

#endif // CREATEJUNCTIONDIALOG_HPP
