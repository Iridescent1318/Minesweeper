#ifndef GAME_H
#define GAME_H

#include "minesweepermap.h"
#include "recordlist.h"
#include <QDialog>
#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QKeyEvent>

class Game : public QMainWindow
{
    Q_OBJECT

public:
    Game( QWidget *parent );
    ~Game();
    void NewGame(GameMode game_mode);
    virtual void keyReleaseEvent(QKeyEvent *event);
signals:
    void SendNewRecordTime(GameMode game_mode, int record_time);
public slots:
    void GameOver();
    void GameWin();
    void TriggerMenu(QAction* act);
    void NewCustomGame();
    void RecordTime(GameMode game_mode, int record_time);
private:
    MineSweeperMap *ms_map_;
    QMessageBox show_game_over_;
    QMessageBox show_game_win_;
    QMenuBar menu_bar_;
    QMenu menu_[3];
    QAction menu_actions_[10];
    QDialog custom_dialog_;
    QLineEdit custom_set_[3];
    QLabel custom_label_[3];
    QPushButton custom_ok_;
    RecordList record_list_;
    QMessageBox msgbox_about_;
};


#endif // GAME_H
