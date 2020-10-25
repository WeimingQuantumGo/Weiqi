#include "ruleform.h"
#include "ui_ruleform.h"

RuleForm::RuleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleForm)
{
    ui->setupUi(this);
    this->setFixedSize(800, 800);
    this->setWindowTitle("Weiming Quantum Go Rules");
}

RuleForm::~RuleForm()
{
    delete ui;
}
