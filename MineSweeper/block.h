#ifndef BLOCK_H
#define BLOCK_H
#include <QPushButton>
#include <QMouseEvent>
#include <queue>

typedef enum {INSIDE, MARGIN_UP, MARGIN_LEFT, MARGIN_RIGHT, MARGIN_DOWN, CORNER_LU, CORNER_LD, CORNER_RU, CORNER_RD} BlockPosition;

class Block : public QPushButton
{
    Q_OBJECT
public:
    Block();
    Block(int coor_x , int coor_y );
    int GetCoorX() const {return coor_x_;}
    int GetCoorY() const {return coor_y_;}
    int GetMinesAround() const {return mines_around_;}
    bool GetSafeStatus() const {return is_safe_;}
    bool GetClickedStatus() const {return is_clicked_;}
    bool GetMarkStatus() const {return is_marked_;}
    bool GetInQueueStatus() const {return is_in_queue_;}
    bool GetBoomStatus() const {return boom_;}
    BlockPosition GetPosition() const {return position_;}
    void SetCoorX(int coor_x) {coor_x_ = coor_x;}
    void SetCoorY(int coor_y) {coor_y_ = coor_y;}
    void SetClickedStatus(bool is_clicked) {is_clicked_ = is_clicked;}
    void SetMinesAround(int mines_around) {mines_around_ = mines_around;}
    void SetSafeStatus(bool is_safe) {is_safe_ = is_safe;}
    void SetMarkStatus(bool is_marked) {is_marked_ = is_marked;}
    void SetInQueueStatus(bool is_in_queue) {is_in_queue_ = is_in_queue;}
    void SetPosition(BlockPosition position) {position_ = position;}
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void SendBlockLeftClicked(Block*);
    void SendBlockRightClicked(Block*);
protected:
    int coor_x_;
    int coor_y_;
    int mines_around_;
    bool is_safe_;
    bool is_clicked_;
    bool is_marked_;
    bool is_in_queue_;
    bool boom_;
    BlockPosition position_;
};

#endif // BLOCK_H
