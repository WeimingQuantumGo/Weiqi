#include "helpform.h"
#include "ui_helpform.h"

HelpForm::HelpForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpForm)
{
    ui->setupUi(this);
    this->setFixedSize(800, 800);
    this->setWindowTitle("How To Play");
}

HelpForm::~HelpForm()
{
    delete ui;
}
