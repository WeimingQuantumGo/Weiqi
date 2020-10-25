#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::GameWindow),
    absolute_coordinate_(-1, -1), mapdisplaymode_(default_display_mode), window_size_(-1, -1)
{
    ui->setupUi(this);
    this->setWindowTitle("Weiming Quantum Go");
    StartGame();
    set_window_size();
    permanent = new QLabel("", this);
    permanent->setFrameStyle(QFrame::Sunken);
    ui->statusbar->addPermanentWidget(permanent);
}

GameWindow::~GameWindow() {
    delete game_;
    delete ui;
}

void GameWindow::paintEvent(QPaintEvent *event) {
    QPalette palette;
    palette.setColor(QPalette::Background, color_background);
    setPalette(palette);

    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);

    //Debug
//    ShowTemporaryInfo(std::to_string(absolute_coordinate_.y()).data(), 1000);
    DrawMap(game_->round_[game_->current_round_id_]);
}

void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    absolute_coordinate_ = coordinate(event->x(), event->y());
    auto relative_coord = get_relative_coordinate(get_chessboard_count(), absolute_coordinate_);
    if (relative_coord.first < 0 || relative_coord.first >= get_chessboard_count())
        return;
//    if (relative_coord.second.x() >= board_size || relative_coord.second.y() >= board_size)
//        return;
    update();
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        auto relative_coord = get_relative_coordinate(get_chessboard_count(), absolute_coordinate_);
        if (mapdisplaymode_ == MapDisplayMode::ONEBOARD)
            relative_coord = get_relative_coordinate(1, absolute_coordinate_);
        game_->round_[game_->current_round_id_]->set_activate_chessboard_index(relative_coord.first);
        auto pos = relative_coord.second;
        if (pos.x() == 19 || pos.y() == 19)
            SwitchDisplayMode();
        else
            game_->Click(pos, event->button() == Qt::RightButton);
        if (game_->FinishRound()) {
            int prev_count = get_chessboard_count();
            game_->NextRound();
            int curr_count = get_chessboard_count();
            if (prev_count != curr_count)
                set_window_size();
        }
        ShowPermanentInfo(game_->round_[game_->current_round_id_]->drop_mode_);
        update();
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event) {}

void GameWindow::ShowPermanentInfo(const DropMode &mode) {
    if (mode == DropMode::CLASSIC)
        permanent->setText("Classic");
    if (mode == DropMode::SUPERPO)
        permanent->setText("Superposition");
    if (mode == DropMode::ENTANGLE)
        permanent->setText("Entangle");
    if (mode == DropMode::GAMEWISE)
        permanent->setText("Gamewise");
    if (mode == DropMode::UNDEFINED)
        permanent->setText("");
}

void GameWindow::ShowTemporaryInfo(const QString &info, const int &time) {
    ui->statusbar->showMessage(info, time);
}

void GameWindow::StartGame() {
    game_ = new Game(this, board_size, TakeMode::TAKEALL);
}

void GameWindow::SwitchDisplayMode() {
    if (mapdisplaymode_ == MapDisplayMode::GLOBAL)
        mapdisplaymode_ = MapDisplayMode::ONEBOARD;
    else if (mapdisplaymode_ == MapDisplayMode::ONEBOARD)
        mapdisplaymode_ = MapDisplayMode::GLOBAL;
    set_window_size();
    update();
}

int GameWindow::get_chessboard_count() {return game_->round_[game_->current_round_id_]->chessboard_group_.size();}

void GameWindow::set_window_size() {
    int display_chessboard_count = (mapdisplaymode_ == MapDisplayMode::ONEBOARD) ? 1 : get_chessboard_count();
    setFixedSize(game_size[display_chessboard_count].window_w, game_size[display_chessboard_count].window_h + 25);
}

