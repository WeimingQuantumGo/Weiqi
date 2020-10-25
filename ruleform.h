#ifndef RULEFORM_H
#define RULEFORM_H

#include <QWidget>

namespace Ui {
class RuleForm;
}

class RuleForm : public QWidget
{
    Q_OBJECT

public:
    explicit RuleForm(QWidget *parent = 0);
    ~RuleForm();

private:
    Ui::RuleForm *ui;
};

#endif // RULEFORM_H
