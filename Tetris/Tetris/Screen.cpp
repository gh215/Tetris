#include "Tetris.h"

void Screen::showConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void Screen::drawRect(int x, int y, int width, int height, symbol border)
{
    if (x < 0 || y < 0 || x + width > FIELD_WIDTH || y + height > FIELD_HEIGHT)
    {
        return;
    }

    for (int row = y; row < y + height; ++row)
    {
        if (row == y || row == y + height - 1)
        {
            for (int col = x; col < x + width; ++col)
            {
                drawSymb(border.first, logicalToPhysicalX(col), row);
            }
        }
        else
        {
            drawSymb(border.second, logicalToPhysicalX(x), row);
            drawSymb(border.second, logicalToPhysicalX(x + width - 1), row);
        }
    }
}

void Screen::clearPauseMessage()
{
    const int messageWidth = 14;
    const int messageHeight = 5;

    int centerX = GAME_FIELD_WIDTH / 2;
    int centerY = FIELD_HEIGHT / 2;

    int startX = centerX - messageWidth / 2;
    int startY = centerY - messageHeight / 2;

    for (int y = startY; y < startY + messageHeight; y++)
    {
        for (int x = startX; x < startX + messageWidth; x++)
        {
            if (x < FIELD_WIDTH && y < FIELD_HEIGHT)
            {
                nextBuffer[y][x] = EMPTY_CELL;
                activeBuffer[y][x] = EMPTY_CELL;
                drawSymb(EMPTY_CELL, x, y);
            }
        }
    }
}

void Screen::boardMessage(string message)
{
    const int messageWidth = 14;
    const int messageHeight = 5;

    int centerX = GAME_FIELD_WIDTH / 2;
    int centerY = FIELD_HEIGHT / 2;

    int startX = centerX - messageWidth / 2;
    int startY = centerY - messageHeight / 2;

    for (int y = startY; y < startY + messageHeight; y++)
    {
        for (int x = startX; x < startX + messageWidth; x++)
        {
            drawSymb(' ', x, y);
        }
    }

    for (int y = startY; y < startY + messageHeight; y++)
    {
        for (int x = startX; x < startX + messageWidth; x++)
        {
            if (y == startY || y == startY + messageHeight - 1)
            {
                drawSymb('-', x, y);
            }
            else if (x == startX || x == startX + messageWidth - 1)
            {
                drawSymb('|', x, y);
            }
        }
    }

    int messageX = startX + (messageWidth - message.length()) / 2;
    int messageY = startY + messageHeight / 2;
    for (size_t i = 0; i < message.size(); i++)
    {
        drawSymb(message[i], messageX + static_cast<int>(i), messageY);
    }
}

void Screen::showPauseMessage()
{
    string message = "PAUSE";
    boardMessage(message);
}

void Screen::showGameOverMessage()
{
    string message = "GAME OVER.\nTry again?";
    boardMessage(message);
}

void Screen::moveCursorToBottom()
{
    COORD endPosition = { 0, (SHORT)(FIELD_HEIGHT / 3) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), endPosition);
}

