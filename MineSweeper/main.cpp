#include "game.h"
#include "minesweepermap.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft YaHei", 9, QFont::Normal));
    Game g(NULL);
    g.NewGame(HARD);
    g.show();

    return a.exec();
}
