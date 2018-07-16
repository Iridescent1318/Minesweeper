#include "minesweepermap.h"


MineSweeperMap::MineSweeperMap(QMainWindow *main_window , GameMode game_mode,
                               int map_width, int map_height, int num_of_mines): QObject() { //Initialization
    main_window_ = main_window;
    opened_blocks_ = 0;
    is_over_ = false;
    game_start_ = false;
    game_mode_ = game_mode;
    if(game_mode_ == EASY){
        map_width_ = 10;
        map_height_ = 10;
        num_of_mines_ = 10;
    }
    if(game_mode_ == NORMAL){
        map_width_ = 16;
        map_height_ = 16;
        num_of_mines_ = 40;
    }
    if(game_mode_ == HARD){
        map_width_ = 30;
        map_height_ = 16;
        num_of_mines_ = 99;
    }
    if(game_mode_ == CUSTOM){
        map_width_ = map_width;
        map_height_ = map_height;
        num_of_mines_ = num_of_mines;
    }
    map_ = new Block[map_width_ * map_height_];
    for(int i = 0; i < map_width_*map_height_; i ++){
        map_[i].setParent(main_window_);
        map_[i].SetCoorX(i % map_width_);
        map_[i].SetCoorY(i / map_width_);
        if(i == 0)
            map_[i].SetPosition(CORNER_LU);
        if(i>0 && i < map_width_-1)
            map_[i].SetPosition(MARGIN_UP);
        if(i == map_width_-1)
            map_[i].SetPosition(CORNER_RU);
        if(i == map_width_*(map_height_-1))
            map_[i].SetPosition(CORNER_LD);
        if(i == map_width_*map_height_-1)
            map_[i].SetPosition(CORNER_RD);
        if(i > map_width_*(map_height_-1) && i < map_width_*map_height_-1)
            map_[i].SetPosition(MARGIN_DOWN);
        if(i > map_width_-1 && i < map_width_*(map_height_-1)){
            if(i%map_width_ == 0)
                map_[i].SetPosition(MARGIN_LEFT);
            if((i+1)%map_width_ == 0)
                map_[i].SetPosition(MARGIN_RIGHT);
        }
        map_[i].setFixedSize(29,29);
        map_[i].setStyleSheet(//"border-width:1px;"
                              "border:none;"
                              "background-color:rgb(222,222,222);"
                              "font:bold;"
                              "font-size:16;"
                              "color:black;"
                              );
        map_[i].move(30+(map_[i].GetCoorX()+1)*30,50+(map_[i].GetCoorY()+1)*30);
        map_[i].show();
        QObject::connect(&map_[i], SIGNAL(SendBlockLeftClicked(Block*)), this, SLOT(MapBegin(Block*)));
        QObject::connect(&map_[i], SIGNAL(SendBlockLeftClicked(Block*)), this, SLOT(LeftClickOnBlock(Block*)));
        QObject::connect(&map_[i], SIGNAL(SendBlockRightClicked(Block*)), this, SLOT(RightClickOnBlock(Block*)));
    }
    game_timer_.setDecMode();
    game_timer_.setDigitCount(3);
    game_timer_.display(0);
    game_timer_.setParent(main_window);
    game_timer_.setFixedSize(80,40);
    game_timer_.move(60,35);
    game_timer_.show();
    QObject::connect(this, SIGNAL(SendMapBegin()), &game_timer_, SLOT(TimingStart()));
    QObject::connect(this, SIGNAL(SendMineAllFound()), &game_timer_, SLOT(TimingStop()));
    QObject::connect(this, SIGNAL(SendMineBoomed()), &game_timer_, SLOT(TimingStop()));

    QObject::connect(this, SIGNAL(SendMineBoomed()), this, SLOT(MapOver()));
    QObject::connect(this, SIGNAL(SendMapOver()), main_window_, SLOT(GameOver()));
    QObject::connect(this, SIGNAL(SendMineAllFound()), this, SLOT(MapWin()));
    QObject::connect(this, SIGNAL(SendMapWin()), main_window_, SLOT(GameWin()));

    QObject::connect(this, SIGNAL(SendTime(GameMode,int)), main_window_, SLOT(RecordTime(GameMode,int)));
}

