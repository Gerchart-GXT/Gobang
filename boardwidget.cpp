#include "boardwidget.h"
#include <QPainter>
#include <QRect>
#include <QPen>
#include <QBrush>
#include <QPoint>
#include <qmath.h>
#include <QMouseEvent>
#include <QString>
#include <QSize>
#include <random>
#include <QDebug>
#include <aiJudge.h>
#include <unistd.h>

std::vector<aiJudge::point> steps;

int depthcfg = 4;
int outcfg = INF;
// 初始化参数
// 1. 棋盘的起始位置：
const QPoint BoardWidget::ROW_NUM_START(15, 45);
const QPoint BoardWidget::COLUMN_NUM_START(35, 25);
// 2.size初始化
const QSize BoardWidget::WIDGET_SIZE(700, 700);
const QSize BoardWidget::CELL_SIZE(45, 45);

const QPoint BoardWidget::START_POS(40, 40);

BoardWidget::BoardWidget(QWidget *parent) : QWidget(parent), trackPos(28, 28)
{
    setFixedSize(WIDGET_SIZE);     // 设置窗口为固定大小
    setMouseTracking(true);        // 开启MouseTracking
    initBoard();                   // 初始化棋盘
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);    // this代表Widget，及绘图设备
    painter.setRenderHint(QPainter::Antialiasing);    // 绘画普通图形启用反走样, 即开启抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing);    // 绘画文字反走样， 即开启抗锯齿

    painter.fillRect(this->rect(), "#f5deb3");    //填充背景颜色
    // 设置字体
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    // 设置画笔
    QPen pen;
    pen.setWidth(1);       // 设置线宽
    pen.setColor("#daa520");     // 设置颜色
    pen.setStyle(Qt::SolidLine);    // 设置线型
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    painter.setPen(pen);

    for(int i=0; i<BOARD_WIDTH; i++)
    {
        painter.drawText(COLUMN_NUM_START+QPoint(i*CELL_SIZE.width(), 0), QString::number(i+1));
    }

    for(int j=0; j<BOARD_HEIGHT; j++)
    {
        painter.drawText(ROW_NUM_START + QPoint(0, j*CELL_SIZE.height()), QString::number(j+1));
    }

    for(int i=0; i<BOARD_WIDTH-1; i++)
    {
        for(int j=0; j<BOARD_HEIGHT-1; j++)
        {
            painter.drawRect(QRect(START_POS+QPoint(i*CELL_SIZE.width(), j*CELL_SIZE.height()), CELL_SIZE));
        }
    }

    painter.setPen(Qt::red);
    QPoint poses[12] = {
        trackPos + QPoint(0, 8),
        trackPos,
        trackPos + QPoint(8, 0),
        trackPos + QPoint(17, 0),
        trackPos + QPoint(25, 0),
        trackPos + QPoint(25, 8),
        trackPos + QPoint(25, 17),
        trackPos + QPoint(25, 25),
        trackPos + QPoint(17, 25),
        trackPos + QPoint(8, 25),
        trackPos + QPoint(0, 25),
        trackPos + QPoint(0, 17)
    };
    painter.drawPolyline(poses, 3);
    painter.drawPolyline(poses+3, 3);
    painter.drawPolyline(poses+6, 3);
    painter.drawPolyline(poses+9, 3);

    //绘制五个点
    PII fivestar[5] = {{3,3},{11,11},{7,7},{3,11},{11,3}};
    for(int i = 0;i<5;i++){
        auto t = fivestar[i];
        int x = t.fi,y = t.se;
        painter.setPen(Qt::NoPen);
        QColor signs;
        signs = "#daa520";
        painter.setBrush(QBrush(signs));
        painter.drawEllipse(START_POS.x() + x*CELL_SIZE.width()-5, START_POS.y() + y*CELL_SIZE.height()-5,
                            CELL_SIZE.width()/5, CELL_SIZE.height()/5);
    }

    // 绘制棋子
    for(int i=0; i<BOARD_WIDTH; i++)
    {
        for(int j=0; j<BOARD_HEIGHT; j++)
        {
            QColor piece_color;
            if(board[i][j] == WHITE_PIECE)
            {
                piece_color = Qt::white;
                painter.setBrush(QBrush(piece_color));
                painter.drawEllipse(START_POS.x() + i*CELL_SIZE.width() - CELL_SIZE.width()/2, START_POS.y() + j*CELL_SIZE.height() - CELL_SIZE.height()/2,
                                    CELL_SIZE.width()-4, CELL_SIZE.height()-4);
            }
            else if(board[i][j] == BLACK_PIECE)
            {
                piece_color = Qt::black;
                painter.setBrush(QBrush(piece_color));
                painter.drawEllipse(START_POS.x() + i*CELL_SIZE.width() - CELL_SIZE.width()/2, START_POS.y() + j*CELL_SIZE.height() - CELL_SIZE.height()/2,
                                    CELL_SIZE.width()-4, CELL_SIZE.height()-4);
            }
            else
            {
                painter.setPen(Qt::NoPen);
            }
        }
    }

    font.setPointSize(15);
    font.setBold(true);
    painter.setFont(font);
    int cnt = 1;
    for(auto i = 0;i<(int)steps.size();++i){
        auto t = steps[i];
        int color = t.color;
        int x = t.x-1,y = t.y-1;
        int x_fix = 0;
        if(cnt>=10)
            x_fix = 5;
        if(cnt>=100)
            x_fix = 8;
        if(color){
             pen.setColor(Qt::white);
             if(i == (int)steps.size()-1)
                pen.setColor(Qt::red);
             painter.setPen(pen);
             painter.drawText(QPoint(START_POS.x() + x*CELL_SIZE.width()-5-x_fix, START_POS.y() + y*CELL_SIZE.height()+5),QString::number(cnt));
        }else{
            pen.setColor(Qt::black);
            if(i == (int)steps.size()-1)
               pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawText(QPoint(START_POS.x() + x*CELL_SIZE.width()-5-x_fix, START_POS.y() + y*CELL_SIZE.height()+5),QString::number(cnt));
        }
        cnt++;
    }
}


