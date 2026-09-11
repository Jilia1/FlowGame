#include "Game.h"
#include <cmath>
#include <string>
using namespace std;
using namespace sf;

Game::Game()
    : window(
        VideoMode({N * CELL, TOP + N * CELL}),
        "Flow Free"
      ),
      level(0),
      score(0),
      currentColor(-1),
      drawing(false)
{
    colors[0] = Color(255, 105, 180);
    colors[1] = Color::White;
    colors[2] = Color(255, 215, 0);
    colors[3] = Color(100, 200, 255);

    reset();
}
void Game::reset()
{
    dots = levelGenerator.createLevel(level);

    for (int i = 0; i < COLORS; i++)
        paths[i].clear();

    currentColor = -1;
    drawing = false;

    updateTitle();
}
bool Game::isDot(int x, int y, int& color)
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
int Game::occupiedBy(int x, int y)
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
bool Game::connected(int color)
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
bool Game::completed()
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
void Game::drawLine(Point a, Point b, Color color)
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
void Game::drawDot(Point p)
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
void Game::drawBoard()
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
                paths[color][i - 1],
                paths[color][i],
                colors[color]
            );
        }
    }
    for (const Point& p : dots)
        drawDot(p);

    window.display();
}
void Game::updateTitle()
{
    window.setTitle(
        "Flow Free | Level " +
        to_string(level + 1) +
        " | Score: " +
        to_string(score) +
        " | R - restart"
    );
}
void Game::startDrawing(int x, int y)
{
    if (x < 0 || x >= N || y < 0 || y >= N)
        return;

    int color;

    if (!isDot(x, y, color))
        return;

    currentColor = color;
    drawing = true;

    paths[color].clear();
    paths[color].push_back({x, y, color});

    last = {x, y, color};
}
void Game::continueDrawing(int x, int y)
{
    if (!drawing)
        return;

    if (x < 0 || x >= N || y < 0 || y >= N)
        return;

    if (abs(x - last.x) + abs(y - last.y) != 1)
        return;

    int dotColor;

    if (isDot(x, y, dotColor) && dotColor != currentColor)
        return;

    int occupied = occupiedBy(x, y);

    if (occupied != -1 && occupied != currentColor)
        return;

    for (size_t i = 0; i < paths[currentColor].size(); i++)
    {
        if (paths[currentColor][i].x == x &&
            paths[currentColor][i].y == y)
        {
            paths[currentColor].resize(i + 1);
            last = {x, y, currentColor};
            return;
        }
    }
    paths[currentColor].push_back({x, y, currentColor});
    last = {x, y, currentColor};
}
void Game::stopDrawing()
{
    if (!drawing)
        return;

    drawing = false;

    if (!completed())
    {
        updateTitle();
        return;
    }
    score += 100;
    currentColor = -1;

    if (level + 1 >= LEVELS)
        level = 0;
    else
        level++;
    reset();
}
void Game::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
        {
            window.close();
        }
        if (auto key = event->getIf<Event::KeyPressed>())
        {
            if (key->code == Keyboard::Key::R)
                reset();
        }
        if (auto mouse = event->getIf<Event::MouseButtonPressed>())
        {
            if (mouse->button == Mouse::Button::Left)
            {
                int x = mouse->position.x / CELL;
                int y = (mouse->position.y - TOP) / CELL;

                if (x >= 0 && x < N &&
                    y >= 0 && y < N)
                {
                    startDrawing(x, y);
                }
            }
        }
        if (auto mouse = event->getIf<Event::MouseMoved>())
        {
            int x = mouse->position.x / CELL;
            int y = (mouse->position.y - TOP) / CELL;

            continueDrawing(x, y);
        }
        if (auto mouse = event->getIf<Event::MouseButtonReleased>())
        {
            if (mouse->button == Mouse::Button::Left)
                stopDrawing();
        }
    }
}
void Game::run()
{
    while (window.isOpen())
    {
        handleEvents();
        drawBoard();
    }
}