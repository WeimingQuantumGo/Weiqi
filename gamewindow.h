#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include "game.h"
#include "helpform.h"
#include "ruleform.h"
#include "aboutform.h"
#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QTextBrowser>

enum MapDisplayMode {ONEBOARD, GLOBAL};

const int board_size = 19;
const MapDisplayMode default_display_mode = GLOBAL;

//const char * color_background = "#B1723C";
//const char * color_boundary = "#B8D582";
//const char * color_board = "#EEC085";
//const char * color_black = "#000000";
//const char * color_white = "#FFFFFF";
//const char * color_red = "#FF0000";

#define color_background "#B1723C"
#define color_boundary "#8D5822"
#define color_board "#EEC085"
#define color_black "#000000"
#define color_white "#FFFFFF"
#define color_red "#FF0000"

struct Size {
    int board_w;
    int bound_w;
    int window_w;
    int window_h;
    int board_x;
    int board_y;
};

const std::vector<Size> game_size = {
    {-1, -1, -1, -1, -1, -1},
    {760, 20, 800, 800, 1, 1},
    {760, 20, 1600, 800, 2, 1},
    {456, 12, 1440, 480, 3, 1},
    {456, 12, 960, 960, 2, 2},
    {456, 12, 1440, 960, 3, 2},
    {456, 12, 1440, 960, 3, 2},
    {380, 10, 1600, 800, 4, 2},
    {380, 10, 1600, 800, 4, 2}
};

/*
 * GameWindow class:
 *
 * Objects:
 * absolute_coordinate_:
 *     the coordinate of mouse
 * window_size_:
 *     current size of window(w, h)
 *
 * Methods:
 * StartGame():
 *     activate when construct GameWindow, start an empty chessboard(group)
 * get_relative_coordinate(chessboardgroup_size, absolute_coordinate):
 *     using window_size_ to determine the graphic and location of each chessboard,
 *     return the chessboard id & relative coordinate
 *     DEFAULT (-1, -1)
 * get_absolute_coordinate(chessboardgroup_size, id, relative_coordinate):
 *     inverse function of get_relative_coordinate, return the center of the point
 *
 * PS:
 * chessboardgroup map in MapDisplayMode::GLOBAL:
 *
 * | 1 | 2 |  3  | 4 |  6  |   8   |
 *   *  **   ***  **   ***   ****
 *                **   ***   ****
 */

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void ShowPermanentInfo(const DropMode &mode);
    void ShowTemporaryInfo(const QString &info, const int &time = 2000);

    void StartGame();
    void SwitchDisplayMode();
    int get_chessboard_count();
    void set_window_size();
    std::pair<int, coordinate> get_relative_coordinate(const int &display_chessboard_count, const coordinate &absolute_coordinate);
    coordinate get_absolute_coordinate(const int &display_chessboard_count, const int &id, const coordinate &relative_coordinate);
    void DrawMap(ChessBoardGroup *group);
    void DrawChess(const int &display_chesssboard_count, const int &id, const ChessWithPos &chess_with_pos, QPainter *painter);
private slots:
    void on_actionwithdraw_triggered();

    void on_actiongamewise_on_triggered();

    void on_actionHow_to_play_triggered();

    void on_actionWeiming_Quantum_Go_Rules_triggered();

    void on_actionWeiming_Quantum_Go_triggered();

private:
    Ui::GameWindow *ui;
    QLabel *permanent;
    Game *game_;
    coordinate absolute_coordinate_;
    MapDisplayMode mapdisplaymode_;
    coordinate window_size_;


};

#endif // GAMEWINDOW_H
