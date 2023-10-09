#include "GameModel.h"
#include <utility>
#include <time.h>
GameModel::GameModel()
{

}

void GameModel::startGame(GameType type)
{
    gameType = type;

    gameMap.clear();

    for(int i=0;i<kBoardSize;i++)
    {
        std::vector<int> lineBoard;
        for(int j=0;j<kBoardSize;j++)
        {
            lineBoard.push_back(0);
        }
        gameMap.push_back(lineBoard);
    }

    if(gameType==bot)
    {
        scoreMap.clear();

        for(int i=0;i<kBoardSize;i++)
        {
            std::vector<int> lineScores;
            for(int j=0;j<kBoardSize;j++)
            {
                lineScores.push_back(0);
            }
            scoreMap.push_back(lineScores);
        }
    }
    playerFlag=true;
}

void GameModel::updateMap(int row, int col)
{
    if(playerFlag)
        gameMap[row][col]=1;
    else
        gameMap[row][col]=-1;

    playerFlag=!playerFlag;
}

void GameModel::actionByPerson(int row, int col)
{
    updateMap(row,col);
}

void GameModel::actionByAi(int &crow, int &ccol)
{
    calculateScore();
    int maxScore=0;
    std::vector<std::pair<int,int>> maxPoint;

    for(int i=1;i<kBoardSize;i++)
        for(int j=1;j<kBoardSize;j++)
        {
            if(gameMap[i][j]==0)
            {
                if(scoreMap[i][j]>maxScore)
                {
                    maxPoint.clear();
                    maxScore=scoreMap[i][j];
                    maxPoint.push_back(std::make_pair(i,j));
                }
                else if(scoreMap[i][j]==maxScore)
                    maxPoint.push_back(std::make_pair(i,j));
            }
        }
    srand((unsigned)time(0));
    int index=rand()%maxPoint.size();
    std::pair<int,int> pointPair=maxPoint.at(index);

    crow=pointPair.first;
    ccol=pointPair.second;
    updateMap(crow,ccol);
}

void GameModel::calculateScore()
{
    int personNum=0;
    int botNum=0;
    int emptyNum=0;

    scoreMap.clear();
    for(int i=0;i<kBoardSize;i++)
    {
        std::vector<int> lineScore;
        for(int j=0;j<kBoardSize;j++)
            lineScore.push_back(0);
        scoreMap.push_back(lineScore);
    }

    for(int row=0;row<kBoardSize;row++)
        for(int col=0;col<kBoardSize;col++)
        {
            if(row>0&&col>0&&gameMap[row][col]==0)
            {
                for(int y=-1;y<=1;y++)
                    for(int x=-1;x<=1;x++)
                    {
                        int personNum=0;
                        int botNum=0;
                        int emptyNum=0;

                        if(!(y==0&&x==0))
                        {
                            for(int i=1;i<=4;i++)
                            {
                                if(row+i*y>0&&row+i*y<kBoardSize&&col+i*x>0&&col+i*x<kBoardSize&&gameMap[row+i*y][col+i*x]==1)
                                {
                                    personNum++;
                                }
                                else if(row+i*y>0&&row+i*y<kBoardSize&&col+i*x>0&&col+i*x<kBoardSize&&gameMap[row+i*y][col+i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }
                            for(int i=1;i<=4;i++)
                            {
                                if(row-i*y>0&&row-i*y<kBoardSize&&col-i*x>0&&col-i*x<kBoardSize&&gameMap[row-i*y][col-i*x]==1)
                                {
                                    personNum++;
                                }
                                else if(row-i*y>0&&row-i*y<kBoardSize&&col-i*x>0&&col-i*x<kBoardSize&&gameMap[row-i*y][col-i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }
                            if(personNum==1)
                                scoreMap[row][col]+=10;
                            else if(personNum==2)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=30;
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=40;
                            }
                            else if(personNum==3)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=60;
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=110;
                            }
                            else if(personNum==4)
                                scoreMap[row][col]+=10000;
                            emptyNum=0;


                            for(int i=1;i<=4;i++)
                            {
                                if(row+i*y>0&&row+i*y<kBoardSize&&col+i*x>0&&col+i*x<kBoardSize&&gameMap[row+i*y][col+i*x]==1)
                                {
                                    botNum++;
                                }
                                else if(row+i*y>0&&row+i*y<kBoardSize&&col+i*x>0&&col+i*x<kBoardSize&&gameMap[row+i*y][col+i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }
                            for(int i=1;i<=4;i++)
                            {
                                if(row-i*y>0&&row-i*y<kBoardSize&&col-i*x>0&&col-i*x<kBoardSize&&gameMap[row-i*y][col-i*x]==-1)
                                {
                                   botNum++;
                                }
                                else if(row-i*y>0&&row-i*y<kBoardSize&&col-i*x>0&&col-i*x<kBoardSize&&gameMap[row-i*y][col-i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }
                            if(botNum==0)
                                scoreMap[row][col]+=5;
                            else if(botNum==1)
                                scoreMap[row][col]+=10;
                            else if(botNum==2)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=25;
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=50;

                            }
                            else if(botNum==3)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=50;
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=110;
                            }
                            else if(botNum==4)
                                scoreMap[row][col]+=10000;
                        }
                    }
            }
        }
}

bool GameModel::isWin(int row, int col)
{
    for(int i=0;i<5;i++)
    {
        if(col-i>0&&col-i+4<kBoardSize&&gameMap[row][col-i]==gameMap[row][col-i+1]&&gameMap[row][col-i]==gameMap[row][col-i+2]&&gameMap[row][col-i]==gameMap[row][col-i+3]&&gameMap[row][col-i]==gameMap[row][col-i+4])
            return true;
    }
    for(int i=0;i<5;i++)
    {
        if(row-i>0&&row-i+4<kBoardSize&&gameMap[row-i][col]==gameMap[row-i+1][col]&&gameMap[row-i][col]==gameMap[row-i+2][col]&&gameMap[row-i][col]==gameMap[row-i+3][col]&&gameMap[row-i][col]==gameMap[row-i+4][col])
            return true;
    }
    for(int i=0;i<5;i++)
    {
        if(row+i<kBoardSize&&row+i-4>0&&col-i>0&&col-i+4<kBoardSize&&gameMap[row+i][col-i]==gameMap[row+i-1][col-i+1]&&gameMap[row+i][col-i]==gameMap[row+i-2][col-i+2]&&gameMap[row+i][col-i]==gameMap[row+i-3][col-i+3]&&gameMap[row+i][col-i]==gameMap[row+i-4][col-i+4])
            return true;
    }
    for(int i=0;i<5;i++)
    {
        if(row-i>0&&row-i+4<kBoardSize&&col-i>0&&col-i+4<kBoardSize&&gameMap[row-i][col-i]==gameMap[row-i+1][col-i+1]&&gameMap[row-i][col-i]==gameMap[row-i+2][col-i+2]&&gameMap[row-i][col-i]==gameMap[row-i+3][col-i+3]&&gameMap[row-i][col-i]==gameMap[row-i+4][col-i+4])
            return true;
    }
    return false;

}

bool GameModel::isDeadGame()
{
    for(int i=1;i<kBoardSize;i++)
        for(int j=1;j<kBoardSize;j++)
        {
            if(!(gameMap[i][j]==1||gameMap[i][j]==-1))
                return false;
        }
    return true;
}

