#ifndef HELPFORM_H
#define HELPFORM_H

#include <QWidget>

namespace Ui {
class HelpForm;
}

class HelpForm : public QWidget
{
    Q_OBJECT

public:
    explicit HelpForm(QWidget *parent = 0);
    ~HelpForm();

private:
    Ui::HelpForm *ui;
};

#endif // HELPFORM_H
