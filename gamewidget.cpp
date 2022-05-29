#include "gamewidget.h"
GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(title);   // 设置窗口标题：
    board = new BoardWidget(this);           // 新建棋盘对象
    initGame(board);
}

GameWidget::~GameWidget()
{

}
void GameWidget::initGame(BoardWidget* board)
{
    board->setGameMode(gameModeChoose());
    if(board->gamemode == 2){
        //进行先手和难度设置=
        board->whoFirst(firstChoose());
        board->selectdifficulty(difficultyChoose());
        QPushButton* btn_new_game = new QPushButton("Restart");    // 按钮，开始新游戏
        QPushButton* btn_Repentance_chess = new QPushButton("Repentance");
        QPushButton* btn_admit_defeat = new QPushButton("Admit Defeat");
        QPushButton* btn_reset_game = new QPushButton("ResetGame");
        //绘制重启，悔棋，认输,重置按钮
        horizon_layout->addWidget(btn_new_game);
        horizon_layout->addWidget(btn_Repentance_chess);
        horizon_layout->addWidget(btn_admit_defeat);
        horizon_layout->addWidget(btn_reset_game);
        //对接按钮的槽函数
        connect(btn_reset_game, SIGNAL(clicked()), this, SLOT(reset_game()));
        connect(btn_new_game, SIGNAL(clicked()), this, SLOT(restart_game()));
        connect(btn_Repentance_chess, SIGNAL(clicked()), this, SLOT(repentance_chess()));
        connect(btn_admit_defeat, SIGNAL(clicked()), this, SLOT(admit_defeat()));
        connect(board, SIGNAL(game_over(int)), this, SLOT(showWinner(int)));
    }else if(board->gamemode == 3){
        //绘制重启，悔棋，认输,重置按钮
        QPushButton* btn_Repentance_chess = new QPushButton("Repentance");
        QPushButton* btn_reset_game = new QPushButton("ResetGame");
        QPushButton* btn_input_finish = new QPushButton("Input Finish");
        horizon_layout->addWidget(btn_Repentance_chess);
        horizon_layout->addWidget(btn_input_finish);
        horizon_layout->addWidget(btn_reset_game);

        //对接按钮的槽函数
        connect(btn_reset_game, SIGNAL(clicked()), this, SLOT(reset_game()));
        connect(btn_Repentance_chess, SIGNAL(clicked()), this, SLOT(repentance_chess()));
        connect(btn_input_finish, SIGNAL(clicked()), this, SLOT(input_finish()));
        connect(board, SIGNAL(game_over(int)), this, SLOT(showWinner(int)));
    }else{
        QPushButton* btn_new_game = new QPushButton("Restart");    // 按钮，开始新游戏
        QPushButton* btn_Repentance_chess = new QPushButton("Repentance");
        QPushButton* btn_admit_defeat = new QPushButton("Admit Defeat");
        QPushButton* btn_reset_game = new QPushButton("ResetGame");
        //绘制重启，悔棋，认输,重置按钮
        horizon_layout->addWidget(btn_new_game);
        horizon_layout->addWidget(btn_Repentance_chess);
        horizon_layout->addWidget(btn_admit_defeat);
        horizon_layout->addWidget(btn_reset_game);
        //对接按钮的槽函数
        connect(btn_reset_game, SIGNAL(clicked()), this, SLOT(reset_game()));
        connect(btn_new_game, SIGNAL(clicked()), this, SLOT(restart_game()));
        connect(btn_Repentance_chess, SIGNAL(clicked()), this, SLOT(repentance_chess()));
        connect(btn_admit_defeat, SIGNAL(clicked()), this, SLOT(admit_defeat()));
        connect(board, SIGNAL(game_over(int)), this, SLOT(showWinner(int)));
    }
    mainLayout->addLayout(horizon_layout);
    mainLayout->addWidget(board);
    update();
}
void GameWidget::showWinner(int winner)
{
    QString playerName;
    if(winner==BoardWidget::WHITE_PIECE)
    {
        playerName = "White Win";
    }
    else if(winner==BoardWidget::BLACK_PIECE)
    {
        playerName = "Black Win";
    }
    else
    {
        playerName = "Both Win";
    }
    QMessageBox::information(this, "Game Over", playerName, QMessageBox::Close);
}