void MineSweeperMap::CountMinesAround(){ // Count number of mines around of every block
    for(int i = 0; i < map_width_ * map_height_; i ++){
        int temp = 0;
        if(map_[i].GetPosition() == CORNER_LU){
            if(!map_[i+1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_].GetSafeStatus()) temp++;
            if(!map_[i+1+map_width_].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == CORNER_LD){
            if(!map_[i+1].GetSafeStatus()) temp++;
            if(!map_[i-map_width_].GetSafeStatus()) temp++;
            if(!map_[i-map_width_+1].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == CORNER_RU){
            if(!map_[i-1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_].GetSafeStatus()) temp++;
            if(!map_[i+map_width_-1].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == CORNER_RD){
            if(!map_[i-1].GetSafeStatus()) temp++;
            if(!map_[i-map_width_].GetSafeStatus()) temp++;
            if(!map_[i-map_width_-1].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == MARGIN_UP){
            if(!map_[i-1].GetSafeStatus()) temp++;
            if(!map_[i+1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_-1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_].GetSafeStatus()) temp++;
            if(!map_[i+map_width_+1].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == MARGIN_DOWN){
            if(!map_[i-1].GetSafeStatus()) temp++;
            if(!map_[i+1].GetSafeStatus()) temp++;
            if(!map_[i-map_width_-1].GetSafeStatus()) temp++;
            if(!map_[i-map_width_].GetSafeStatus()) temp++;
            if(!map_[i-map_width_+1].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == MARGIN_LEFT){
            if(!map_[i-map_width_].GetSafeStatus()) temp++;
            if(!map_[i-map_width_+1].GetSafeStatus()) temp++;
            if(!map_[i+1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_].GetSafeStatus()) temp++;
            if(!map_[i+map_width_+1].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == MARGIN_RIGHT){
            if(!map_[i-map_width_].GetSafeStatus()) temp++;
            if(!map_[i-map_width_-1].GetSafeStatus()) temp++;
            if(!map_[i-1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_].GetSafeStatus()) temp++;
            if(!map_[i+map_width_-1].GetSafeStatus()) temp++;
        }
        if(map_[i].GetPosition() == INSIDE){
            if(!map_[i-map_width_-1].GetSafeStatus()) temp++;
            if(!map_[i-map_width_].GetSafeStatus()) temp++;
            if(!map_[i-map_width_+1].GetSafeStatus()) temp++;
            if(!map_[i-1].GetSafeStatus()) temp++;
            if(!map_[i+1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_-1].GetSafeStatus()) temp++;
            if(!map_[i+map_width_].GetSafeStatus()) temp++;
            if(!map_[i+map_width_+1].GetSafeStatus()) temp++;
        }
        map_[i].SetMinesAround(temp);
    }
}

void MineSweeperMap::Reset(){ //Reset the map
    opened_blocks_ = 0;
    is_over_ = false;
    game_start_ = false;
    for(int i = 0; i < map_width_*map_height_; i ++){
        map_[i].SetClickedStatus(false);
        map_[i].SetSafeStatus(true);
        map_[i].SetMarkStatus(false);
        map_[i].SetInQueueStatus(false);
        map_[i].setText("");
        map_[i].setStyleSheet("border-width:1px;"
                              "border:none;"
                              "background-color:rgb(222,222,222);"
                              "font:bold;"
                              "font-size:16;"
                              "color:black;");
    }
    game_timer_.TimingReset();
}

void MineSweeperMap::MapBegin(Block* block){ //The first left-click makes the game begin
    if(game_start_ == false){
        srand((unsigned)time(NULL));
        int mines_count = 0;
        while(mines_count < num_of_mines_){
            int temp = rand()% (map_height_*map_width_);
            if(map_[temp].GetSafeStatus() == true && block != &map_[temp]){
                map_[temp].SetSafeStatus(false);
                mines_count ++;
            }
        }
        CountMinesAround();
        emit SendMapBegin();
        game_start_ = true;
    }
}

void MineSweeperMap::LeftClickOnBlock(Block* block){ //Left-click event
    if(!block->GetMarkStatus() && block->GetSafeStatus()) {
        // When a safe block is clicked, some of its neighbors (neighbors' neighbors and so on) will be opened.
        // Use queue to make breath-first-search
        block->SetClickedStatus(true);
        std::queue <Block*> blocks_to_open;
        blocks_to_open.push(block);
        block->SetInQueueStatus(true);
        while(!blocks_to_open.empty()){ // Control the depth
            if(blocks_to_open.front()->GetSafeStatus()){ // won't open mines
                blocks_to_open.front()->SetClickedStatus(true);// Mark as clicked(visited)
                if(blocks_to_open.front()->GetMinesAround() == 0){ //Open 0-mine-around blocks and its neighbors
                    if(blocks_to_open.front()->GetPosition() == INSIDE){
                        if(!(blocks_to_open.front()+1)->GetClickedStatus() && (blocks_to_open.front()+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+1)->GetInQueueStatus()){ //Right
                            blocks_to_open.push(blocks_to_open.front()+1);
                            (blocks_to_open.front()+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-1)->GetClickedStatus() && (blocks_to_open.front()-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-1)->GetInQueueStatus()) { //Left
                            blocks_to_open.push(blocks_to_open.front()-1);
                            (blocks_to_open.front()-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_)->GetClickedStatus() && (blocks_to_open.front()+map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_)->GetInQueueStatus()) { //Down
                            blocks_to_open.push(blocks_to_open.front()+map_width_);
                            (blocks_to_open.front()+map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_)->GetClickedStatus() && (blocks_to_open.front()-map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_)->GetInQueueStatus()) { //Up
                            blocks_to_open.push(blocks_to_open.front()-map_width_);
                            (blocks_to_open.front()-map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_+1)->GetClickedStatus() && (blocks_to_open.front()-map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_+1)->GetInQueueStatus()) { //RU
                            blocks_to_open.push(blocks_to_open.front()-map_width_+1);
                            (blocks_to_open.front()-map_width_+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_-1)->GetClickedStatus() && (blocks_to_open.front()-map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_-1)->GetInQueueStatus()) { //LU
                            blocks_to_open.push(blocks_to_open.front()-map_width_-1);
                            (blocks_to_open.front()-map_width_-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_-1)->GetClickedStatus() && (blocks_to_open.front()+map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_-1)->GetInQueueStatus()) { //LD
                            blocks_to_open.push(blocks_to_open.front()+map_width_-1);
                            (blocks_to_open.front()+map_width_-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_+1)->GetClickedStatus() && (blocks_to_open.front()+map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_+1)->GetInQueueStatus()) { //RD
                            blocks_to_open.push(blocks_to_open.front()+map_width_+1);
                            (blocks_to_open.front()+map_width_+1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == MARGIN_UP){
                        if(!(blocks_to_open.front()+1)->GetClickedStatus() && (blocks_to_open.front()+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+1)->GetInQueueStatus()){ //Right
                            blocks_to_open.push(blocks_to_open.front()+1);
                            (blocks_to_open.front()+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-1)->GetClickedStatus() && (blocks_to_open.front()-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-1)->GetInQueueStatus()) { //Left
                            blocks_to_open.push(blocks_to_open.front()-1);
                            (blocks_to_open.front()-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_)->GetClickedStatus() && (blocks_to_open.front()+map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_)->GetInQueueStatus()) { //Down
                            blocks_to_open.push(blocks_to_open.front()+map_width_);
                            (blocks_to_open.front()+map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_-1)->GetClickedStatus() && (blocks_to_open.front()+map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_-1)->GetInQueueStatus()) { //LD
                            blocks_to_open.push(blocks_to_open.front()+map_width_-1);
                            (blocks_to_open.front()+map_width_-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_+1)->GetClickedStatus() && (blocks_to_open.front()+map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_+1)->GetInQueueStatus()) { //RD
                            blocks_to_open.push(blocks_to_open.front()+map_width_+1);
                            (blocks_to_open.front()+map_width_+1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == MARGIN_DOWN){
                        if(!(blocks_to_open.front()+1)->GetClickedStatus() && (blocks_to_open.front()+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+1)->GetInQueueStatus()){ //Right
                            blocks_to_open.push(blocks_to_open.front()+1);
                            (blocks_to_open.front()+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-1)->GetClickedStatus() && (blocks_to_open.front()-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-1)->GetInQueueStatus()) { //Left
                            blocks_to_open.push(blocks_to_open.front()-1);
                            (blocks_to_open.front()-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_)->GetClickedStatus() && (blocks_to_open.front()-map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_)->GetInQueueStatus()) { //Up
                            blocks_to_open.push(blocks_to_open.front()-map_width_);
                            (blocks_to_open.front()-map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_+1)->GetClickedStatus() && (blocks_to_open.front()-map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_+1)->GetInQueueStatus()) { //RU
                            blocks_to_open.push(blocks_to_open.front()-map_width_+1);
                            (blocks_to_open.front()-map_width_+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_-1)->GetClickedStatus() && (blocks_to_open.front()-map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_-1)->GetInQueueStatus()) { //LU
                            blocks_to_open.push(blocks_to_open.front()-map_width_-1);
                            (blocks_to_open.front()-map_width_-1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == MARGIN_LEFT){
                        if(!(blocks_to_open.front()+1)->GetClickedStatus() && (blocks_to_open.front()+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+1)->GetInQueueStatus()){ //Right
                            blocks_to_open.push(blocks_to_open.front()+1);
                            (blocks_to_open.front()+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_)->GetClickedStatus() && (blocks_to_open.front()+map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_)->GetInQueueStatus()) { //Down
                            blocks_to_open.push(blocks_to_open.front()+map_width_);
                            (blocks_to_open.front()+map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_)->GetClickedStatus() && (blocks_to_open.front()-map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_)->GetInQueueStatus()) { //Up
                            blocks_to_open.push(blocks_to_open.front()-map_width_);
                            (blocks_to_open.front()-map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_+1)->GetClickedStatus() && (blocks_to_open.front()-map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_+1)->GetInQueueStatus()) { //RU
                            blocks_to_open.push(blocks_to_open.front()-map_width_+1);
                            (blocks_to_open.front()-map_width_+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_+1)->GetClickedStatus() && (blocks_to_open.front()+map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_+1)->GetInQueueStatus()) { //RD
                            blocks_to_open.push(blocks_to_open.front()+map_width_+1);
                            (blocks_to_open.front()+map_width_+1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == MARGIN_RIGHT){
                        if(!(blocks_to_open.front()-1)->GetClickedStatus() && (blocks_to_open.front()-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-1)->GetInQueueStatus()) { //Left
                            blocks_to_open.push(blocks_to_open.front()-1);
                            (blocks_to_open.front()-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_)->GetClickedStatus() && (blocks_to_open.front()+map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_)->GetInQueueStatus()) { //Down
                            blocks_to_open.push(blocks_to_open.front()+map_width_);
                            (blocks_to_open.front()+map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_)->GetClickedStatus() && (blocks_to_open.front()-map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_)->GetInQueueStatus()) { //Up
                            blocks_to_open.push(blocks_to_open.front()-map_width_);
                            (blocks_to_open.front()-map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_-1)->GetClickedStatus() && (blocks_to_open.front()-map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_-1)->GetInQueueStatus()) { //LU
                            blocks_to_open.push(blocks_to_open.front()-map_width_-1);
                            (blocks_to_open.front()-map_width_-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_-1)->GetClickedStatus() && (blocks_to_open.front()+map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_-1)->GetInQueueStatus()) { //LD
                            blocks_to_open.push(blocks_to_open.front()+map_width_-1);
                            (blocks_to_open.front()+map_width_-1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == CORNER_LU){
                        if(!(blocks_to_open.front()+1)->GetClickedStatus() && (blocks_to_open.front()+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+1)->GetInQueueStatus()){ //Right
                            blocks_to_open.push(blocks_to_open.front()+1);
                            (blocks_to_open.front()+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_)->GetClickedStatus() && (blocks_to_open.front()+map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_)->GetInQueueStatus()) { //Down
                            blocks_to_open.push(blocks_to_open.front()+map_width_);
                            (blocks_to_open.front()+map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_+1)->GetClickedStatus() && (blocks_to_open.front()+map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_+1)->GetInQueueStatus()) { //RD
                            blocks_to_open.push(blocks_to_open.front()+map_width_+1);
                            (blocks_to_open.front()+map_width_+1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == CORNER_LD){
                        if(!(blocks_to_open.front()+1)->GetClickedStatus() && (blocks_to_open.front()+1)->GetSafeStatus()
                                && !(blocks_to_open.front()+1)->GetInQueueStatus()){ //Right
                            blocks_to_open.push(blocks_to_open.front()+1);
                            (blocks_to_open.front()+1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_)->GetClickedStatus() && (blocks_to_open.front()-map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_)->GetInQueueStatus()) { //Up
                            blocks_to_open.push(blocks_to_open.front()-map_width_);
                            (blocks_to_open.front()-map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_+1)->GetClickedStatus() && (blocks_to_open.front()-map_width_+1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_+1)->GetInQueueStatus()) { //RU
                            blocks_to_open.push(blocks_to_open.front()-map_width_+1);
                            (blocks_to_open.front()-map_width_+1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == CORNER_RU){
                        if(!(blocks_to_open.front()-1)->GetClickedStatus() && (blocks_to_open.front()-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-1)->GetInQueueStatus()) { //Left
                            blocks_to_open.push(blocks_to_open.front()-1);
                            (blocks_to_open.front()-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_)->GetClickedStatus() && (blocks_to_open.front()+map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_)->GetInQueueStatus()) { //Down
                            blocks_to_open.push(blocks_to_open.front()+map_width_);
                            (blocks_to_open.front()+map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()+map_width_-1)->GetClickedStatus() && (blocks_to_open.front()+map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()+map_width_-1)->GetInQueueStatus()) { //LD
                            blocks_to_open.push(blocks_to_open.front()+map_width_-1);
                            (blocks_to_open.front()+map_width_-1)->SetInQueueStatus(true);
                        }
                    }
                    if(blocks_to_open.front()->GetPosition() == CORNER_RD){
                        if(!(blocks_to_open.front()-1)->GetClickedStatus() && (blocks_to_open.front()-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-1)->GetInQueueStatus()) { //Left
                            blocks_to_open.push(blocks_to_open.front()-1);
                            (blocks_to_open.front()-1)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_)->GetClickedStatus() && (blocks_to_open.front()-map_width_)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_)->GetInQueueStatus()) { //Up
                            blocks_to_open.push(blocks_to_open.front()-map_width_);
                            (blocks_to_open.front()-map_width_)->SetInQueueStatus(true);
                        }
                        if(!(blocks_to_open.front()-map_width_-1)->GetClickedStatus() && (blocks_to_open.front()-map_width_-1)->GetSafeStatus()
                                && !(blocks_to_open.front()-map_width_-1)->GetInQueueStatus()) { //LU
                            blocks_to_open.push(blocks_to_open.front()-map_width_-1);
                            (blocks_to_open.front()-map_width_-1)->SetInQueueStatus(true);
                        }
                    }
                }
            }
            QString temp = blocks_to_open.front()->GetMinesAround()?
                        QString::number(blocks_to_open.front()->GetMinesAround(),10):" ";
            blocks_to_open.front()->setText(temp); // Mark number of mines around
            blocks_to_open.front()->setStyleSheet(//"border-width:1px;"
                                    "border:none;"
                                    "background-color:rgb(233,233,233);"
                                    "font:bold;"
                                    "font-size:16;");
            blocks_to_open.pop(); // Remove the block
        }
    }
    if(!block->GetMarkStatus() && !block->GetSafeStatus()) {
        block->SetClickedStatus(true);
        //qDebug("Game Over");
        is_over_ = true;
        block->setStyleSheet(//"border-width:1px;"
                             "border:none;"
                             "background-color:rgb(222,222,222);"
                             "font:bold;"
                             "font-size:16;"
                             "color:red;");
        block->setText("X");
        emit SendMineBoomed();
    }
    bool b_win = 1;
    for(int i = 0; i < map_width_*map_height_; i ++){
        if(!map_[i].GetClickedStatus() && map_[i].GetSafeStatus()){
            b_win = 0;
            break;
        }
    }
    if(b_win && !is_over_) emit SendMineAllFound();
}

void MineSweeperMap::RightClickOnBlock(Block* block){ //Right-click event

}

void MineSweeperMap::MapOver(){ // Mines boomed
    for(int i = 0; i < map_width_*map_height_; i ++){
        map_[i].SetClickedStatus(true);
        if(map_[i].GetSafeStatus()){
            QString temp = map_[i].GetMinesAround()?
                        QString::number(map_[i].GetMinesAround(),10):" ";
            map_[i].setStyleSheet(map_[i].GetMarkStatus()?
                                      "color:blue;"
                                      //"border-width:1px;"
                                      "border:none;"
                                      "background-color:rgb(233,233,233);"
                                      "font:bold;"
                                      "font-size:16;"
                                    :
                                      "color:black;"
                                      //"border-width:1px;"
                                      "border:none;"
                                      "background-color:rgb(233,233,233);"
                                      "font:bold;"
                                      "font-size:16;");
            map_[i].setText(temp);
        }
        else {
            map_[i].setStyleSheet(map_[i].GetMarkStatus()?"color:blue;"
                                                          //"border-width:1px;"
                                                          "border:none;"
                                                          "background-color:rgb(222,222,222);"
                                                          "font:bold;"
                                                          "font-size:16;"
                                                        :
                                                          "color:red;"
                                                          //"border-width:1px;"
                                                          "border:none;"
                                                          "background-color:rgb(222,222,222);"
                                                          "font:bold;"
                                                          "font-size:16;");
            map_[i].setText("X");
        }
    }
    emit SendMapOver();
}

void MineSweeperMap::MapWin(){ // Mines all found
    for(int i = 0; i < map_width_*map_height_; i ++){
        map_[i].SetClickedStatus(true);
        if(map_[i].GetSafeStatus()){
            QString temp = map_[i].GetMinesAround()?
                        QString::number(map_[i].GetMinesAround(),10):" ";
            map_[i].setStyleSheet("color:black;"
                                  //"border-width:1px;"
                                  "border:none;"
                                  "background-color:rgb(233,233,233);"
                                  "font:bold;"
                                  "font-size:16;");
            map_[i].setText(temp);
        }
        else {
            map_[i].setStyleSheet("color:blue;"
                                  //"border-width:1px;"
                                  "border:none;"
                                  "background-color:rgb(222,222,222);"
                                  "font:bold;"
                                  "font-size:16;");
            map_[i].setText("X");
        }
    }
    emit SendMapWin();
    emit SendTime(this->game_mode_, game_timer_.GetTiming());
}

MineSweeperMap::~MineSweeperMap(){
    main_window_ = NULL;
    delete []map_;
}