void BoardWidget::mouseMoveEvent(QMouseEvent *event)    // 鼠标事件
{
    QPoint pos = event->pos() - START_POS;    // 相对于棋盘起始位置的坐标
    QPoint temp_point = pos + QPoint(CELL_SIZE.width()/2, CELL_SIZE.height()/2);   // 坐标
    int x = temp_point.x();
    int y = temp_point.y();
    if(x <= 0 || y <= 0 || x >= BOARD_WIDTH*CELL_SIZE.width() || y >= BOARD_HEIGHT*CELL_SIZE.height())
        return;
    int offset_x = x % CELL_SIZE.width();    // 这个坐标表示不够一个cell_size的坐标的大小
    int offset_y = y % CELL_SIZE.height();
    // 绘制的图标的位置，中心的为十字交叉的位置
    QPoint tip_position = QPoint(x-offset_x, y-offset_y)+START_POS-QPoint(CELL_SIZE.width()/2, CELL_SIZE.height()/2)+QPoint(8, 8);
    setTrackPos(tip_position);

}


void BoardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(!endGame)      // 游戏未结束
    {
        QPoint pos = event->pos() - START_POS;
        int x = pos.x();
        int y = pos.y();
        int x_pos = x / CELL_SIZE.width();         // 整数，确定第几个格子
        int y_pos = y / CELL_SIZE.height();
        int x_offset = x % CELL_SIZE.width();     // 余数，计算是否需要偏移
        int y_offset = y % CELL_SIZE.height();
        if(x_offset > CELL_SIZE.width()/2)
        {
            x_pos++;
        }
        if(y_offset > CELL_SIZE.height()/2)
        {
            y_pos++;
        }
        dropPiece(x_pos, y_pos);   //落子
    }
}

void BoardWidget::initBoard()
{
    // 对棋盘进行初始化
    for(int i=0; i<BOARD_WIDTH; i++)
    {
        for(int j=0; j<BOARD_HEIGHT; j++)
        {
            board[i][j] = NO_PIECE;
        }
    }
    endGame = false;
    // 黑棋先落子
    WHITE_PALYER = false;
    BLACK_PLAYER = true;
    next_player = BLACK_PLAYER;

    update();
    initZobrist();
    steps.clear();
}
bool BoardWidget::repentance()
{
    endGame = false;
    if(steps.size()<2)
        return false;
    for(int i = 0;i<2;i++){
        auto t = steps.back();
        int x = t.x-1,y = t.y-1;
        board[x][y] = NO_PIECE;
        steps.pop_back();
    }
    return true;;
}

void BoardWidget::setTrackPos(QPoint point)
{
    trackPos = point;
    repaint();
}


