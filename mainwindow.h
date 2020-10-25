#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define screen_w 800
#define screen_h 800
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_NewGameButton_clicked();

    void on_actionNew_Game_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
