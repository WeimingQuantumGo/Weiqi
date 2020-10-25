#include "mainwindow.h"
#include "gamewindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(screen_w, screen_h);
    this->setWindowTitle("Weiming Quantum Go");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_NewGameButton_clicked()
{
    this->close();
    GameWindow *gw = new GameWindow();
    gw->show();
}

void MainWindow::on_actionNew_Game_triggered()
{
    on_NewGameButton_clicked();
}