void GameWidget::restart_game()
{
    qDebug() << "Restart the game!" << Qt::endl;
    board->initBoard();
    if(board->gamemode == 1)
        return;
    if(!board->AI_FIRST){
        whofirst = "   Black = AI";
    }else
        whofirst = "   Black = Human";
    setWindowTitle(title+" "+difficuly+" "+whofirst);
    board->whoFirst(!board->AI_FIRST);
}
void GameWidget::reset_game()
{
    qDebug() << "Restart the game!" << Qt::endl;
    board->initBoard();
    QLayoutItem *child;
     while ((child = horizon_layout->takeAt(0)) != 0)
     {
            //setParent为NULL，防止删除之后界面不消失
            if(child->widget())
            {
                child->widget()->setParent(NULL);
            }

            delete child;
    }
    delete board;
    board = new BoardWidget(this);
    initGame(board);
}
void GameWidget::repentance_chess()
{
    if(board->repentance())
        qDebug() << "Repentance chess!" << Qt::endl;
    else{
        QMessageBox::information(this, "Repentance Erro", "No steps to repentance!", QMessageBox::Close);
        qDebug() << "No step to repentance!" << Qt::endl;
    }
    board->update();
}
void GameWidget::admit_defeat()
{
    board->endGame = true;
    board->game_over(2-(int)board->next_player);
}

void GameWidget::input_finish()
{
    board->gamemode = 2;
    board->selectdifficulty(difficultyChoose());
    board->aiPiece();
}
int GameWidget::gameModeChoose()
{
    QStringList items;
    items <<"PVE"<<"PVP"<<"Deduction";
    QString dlgTitle="GameMode";
    QString txtLabel="Please choose the gamemode!";
    int curIndex=0;
    bool editable=true;
    bool ok=false;
    QString text;
    while(!ok){
        text = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);
        if(!ok)
            QMessageBox::information(this, "Warming!", txtLabel, QMessageBox::Close);
    }
    if (ok && !text.isEmpty()){

        if(text == "PVP"){
            setWindowTitle(title+" PVP");
            return 1;
        }
        if(text == "PVE"){
            setWindowTitle(title+" PVE");
            return 2;
        }
        if(text == "Deduction"){
            setWindowTitle(title+" Deduction");
            return 3;
        }
    }
    return 2;
}
bool GameWidget::firstChoose()
{
    QStringList items;
    items <<"AI"<<"Human";
    QString dlgTitle="Offensive Choose";
    QString txtLabel="Please choose the offensive player!";
    int curIndex=0;
    bool editable=true;
    bool ok=false;
    QString text;
    while(!ok){
        text = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);
        if(!ok)
            QMessageBox::information(this, "Warming!", txtLabel, QMessageBox::Close);
    }
    if (ok && !text.isEmpty()){
        whofirst = "   Black = "+text;
        setWindowTitle(title+" "+difficuly+" "+whofirst);
        if(text == "Human")
            return false;
        else
            return true;
    }
    return false;
}
int GameWidget::difficultyChoose()
{
    QStringList items;
    items <<"Journey--(2)"<<"Classic--(4)"<<"Expert--(6)--Per Steps need about 3s"<<"Master--(8)--Per Steps need about 8s";
    QString dlgTitle="Difficulty Selection";
    QString txtLabel="Please choose the Depth of thinking";
    int curIndex=0;
    bool editable=true;
    bool ok=false;
    QString text;
    while(!ok){
        text = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);
        if(!ok)
            QMessageBox::information(this, "Warming!", txtLabel, QMessageBox::Close);
    }
    if (ok && !text.isEmpty()){
        QString s;
        for(auto i:text){
            if(i =='-')
                break;
            s+=i;
        }
        difficuly = "   Mode = "+s;
        setWindowTitle(title+" "+difficuly+" "+whofirst);
        if(text == "Journey--(2)")
            return 2;
        if(text == "Classic--(4)")
            return 4;
        if(text == "Expert--(6)--Per Steps need about 3s")
            return 6;
        if(text == "Master--(8)--Per Steps need about 8s")
            return 8;
    }
    return 2;
}
