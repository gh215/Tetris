#include "Tetris.h"

void Figure::move(Direction dir)
{
    switch (dir)
    {
    case Direction::DOWN:
        position.y++;
        break;
    case Direction::LEFT:
        position.x--;
        break;
    case Direction::RIGHT:
        position.x++;
        break;
    }
}