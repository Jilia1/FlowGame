#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
using namespace sf;
using namespace std;

const int N = 5;
const int CELL = 100;
const int TOP = 80;
const int COLORS = 4;
const int LEVELS = 100;

Color colors[COLORS] = {
    Color(255, 105, 180),
    Color::White,
    Color(255, 215, 0),
    Color(100, 200, 255)
};
struct Point {
    int x;
    int y;
    int color;
};
vector<Point> dots;
vector<Point> paths[COLORS];

int level = 0;
int score = 0;
int currentColor = -1;
bool drawing = false;
Point last;

vector<Point> makeSnake()
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
vector<Point> createLevel(int number)
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

        auto change = [](Point p, int type)
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
        };
        a = change(a, transform);
        b = change(b, transform);

        result.push_back(a);
        result.push_back(b);
        start += lengths[color];
    }
    return result;
}
void reset()
{
    dots = createLevel(level);

    for (int i = 0; i < COLORS; i++)
        paths[i].clear();
    currentColor = -1;
    drawing = false;
}
bool isDot(int x, int y, int& color)
{
    for (const Point& p : dots)
    {
        if (p.x == x && p.y == y)
        {
            color = p.color;
            return true;
        }
    }
    return false;
}
int occupiedBy(int x, int y)
{
    for (int color = 0; color < COLORS; color++)
    {
        for (const Point& p : paths[color])
        {
            if (p.x == x && p.y == y)
                return color;
        }
    }
    return -1;
}
bool connected(int color)
{
    if (paths[color].empty())
        return false;

    Point first = dots[color * 2];
    Point second = dots[color * 2 + 1];

    bool hasFirst = false;
    bool hasSecond = false;

    for (const Point& p : paths[color])
    {
        if (p.x == first.x && p.y == first.y)
            hasFirst = true;
        if (p.x == second.x && p.y == second.y)
            hasSecond = true;
    }
    return hasFirst && hasSecond;
}
bool completed()
{
    int cells = 0;

    for (int color = 0; color < COLORS; color++)
    {
        if (!connected(color))
            return false;

        cells += static_cast<int>(paths[color].size());
    }
    return cells == N * N;
}
void drawLine(RenderWindow& window, Point a, Point b, Color color)
{
    float x1 = a.x * CELL + CELL / 2.f;
    float y1 = TOP + a.y * CELL + CELL / 2.f;
    float x2 = b.x * CELL + CELL / 2.f;
    float y2 = TOP + b.y * CELL + CELL / 2.f;
    
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx) * 180.f / 3.14159265f;

    RectangleShape line;
    line.setSize(Vector2f(length, 38));
    line.setOrigin(Vector2f(0, 19));
    line.setPosition(Vector2f(x1, y1));
    line.setRotation(degrees(angle));
    line.setFillColor(color);
    window.draw(line);

    CircleShape round(19);
    round.setOrigin(Vector2f(19, 19));
    round.setFillColor(color);
    round.setPosition(Vector2f(x1, y1));
    window.draw(round);
    round.setPosition(Vector2f(x2, y2));
    window.draw(round);
}
void drawDot(RenderWindow& window, Point p)
{
    float x = p.x * CELL + CELL / 2.f;
    float y = TOP + p.y * CELL + CELL / 2.f;

    CircleShape outer(28);
    outer.setOrigin(Vector2f(28, 28));
    outer.setPosition(Vector2f(x, y));
    outer.setFillColor(colors[p.color]);
    window.draw(outer);

    CircleShape middle(18);
    middle.setOrigin(Vector2f(18, 18));
    middle.setPosition(Vector2f(x, y));
    middle.setFillColor(Color(25, 28, 35));
    window.draw(middle);

    CircleShape center(9);
    center.setOrigin(Vector2f(9, 9));
    center.setPosition(Vector2f(x, y));
    center.setFillColor(colors[p.color]);
    window.draw(center);
}
void drawBoard(RenderWindow& window)
{
    window.clear(Color(25, 28, 35));

    RectangleShape top;
    top.setSize(Vector2f(N * CELL, TOP));
    top.setPosition(Vector2f(0, 0));
    top.setFillColor(Color(35, 40, 50));
    window.draw(top);

    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            RectangleShape cell;
            cell.setSize(Vector2f(CELL - 4, CELL - 4));
            cell.setPosition(Vector2f(
                x * CELL + 2,
                TOP + y * CELL + 2
            ));
            cell.setFillColor(Color(48, 53, 63));
            window.draw(cell);
        }
    }
    for (int color = 0; color < COLORS; color++)
    {
        for (int i = 1; i < static_cast<int>(paths[color].size()); i++)
        {
            drawLine(
                window,
                paths[color][i - 1],
                paths[color][i],
                colors[color]
            );
        }
    }
    for (const Point& p : dots)
        drawDot(window, p);

    window.display();
}
void updateTitle(RenderWindow& window)
{
    window.setTitle(
        "Flow Free | Level " +
        to_string(level + 1) +
        " | Score: " +
        to_string(score) +
        " | R - restart"
    );
}
int main()
{
    RenderWindow window(
        VideoMode({
            N * CELL,
            N * CELL + TOP
        }),
        "Flow Free"
    );
    window.setFramerateLimit(60);
    reset();
    updateTitle(window);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

            if (event->is<Event::KeyPressed>())
            {
                auto key = event->getIf<Event::KeyPressed>();

                if (key->code == Keyboard::Key::R)
                {
                    reset();
                    updateTitle(window);
                }
            }
            if (event->is<Event::MouseButtonPressed>())
            {
                auto mouse =
                    event->getIf<Event::MouseButtonPressed>();
                if (mouse->button == Mouse::Button::Left)
                {
                    int x = mouse->position.x / CELL;
                    int y = (mouse->position.y - TOP) / CELL;
                    int color;
                    if (x >= 0 && x < N &&
                        y >= 0 && y < N &&
                        isDot(x, y, color))
                    {
                        paths[color].clear();
                        paths[color].push_back({x, y, color});
                        currentColor = color;last = {x, y, color};
                        drawing = true;
                    }
                }
            }
            if (event->is<Event::MouseMoved>() && drawing)
            {
                auto mouse =
                    event->getIf<Event::MouseMoved>();

                int x = mouse->position.x / CELL;
                int y = (mouse->position.y - TOP) / CELL;
                if (x < 0 || x >= N ||
                    y < 0 || y >= N)
                    continue;

                int dx = abs(x - last.x);
                int dy = abs(y - last.y);

                if (dx + dy != 1)
                    continue;

                int dotColor;

                if (isDot(x, y, dotColor) &&
                    dotColor != currentColor)
                    continue;

                int other = occupiedBy(x, y);

                if (other != -1 &&
                    other != currentColor)
                    continue;

                if (other == currentColor)
                {
                    int index = -1;

                    for (int i = 0;
                         i < static_cast<int>(
                             paths[currentColor].size());
                         i++)
                    {
                        if (paths[currentColor][i].x == x &&
                            paths[currentColor][i].y == y)
                        {
                            index = i;
                            break;
                        }
                    }
                    if (index != -1)
                    {
                        paths[currentColor].resize(index + 1);
                        last = {x, y, currentColor};
                    }
                    continue;
                }
                paths[currentColor].push_back(
                    {x, y, currentColor}
                );
                last = {x, y, currentColor};
            }
            if (event->is<Event::MouseButtonReleased>())
            {
                auto mouse =
                    event->getIf<Event::MouseButtonReleased>();
                if (mouse->button == Mouse::Button::Left)
                {
                    drawing = false;
                    if (completed())
                    {
                        score += 100;
                        level++;
                        if (level >= LEVELS)
                            level = 0;

                        reset();
                        updateTitle(window);
                    }
                    currentColor = -1;
                }
            }
        }
        drawBoard(window);
    }
    return 0;
}