void BoardWidget::dropPiece(int x, int y)
{
    if(endGame)
        return;
    if(x>=0 && x<=BOARD_WIDTH && y>=0 && y<=BOARD_HEIGHT && board[x][y]==NO_PIECE)
    {
        if(next_player == BLACK_PLAYER){
            board[x][y] = BLACK_PIECE;
            steps.push_back({1,x+1,y+1});
        }
        else{
            board[x][y] = WHITE_PIECE;
            steps.push_back({0,x+1,y+1});
        }
        adjustSize();
        if(checkWinner())
            return;
        next_player = !next_player;
        //如果为PVE进入aiPiece
        if(gamemode == 2){
            aiPiece();
        }
    }
}
void BoardWidget::aiPiece()
{
   if(steps.size() == 1){
       int x = steps.back().x,y = steps.back().y;
       int tx = rand()%2;
       int ty = rand()%2;
       while(tx == 0&&ty == 0){
           tx = rand()%2;
           ty = rand()%2;
       }
       steps.push_back({0,x+tx,x+ty});
       board[x+tx-1][x+ty-1] = WHITE_PIECE;
       repaint();
       next_player = !next_player;
       return;
   }
   if(checkWinner())
       return;
   aiJudge::point aistep = {0,0,0,-INF};
   //IDA*（迭代加深）优化，从底层开始搜，如果有杀棋则提前返回
   for(int i = 2;i<=depthcfg;i+=2){
       repaint();
       auto ai = new aiJudge(steps,next_player,i);
       auto tmpstep = ai->getStep();
       //更新aistep
       if(tmpstep.score>aistep.score)
           aistep = tmpstep;
       delete ai;
       //如果有杀棋
       if(aistep.score>720*2)
           break;
   }
   int x = aistep.x-1,y = aistep.y-1;
   steps.push_back(aistep);
   if(next_player == BLACK_PLAYER)
        board[x][y] = BLACK_PIECE;
   else{
       board[x][y] = WHITE_PIECE;
   }
   next_player = !next_player;
   repaint();
   if(checkWinner())
       return;
}

bool BoardWidget::isHFivePiece(int x, int y)
{
    // 判断水平方向
    int piece = board[x][y];     // 当前棋子的值
    for(int i=1; i<5; i++)
    {
        if(x+i>BOARD_WIDTH || board[x+i][y]!=piece)
        {
            return false;
        }
    }
    return true;
}


bool BoardWidget::isVFivePiece(int x, int y)
{
    // 判断垂直方向
    int piece = board[x][y];
    for(int i=1; i<5; i++)
    {
        if(y+i>BOARD_HEIGHT || board[x][y+i]!=piece)
        {
            return false;
        }
    }
    return true;
}

bool BoardWidget::isLeftSlash(int x, int y)
{
    // 沿着左对角线
    int piece = board[x][y];
    for(int i=1; i<5; i++)
    {
        if(x+i>BOARD_WIDTH || y+i>BOARD_HEIGHT || board[x+i][y+i]!=piece)
        {
            return false;
        }
    }
    return true;
}

bool BoardWidget::isRightSlash(int x, int y)
{
    // 沿着右对角线
    int piece = board[x][y];
    for(int i=1; i<5; i++)
    {
        if(x-i<0 || y+i>BOARD_HEIGHT || board[x-i][y+i]!=piece)
        {
            return false;
        }
    }
    return true;
}

bool BoardWidget::isFivePiece(int x, int y)
{
    // 是否赢棋
    return isHFivePiece(x, y) || isVFivePiece(x, y) || isLeftSlash(x, y) || isRightSlash(x, y);
}


bool BoardWidget::checkWinner()
{
    bool fullPieces = true;     // 和棋
    for(int i=0; i<BOARD_WIDTH; i++)
    {
        for(int j=0; j<BOARD_HEIGHT; j++)
        {
            if(board[i][j]==NO_PIECE)    // 如果棋盘不是满的，就不是和棋
            {
                fullPieces = false;
            }
            if(board[i][j]!=NO_PIECE && isFivePiece(i, j))
            {
                endGame = true;
                int winner;
                if(board[i][j]==WHITE_PIECE)
                {
                   winner = WHITE_PIECE;
                   qDebug() << "White win!" << Qt::endl;

                   // 发出信号
                   emit game_over(winner);
                }
                else
                {
                    winner = BLACK_PIECE;
                    qDebug() << "Black win" << Qt::endl;

                    // 发出信号
                    emit game_over(winner);
                }
                return true;
            }
        }
    }
    if(fullPieces)
    {
        emit game_over(3);
        qDebug() << "middle result" << Qt::endl;
        return true;
    }
    return false;
}

void BoardWidget::setGameMode(int gamemode)
{
    this->gamemode = gamemode;
}

void BoardWidget::whoFirst(bool aifirst)
{
    this->AI_FIRST =aifirst;
    if(!AI_FIRST)
        return;
    //ai先手默认走最中间
    steps.push_back({1,8,8});
    board[7][7] = BLACK_PIECE;
    update();
    next_player = !next_player;
}

void BoardWidget::selectdifficulty(int x)
{
    depthcfg = x;
}
