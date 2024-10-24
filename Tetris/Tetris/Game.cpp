#include "Tetris.h"

void Game::update()
{
    field.drawBorders();
    moveFigure(Direction::DOWN);
}

void Game::run()
{
    spawnFigure();
    while (true)
    {
        processInput();
        update();
        draw(field);
        Sleep(SLEEP);
    }
}

void Game::draw(Screen& field)
{
    currentFigure.draw(field);
    field.draw();
}

void Game::processInput()
{
    if (_kbhit())
    {
        int key = _getch();
        if (key == ARROW)
        {
            key = _getch();
            if (key == LEFT)
            {
                moveFigure(Direction::LEFT);
            }
            else if (key == RIGHT)
            {
                moveFigure(Direction::RIGHT);
            }
        }
        else if (key == COUNTER_CLOCK)
        {
            rotateFigure(Direction::LEFT);
        }
        else if (key == CLOCK)
        {
            rotateFigure(Direction::RIGHT);
        }
    }
}

vector<vector<int>> Game::rotateClockwise(const vector<vector<int>>& shape)
{
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<int>> rotated(cols, vector<int>(rows));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            rotated[j][rows - 1 - i] = shape[i][j];
        }
    }

    return rotated;
}

vector<vector<int>> Game::rotateCounterClockwise(const vector<vector<int>>& shape)
{
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<int>> rotated(cols, vector<int>(rows));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            rotated[cols - 1 - j][i] = shape[i][j];
        }
    }

    return rotated;
}
