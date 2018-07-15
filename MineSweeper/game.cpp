#include "game.h"

Game::Game(QWidget *parent = 0 ) : QMainWindow(parent) {
    parent = NULL;
    show_game_over_.setWindowTitle("What a pity...");
    show_game_over_.setText("You lose the game.\nGood luck next time!");
    show_game_over_.setFixedSize(500,400);

    show_game_win_.setWindowTitle("Congratulations!");
    show_game_win_.setText("You have won the game!");
    show_game_win_.setFixedSize(500,400);

    custom_dialog_.setFixedSize(300,250);
    custom_dialog_.setWindowTitle("Custom game");
    custom_label_[0].setText("Width:(10-99)");
    custom_label_[1].setText("Height:(10-99)");
    custom_label_[2].setText("Mines:(10-999)");
    for(int i = 0; i < 3; i ++){
        custom_set_[i].setParent(&custom_dialog_);
        custom_label_[i].setParent(&custom_dialog_);
        custom_set_[i].move(180,30+i*60);
        custom_set_[i].setFixedSize(50,25);
        custom_set_[i].show();
        custom_label_[i].move(35,30+i*60);
        custom_label_[i].setFixedSize(120,25);
        custom_label_[i].show();
    }
    custom_ok_.setParent(&custom_dialog_);
    custom_ok_.setFixedSize(50,25);
    custom_ok_.move(125,202);
    custom_ok_.setText("OK");
    custom_ok_.show();
    QObject::connect(&custom_ok_, SIGNAL(clicked()), &custom_dialog_, SLOT(close()));
    QObject::connect(&custom_ok_, SIGNAL(clicked()), this, SLOT(NewCustomGame()));

    menu_bar_.setParent(this);
    menu_bar_.setGeometry(0,0,1000,30);

    menu_[0].setTitle("Game");
    menu_[1].setTitle("New game");
    menu_[2].setTitle("Help");

    menu_actions_[0].setParent(this);
    menu_actions_[0].setText("Quit");
    menu_actions_[0].setShortcut(tr("Ctrl+Q"));
    menu_actions_[1].setParent(this);
    menu_actions_[1].setText("Easy");
    menu_actions_[1].setShortcut(tr("Ctrl+E"));
    menu_actions_[2].setParent(this);
    menu_actions_[2].setText("Normal");
    menu_actions_[2].setShortcut(tr("Ctrl+N"));
    menu_actions_[3].setParent(this);
    menu_actions_[3].setText("Hard");
    menu_actions_[3].setShortcut(tr("Ctrl+H"));
    menu_actions_[4].setParent(this);
    menu_actions_[4].setText("Custom");
    menu_actions_[4].setShortcut(tr("Ctrl+C"));
    menu_actions_[5].setParent(this);
    menu_actions_[5].setText("About");
    menu_actions_[5].setShortcut(tr("F1"));
    menu_actions_[6].setParent(this);
    menu_actions_[6].setText("Restart");
    menu_actions_[6].setShortcut(tr("F5"));
    menu_actions_[7].setParent(this);
    menu_actions_[7].setText("Records");
    menu_actions_[7].setShortcut(tr("F3"));

    menu_[0].addMenu(&menu_[1]);
    menu_[0].addAction(&menu_actions_[6]);
    menu_[0].addAction(&menu_actions_[7]);
    menu_[0].addAction(&menu_actions_[0]);
    menu_[1].addAction(&menu_actions_[1]);
    menu_[1].addAction(&menu_actions_[2]);
    menu_[1].addAction(&menu_actions_[3]);
    menu_[1].addAction(&menu_actions_[4]);
    menu_[2].addAction(&menu_actions_[5]);

    menu_bar_.addMenu(&menu_[0]);
    menu_bar_.addMenu(&menu_[2]);

    QObject::connect(&menu_bar_, SIGNAL(triggered(QAction*)), this, SLOT(TriggerMenu(QAction*)));

    msgbox_about_.setWindowTitle("About");
    msgbox_about_.setText("Made by Iridescent1318\n"
                         "Github:  https://github.com/Iridescent1318");

    this->grabKeyboard();
}

void Game::GameOver(){
    show_game_over_.exec();
}

void Game::GameWin(){
    show_game_win_.exec();
}

void Game::NewGame(GameMode game_mode){
    switch(game_mode){
    case EASY:
        ms_map_ = new MineSweeperMap(this, EASY, 10, 10, 10);
        this->setFixedSize(420,420);
        break;
    case NORMAL:
        ms_map_ = new MineSweeperMap(this, NORMAL, 16, 16, 40);
        this->setFixedSize(600,600);
        break;
    case HARD:
        ms_map_ = new MineSweeperMap(this, HARD, 30, 16, 99);
        this->setFixedSize(1020,600);
        break;
    default: ;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event){
    if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Q))
        this->close();
    if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_E)){
        delete this->ms_map_;
        this->NewGame(EASY);
    }
    if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_N)){
        delete this->ms_map_;
        this->NewGame(NORMAL);
    }
    if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_H)){
        delete this->ms_map_;
        this->NewGame(HARD);
    }
    if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_H)){
        custom_dialog_.exec();
    }
    if(event->key() == Qt::Key_F5)
        ms_map_->Reset();
    if(event->key() == Qt::Key_F3){
        record_list_.Read();
        record_list_.show();
    }
    if(event->key() == Qt::Key_F1){
        msgbox_about_.exec();
    }
}

void Game::TriggerMenu(QAction* act){
    if(act->text() == "Quit"){
        this->close();
    }
    if(act->text() == "Easy"){
        delete this->ms_map_;
        this->NewGame(EASY);
    }
    if(act->text() == "Normal"){
        delete this->ms_map_;
        this->NewGame(NORMAL);
    }
    if(act->text() == "Hard"){
        delete this->ms_map_;
        this->NewGame(HARD);
    }
    if(act->text() == "Custom"){
        custom_dialog_.exec();
    }
    if(act->text() == "Restart"){
        ms_map_->Reset();
    }
    if(act->text() == "Records"){
        record_list_.Read();
        record_list_.show();
    }
    if(act->text() == "About"){
        msgbox_about_.exec();
    }
}

void Game::NewCustomGame(){
    bool all_digit = 1;
    for(int i = 0; i < 3; i ++){
        QByteArray ba = custom_set_[i].text().toLatin1();
        const char *s = ba.data();
        while(*s && *s>='0' && *s<='9') s++;
        if(*s) all_digit = 0;   // True iff the string consists of only digits
    }
    int temp_input[3] = {custom_set_[0].text().toInt(),
                         custom_set_[1].text().toInt(), custom_set_[2].text().toInt()};
    if(all_digit && temp_input[0] >= 10 && temp_input[0] <= 99 && temp_input[1] >= 10 && temp_input[1] <= 99
            && temp_input[2] >= 10 && temp_input[2] <= 999 && temp_input[2] < temp_input[0]*temp_input[1]){
        delete ms_map_;
        ms_map_ = new MineSweeperMap(this, CUSTOM, custom_set_[0].text().toInt(),
                custom_set_[1].text().toInt(), custom_set_[2].text().toInt());
        this->setFixedSize(120+temp_input[0]*30, 120+temp_input[1]*30);
    }
    else{
        QMessageBox* custom_warning = new QMessageBox();
        custom_warning->setWindowTitle("Warning");
        custom_warning->setText("Invalid input!");
        custom_warning->show();
    }
}

void Game::RecordTime(GameMode game_mode, int record_time){
    this->record_list_.Record(game_mode, record_time);
    this->record_list_.Read();
}

Game::~Game(){
    delete ms_map_;
}
