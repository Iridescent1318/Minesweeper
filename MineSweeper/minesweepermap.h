#ifndef MINESWEEPERMAP_H
#define MINESWEEPERMAP_H

#include "gamemode_def.h"
#include "block.h"
#include "timer.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <QDebug>
#include <QString>

class MineSweeperMap: public QObject
{
    Q_OBJECT
public:
    MineSweeperMap(QMainWindow *main_window, GameMode game_mode, int map_width=10, int map_height=10, int num_of_mines=10);
    void SetMainWindow(QMainWindow *main_window) {main_window_ = main_window;}
    int GetMapWidth() const {return map_width_;}
    int GetMapHeight() const {return map_height_;}
    void CountMinesAround();
    void Reset();
    ~MineSweeperMap();
signals:
    void SendMapBegin();
    void SendMapOver();
    void SendMineBoomed();
    void SendMineAllFound();
    void SendMapWin();
    void SendTime(GameMode game_mode, int record_time);
public slots:
    void MapBegin(Block*);
    void LeftClickOnBlock(Block*);
    void RightClickOnBlock(Block*);
    void MapOver();
    void MapWin();
protected:
    QMainWindow *main_window_;
    GameMode game_mode_;
    int map_width_;
    int map_height_;
    int num_of_mines_;
    int opened_blocks_;
    bool game_start_;
    bool is_over_;
    Block* map_;
    Timer game_timer_;
};

#endif // MINESWEEPERMAP_H