std::pair<int, coordinate> GameWindow::get_relative_coordinate(const int &display_chessboard_count, const coordinate &absolute_coordinate) {
    const int count = display_chessboard_count;
    const int board_w = game_size[count].board_w;
    const int block_w = board_w / board_size;
    const int bound_w = game_size[count].bound_w;
    const coordinate coord = absolute_coordinate;
    const int total_w = board_w + 2 * bound_w;
    const int loc_x = coord.x() / total_w;
    const int loc_y = coord.y() / total_w;
    const int id = loc_y * game_size[count].board_x + loc_x;
    coordinate pos_in_board = (coord % total_w - coordinate(bound_w, bound_w) - coordinate(0, 30)/* if in windows */) / block_w;
    return std::make_pair(id, pos_in_board);
}

coordinate GameWindow::get_absolute_coordinate(const int &display_chessboard_count, const int &id, const coordinate &relative_coordinate) {
    const int count = display_chessboard_count;
    const int loc_x = id % game_size[count].board_x;
    const int loc_y = id / game_size[count].board_x;
    const int board_w = game_size[count].board_w;
    const int bound_w = game_size[count].bound_w;
    const int total_w = board_w + 2 * bound_w;
    coordinate pos_in_board = (relative_coordinate * board_w + coordinate(board_w / 2, board_w / 2)) / board_size;
    return coordinate(total_w, 0) * loc_x + coordinate(0, total_w) * loc_y + coordinate(bound_w, bound_w) + coordinate(0, 30)/* if in windows */ + pos_in_board;
}

