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


//回溯法判断输赢
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

