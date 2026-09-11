#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Point.h"
#include "Level.h"
using namespace std;
using namespace sf;

class Game
{
private:
    static const int N = 5;
    static const int CELL = 100;
    static const int TOP = 80;
    static const int COLORS = 4;
    static const int LEVELS = 100;

    RenderWindow window;
    Level levelGenerator;

    vector<Point> dots;
    vector<Point> paths[COLORS];

    Color colors[COLORS];

    int level;
    int score;
    int currentColor;
    bool drawing;
    Point last;

    void reset();
    bool isDot(int x, int y, int& color);
    int occupiedBy(int x, int y);
    bool connected(int color);
    bool completed();

    void drawLine(Point a, Point b, Color color);
    void drawDot(Point p);
    void drawBoard();
    void updateTitle();

    void handleEvents();
    void startDrawing(int x, int y);
    void continueDrawing(int x, int y);
    void stopDrawing();

public:
    Game();
    void run();
};