void GameWindow::DrawMap(ChessBoardGroup *group) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen = painter.pen();
    pen.setColor(color_boundary);
    pen.setWidth(3);
    painter.setPen(pen);
    int count = group->chessboard_group_.size();
    if (mapdisplaymode_ == MapDisplayMode::GLOBAL) {
        QBrush brush;
        int board_w = game_size[count].board_w, bound_w = game_size[count].bound_w;
        int block_w = board_w / board_size;
        int total_w = board_w + 2 * bound_w;
        for (int i = 0; i < count; ++i) {
            int loc_x = i % game_size[count].board_x;
            int loc_y = i / game_size[count].board_x;
            pen.setColor(color_boundary);
            pen.setWidth(5);
            painter.setPen(pen);
            coordinate start_point = coordinate(total_w, 0) * loc_x + coordinate(0, total_w) * loc_y + coordinate(bound_w, bound_w) + coordinate(0, 30)/* if in windows */;
            brush.setColor(color_board);
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawRect(start_point.x(), start_point.y(), board_w, board_w);
            /* draw lines */
            pen.setColor(color_black);
            pen.setWidth(1);
            painter.setPen(pen);
            for (int j = 0; j < board_size; ++j) {
                coordinate p1 = get_absolute_coordinate(count, i, {0, j});
                coordinate p2 = get_absolute_coordinate(count, i, {board_size - 1, j});
                coordinate q1 = get_absolute_coordinate(count, i, {j, 0});
                coordinate q2 = get_absolute_coordinate(count, i, {j, board_size - 1});
                painter.drawLine(p1.x(), p1.y(), p2.x(), p2.y());
                painter.drawLine(q1.x(), q1.y(), q2.x(), q2.y());
            }
            /* draw black points */
            brush.setColor(color_black);
            painter.setBrush(brush);
            std::vector<coordinate> black_points = {coordinate(3, 3), coordinate(9, 9), coordinate(3, 15), coordinate(15, 3), coordinate(15, 15)};
            for (coordinate b : black_points) {
                coordinate p = get_absolute_coordinate(count, i, b);
                int half_w = block_w / 8;
                painter.drawRect(p.x() - half_w, p.y() - half_w, half_w * 2, half_w * 2);
            }
            /* draw chess */
            auto chessboard = game_->round_[game_->current_round_id_]->chessboard_group_[i];
            for (int j = 0; j < board_size; ++j)
                for (int k = 0; k < board_size; ++k) {
                    auto chess = chessboard->board_[j][k];
                    if (chess != nullptr)
                        DrawChess(count, i, ChessWithPos(chess, {j, k}), &painter);
                }
            auto dropped = game_->round_[game_->current_round_id_]->dropped_chess_group_;
            for (auto chess_pack : dropped)
                if ((chess_pack.first >> i) & 1) DrawChess(count, i, chess_pack.second, &painter);
            QFont font;
            font.setPixelSize(game_size[count].board_w / 50);
            int half_w = game_size[count].board_w / 40;
            auto absolute_coord = get_absolute_coordinate(count, i, coordinate(18, 18)) + coordinate(block_w / 2, block_w / 2);
            painter.setPen(color_black);
            painter.drawText(QRectF(absolute_coord.x() - half_w, absolute_coord.y() - half_w, 2 * half_w, 2 * half_w),
                              Qt::AlignCenter, static_cast<QString>(chessboard->board_weight_.Reduce())
                             );
        }
        /* draw red rectangle */
        pen.setColor(color_red);
        pen.setWidth(1);
        painter.setPen(pen);
        auto relative_coord = get_relative_coordinate(count, absolute_coordinate_);
        if (relative_coord.second.x() < board_size && relative_coord.second.y() < board_size)
            for (int k = 0; k < count; ++k) {
                point p = get_absolute_coordinate(count, k, relative_coord.second);
                int half_w = block_w / 2;
                for (int i = -half_w; i <= half_w; i += half_w * 2)
                    for (int j = -half_w; j <= half_w; j += half_w * 2) {
                        painter.drawLine(p.x() + i, p.y() + j, p.x() + i / 2, p.y() + j);
                        painter.drawLine(p.x() + i, p.y() + j, p.x() + i, p.y() + j / 2);
                    }
            }
        else {
            pen.setWidth(3);
            painter.setPen(pen);
            auto left_top = get_absolute_coordinate(count, relative_coord.first, {0, 0});
            int x1 = left_top.x(), y1 = left_top.y();
            auto right_bottom = get_absolute_coordinate(count, relative_coord.first, {19, 19});
            int x2 = right_bottom.x(), y2 = right_bottom.y();
            int d =  block_w / 2;
            painter.drawLine(x1 - d, y1 - d, x1 - d, y2 - d);
            painter.drawLine(x1 - d, y1 - d, x2 - d, y1 - d);
            painter.drawLine(x2 - d, y1 - d, x2 - d, y2 - d);
            painter.drawLine(x1 - d, y2 - d, x2 - d, y2 - d);
        }
    }
    if (mapdisplaymode_ == MapDisplayMode::ONEBOARD) {
        QBrush brush;
        int board_w = game_size[1].board_w, bound_w = game_size[1].bound_w;
        int block_w = board_w / board_size;
        int total_w = board_w + 2 * bound_w;
        pen.setColor(color_boundary);
        pen.setWidth(5);
        painter.setPen(pen);
        brush.setColor(color_board);
        brush.setStyle(Qt::SolidPattern);
        painter.setBrush(brush);
        painter.drawRect(bound_w, bound_w, board_w, board_w);
        /* draw lines */
        pen.setColor(color_black);
        pen.setWidth(1);
        painter.setPen(pen);
        for (int j = 0; j < board_size; ++j) {
            point p1 = get_absolute_coordinate(1, 0, {0, j});
            point p2 = get_absolute_coordinate(1, 0, {board_size - 1, j});
            point q1 = get_absolute_coordinate(1, 0, {j, 0});
            point q2 = get_absolute_coordinate(1, 0, {j, board_size - 1});
            painter.drawLine(p1.x(), p1.y(), p2.x(), p2.y());
            painter.drawLine(q1.x(), q1.y(), q2.x(), q2.y());
        }
        /* draw black points */
        brush.setColor(color_black);
        painter.setBrush(brush);
        std::vector<coordinate> black_points = {coordinate(3, 3), coordinate(9, 9), coordinate(3, 15), coordinate(15, 3), coordinate(15, 15)};
        for (coordinate b : black_points) {
            coordinate p = get_absolute_coordinate(1, 0, b);
            int half_w = block_w / 8;
            painter.drawRect(p.x() - half_w, p.y() - half_w, half_w * 2, half_w * 2);
        }
        /* draw chess */
        auto index = game_->round_[game_->current_round_id_]->activate_chessboard_index_;
        auto chessboard = game_->round_[game_->current_round_id_]->chessboard_group_[index];
        for (int j = 0; j < board_size; ++j)
            for (int k = 0; k < board_size; ++k) {
                auto chess = chessboard->board_[j][k];
                if (chess != nullptr)
                    DrawChess(1, 0, ChessWithPos(chess, {j, k}), &painter);
            }
        auto dropped = game_->round_[game_->current_round_id_]->dropped_chess_group_;
        for (auto chess_pack : dropped)
            if ((chess_pack.first >> index) & 1) DrawChess(1, 0, chess_pack.second, &painter);
        /* draw red rectangle */
        pen.setColor(color_red);
        pen.setWidth(1);
        painter.setPen(pen);
        auto relative_coord = get_relative_coordinate(1, absolute_coordinate_);
        point p = get_absolute_coordinate(1, 0, relative_coord.second);
        int half_w = block_w / 2;
        if (relative_coord.second.x() < board_size && relative_coord.second.y() < board_size)
            for (int i = -half_w; i <= half_w; i += half_w * 2)
                for (int j = -half_w; j <= half_w; j += half_w * 2) {
                    painter.drawLine(p.x() + i, p.y() + j, p.x() + i / 2, p.y() + j);
                    painter.drawLine(p.x() + i, p.y() + j, p.x() + i, p.y() + j / 2);
                 }
        }
}

