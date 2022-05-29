#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "boardwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QInputDialog>
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    //初始化棋盘
    GameWidget(QWidget *parent = 0);
    ~GameWidget();
    //
    int gameModeChoose();
    bool firstChoose();
    int difficultyChoose();
private:
    BoardWidget* board;
    QString title = "<--Gerchart's GolangGame-->          Config:";
    QString whofirst = "";
    QString difficuly = "";
    void initGame(BoardWidget* board);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);    // 布局管理器 垂直
    QHBoxLayout* horizon_layout = new QHBoxLayout(this);    // 水平布局管理器

private slots:
    void restart_game();
    void repentance_chess();
    void admit_defeat();
    void reset_game();
    void input_finish();
public slots:
    void showWinner(int winner);
};

#endif // GAMEWIDGET_H
