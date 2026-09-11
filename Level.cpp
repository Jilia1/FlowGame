#include "Level.h"
using namespace std;

vector<Point> Level::makeSnake()
{
    vector<Point> snake;
    for (int y = 0; y < N; y++)
    {
        if (y % 2 == 0)
        {
            for (int x = 0; x < N; x++)
                snake.push_back({x, y, 0});
        }
        else
        {
            for (int x = N - 1; x >= 0; x--)
                snake.push_back({x, y, 0});
        }
    }
    return snake;
}
Point Level::change(Point p, int type)
{
    int x = p.x;
    int y = p.y;

    if (type == 1)
        x = N - 1 - x;
    else if (type == 2)
        y = N - 1 - y;
    else if (type == 3)
    {
        x = N - 1 - x;
        y = N - 1 - y;
    }
    else if (type == 4)
    {
        int oldX = x;
        x = N - 1 - y;
        y = oldX;
    }
    else if (type == 5)
    {
        int oldX = x;
        x = y;
        y = N - 1 - oldX;
    }
    else if (type == 6)
    {
        int oldX = x;
        x = N - 1 - y;
        y = N - 1 - oldX;
    }
    else if (type == 7)
    {
        x = N - 1 - x;
    }

    p.x = x;
    p.y = y;

    return p;
}
vector<Point> Level::createLevel(int number)
{
    vector<Point> snake = makeSnake();
    vector<Point> result;

    int lengths[COLORS] = {7, 6, 6, 6};
    int start = 0;
    int transform = number % 8;

    for (int color = 0; color < COLORS; color++)
    {
        Point a = snake[start];
        Point b = snake[start + lengths[color] - 1];

        a.color = color;
        b.color = color;

        a = change(a, transform);
        b = change(b, transform);

        result.push_back(a);
        result.push_back(b);

        start += lengths[color];
    }

    return result;
}