void GameWindow::DrawChess(const int &display_chessboard_count, const int &id, const ChessWithPos &chess_with_pos, QPainter *painter) {
    int count = display_chessboard_count;
    auto absolute_coord = get_absolute_coordinate(count, id, chess_with_pos.pos_);
    auto chess = chess_with_pos.chess_;
    int block_w = game_size[count].board_w / board_size;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if (chess->player() == Player::WHITE) brush.setColor(color_white), painter->setPen(color_white);
    if (chess->player() == Player::BLACK) brush.setColor(color_black), painter->setPen(color_black);
    painter->setBrush(brush);
    painter->drawEllipse(QPoint(absolute_coord.x(), absolute_coord.y()), block_w / 3, block_w / 3);
    if (chess->player() == Player::WHITE) painter->setPen(color_black);
    if (chess->player() == Player::BLACK) painter->setPen(color_white);
    QFont font;
    font.setPixelSize(game_size[count].board_w / 50);
    int half_w = game_size[count].board_w / 40;
    painter->drawText(QRectF(absolute_coord.x() - half_w, absolute_coord.y() - half_w, 2 * half_w, 2 * half_w),
                      Qt::AlignCenter, static_cast<QString>(*chess));
}

void GameWindow::on_actionwithdraw_triggered()
{
    if (game_->round_.size() == 1)
        return;
    ShowTemporaryInfo("Withdraw");
    game_->PrevRound();
    set_window_size();
    ShowPermanentInfo(game_->round_[game_->current_round_id_]->drop_mode_);
    update();
}

void GameWindow::on_actiongamewise_on_triggered()
{
    auto &round = game_->round_[game_->current_round_id_];
    auto &dropmode = round->drop_mode_;
    if (dropmode == DropMode::GAMEWISE) {
        round->reset();
        dropmode = DropMode::UNDEFINED;
        ui->menubar->actions()[0]->menu()->actions()[1]->setText("gamewise on");
    } else {
        round->reset();
        dropmode = DropMode::GAMEWISE;
        ui->menubar->actions()[0]->menu()->actions()[1]->setText("gamewise off");
    }
    ShowPermanentInfo(dropmode);
    update();
}

void GameWindow::on_actionHow_to_play_triggered()
{
    HelpForm * helpForm = new HelpForm();
    helpForm->show();
}

void GameWindow::on_actionWeiming_Quantum_Go_Rules_triggered()
{
    RuleForm * ruleForm = new RuleForm();
    ruleForm->show();
}
