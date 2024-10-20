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

void Figure::draw(GameField& field)
{
    for (int i = 0; i < shape.size(); ++i)
    {
        for (int j = 0; j < shape[i].size(); ++j)
        {
            if (shape[i][j] == 1)
            {
                // Рисуем левую скобку
                field.put(FILLED_CELL_LEFT, { position.x + j * 2, position.y + i });
                // Рисуем правую скобку рядом с левой
                field.put(FILLED_CELL_RIGHT, { position.x + j * 2 + 1, position.y + i });
            }
        }
    }
}

vector<vector<int>> Figure::rotateClockwise(const vector<vector<int>>& shape)
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

vector<vector<int>> Figure::rotateCounterClockwise(const vector<vector<int>>& shape)
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

void Figure::rotate(Direction dir)
{
    if (dir == Direction::LEFT)
    {
        shape = rotateCounterClockwise(shape);
    }
    else if (dir == Direction::RIGHT)
    {
        shape = rotateClockwise(shape);
    }
}