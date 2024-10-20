#include "Tetris.h"

void Game::update()
{
    field.moveFigure(Direction::DOWN);
    field.draw();
}

void Game::run()
{
    field.spawnFigure();
    while (true)
    {
        processInput();
        update();
        Sleep(SLEEP);
    }
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
                field.moveFigure(Direction::LEFT);
            }
            else if (key == RIGHT)
            {
                field.moveFigure(Direction::RIGHT);
            }
        }
        else if (key == COUNTER_CLOCK)
        {
            field.rotateFigure(Direction::LEFT);
        }
        else if (key == CLOCK)
        {
            field.rotateFigure(Direction::RIGHT);
        }
    }
}