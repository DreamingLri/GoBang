#include "mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QDebug>
#include <math.h>


const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限

const int kAIDelay = 700; // AI下棋的思考时间

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setFixedSize(kBoardMargin*2+kBlockSize*kBoardSize,kBoardMargin*2+kBlockSize*kBoardSize);
    setMouseTracking(true);
    initGame();
}

MainWindow::~MainWindow()
{

    if(game)
    {
        delete game;
        game=nullptr;
    }
}

void MainWindow::initGame()
{
    game = new GameModel;
    initPVPGame();
}

void MainWindow::initPVPGame()
{
    game_type=person;
    game->gameStatus=playing;
    game->startGame(game_type);
    update();

}

void MainWindow::initPVCGame()
{
    game_type=bot;
    game->gameStatus=playing;
    game->startGame(game_type);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing,true);

    for(int i=0;i<kBoardSize+1;i++)
    {
        painter.drawLine(kBoardMargin+kBlockSize*i,kBoardMargin,kBoardMargin+kBlockSize*i,size().height()-kBoardMargin);
        painter.drawLine(kBoardMargin,kBoardMargin+kBlockSize*i,size().width()-kBoardMargin,kBoardMargin+kBlockSize*i);
    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow>0&&clickPosRow<kBoardSize&&clickPosCol>0&&clickPosCol<kBoardSize&&game->gameMap[clickPosRow][clickPosCol]==0)
    {
        if(game->playerFlag)
            brush.setColor(Qt::white);
        else
            brush.setColor(Qt::black);
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin+kBlockSize*clickPosCol-kMarkSize/2,kBoardMargin+kBlockSize*clickPosRow-kMarkSize/2,kMarkSize,kMarkSize);
    }

    for(int i=0;i<kBoardSize;i++)
        for(int j=0;j<kBoardSize;j++)
        {
            if(game->gameMap[i][j]==1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin+kBlockSize*j-kRadius,kBoardMargin+kBlockSize*i-kRadius,kRadius*2,kRadius*2);
            }
            else if(game->gameMap[i][j]==-1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin+kBlockSize*j-kRadius,kBoardMargin+kBlockSize*i-kRadius,kRadius*2,kRadius*2);
            }
        }

    if(clickPosRow>0&&clickPosRow<kBoardSize&&clickPosCol>0&&clickPosCol<kBoardSize&&(game->gameMap[clickPosRow][clickPosCol]==1||game->gameMap[clickPosRow][clickPosCol]==-1))
    {
        if(game->isWin(clickPosRow,clickPosCol)&&game->gameStatus==playing)
        {
            qDebug()<<"win";
            game->gameStatus=win;

            QString str;
            if(game->gameMap[clickPosRow][clickPosCol]==1)
                str="white player";
            else if(game->gameMap[clickPosRow][clickPosCol]==-1)
                str="black player";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this,"congratulations",str+"win!");

            if(btnValue==QMessageBox::Ok)
            {
                game->startGame(game_type);
                game->gameStatus=playing;
            }
        }
    }
    if(game->isDeadGame())
    {
        QMessageBox::StandardButton btnValue=QMessageBox::information(this,"oops","dead game");
        if(btnValue==QMessageBox::Ok)
        {
            game->startGame(game_type);
            game->gameStatus=playing;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();

    if(x>=kBoardMargin+kBlockSize/2&&x<size().width()-kBoardMargin&&y>=kBoardMargin+kBlockSize/2&&y<size().height()-kBoardMargin)
    {
        int col=x/kBlockSize;
        int row=y/kBlockSize;

        int leftTopPosX=kBoardMargin+kBlockSize*col;
        int leftTopPosY=kBoardMargin+kBlockSize*row;

        clickPosRow=-1;
        clickPosCol=-1;
        int len=0;

        len=sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if(len<kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len=sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row ;
            clickPosCol = col + 1;
        }
        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY-kBlockSize)*(y-leftTopPosY-kBlockSize));
        if(len<kPosDelta)
        {
            clickPosRow=row+1;
            clickPosCol=col;
        }
        len=sqrt((x-leftTopPosX-kBlockSize)*(x-leftTopPosX-kBlockSize)+(y-leftTopPosY-kBlockSize)*(y-leftTopPosY-kBlockSize));
        if(len<kPosDelta)
        {
            clickPosRow=row+1;
            clickPosCol=col+1;
        }

    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (!(game_type == bot && !game->playerFlag))
    {
        chessOneByPerson();
    }

}

void MainWindow::chessOneByPerson()
{
    if(clickPosRow!=-1&&clickPosCol!=-1&&game->gameMap[clickPosRow][clickPosCol]==0)
    {
        game->actionByPerson(clickPosRow,clickPosCol);
        update();
    }
}

