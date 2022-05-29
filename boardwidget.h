#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    //初始化棋盘
    explicit BoardWidget(QWidget *parent = 0);

    //结束信号传回
signals:
    void game_over(int winner);        // 游戏结束的信号


public slots:

protected:
    //控制鼠标事件
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

public:
    //清空棋盘
    void initBoard();
    //悔棋
    bool repentance();
    //设置游戏模式——gamemode = 1，PVE，gamemode = 2,Deduction = 3
    void setGameMode(int gamemode);
    //对于PVE设置先手
    void whoFirst(bool aifirst);;
    //难度设置
    void selectdifficulty(int x);
    void aiPiece();
private:
    // 追踪鼠标位置
    void setTrackPos(QPoint point);
     // 落子
    void dropPiece(int x, int y);

    // 判断输赢
    bool isFivePiece(int x, int y);
    bool isVFivePiece(int x, int y);   // 从(x,y)开始，沿着垂直方向
    bool isHFivePiece(int x, int y);   // 从(x,y)开始，沿着水平方向
    bool isLeftSlash(int x, int y);    // 从(x,y)开始，沿着左对角线
    bool isRightSlash(int x, int y);   // 从(x,y)开始，沿着右对角线
    bool checkWinner();

    QPoint trackPos;    // 记录鼠标的位置

public:
    bool AI_FIRST;
    int gamemode;
    bool endGame;       // 标志游戏是否结束
    bool next_player;   // 下一位玩家
    bool WHITE_PALYER;  // 白色玩家
    bool BLACK_PLAYER;
    // 棋盘的大小15x15
    static const int BOARD_WIDTH = 15;
    static const int BOARD_HEIGHT = 15;
    int board[BOARD_WIDTH+10][BOARD_HEIGHT+10];   // 定义棋盘

    //棋盘起始的的位置  行和列
    static const QPoint ROW_NUM_START;
    static const QPoint COLUMN_NUM_START;

    // size
    static const QSize WIDGET_SIZE;
    static const QSize CELL_SIZE;

    static const QPoint START_POS;
    static const int NO_PIECE = 0;     // 用于标记棋盘中某个位置没有棋子

    // 定义玩家
    static const int WHITE_PIECE = 1;   // 白棋
    static const int BLACK_PIECE = 2;   // 黑棋


};

#endif // BOARDWIDGET_H
