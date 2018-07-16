#include "block.h"

Block::Block(): QPushButton(){
    coor_x_ = 0;
    coor_y_ = 0;
    mines_around_ = 0;
    position_ = INSIDE;
    is_safe_ = true;
    is_clicked_ = false;
    is_marked_ = false;
    is_in_queue_ = false;
    boom_ = false;
    this->resize(29,29);
}


void Block::mouseReleaseEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        emit SendBlockLeftClicked(this);
    }
    if(e->button() == Qt::RightButton){
        emit SendBlockRightClicked(this);
        if(!is_clicked_){
            is_marked_ = is_marked_?0:1;
            setStyleSheet(is_marked_?
                          "border:none;"
                          "background-color:rgb(222,222,222);"
                          "font:bold;"
                          "font-size:16;"
                          "color:blue;"
                          :
                          "border:none;"
                          "background-color:rgb(222,222,222);"
                          "font:bold;"
                          "font-size:16;"
                          "color:black;");
            setText(is_marked_?"F":" ");
        }
    }
}
