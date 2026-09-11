#pragma once
#include "Point.h"
#include <vector>
using namespace std;

class Level
{
private:
    static const int N = 5;
    static const int COLORS = 4;
    vector<Point> makeSnake();
    Point change(Point p, int type);

public:
    vector<Point> createLevel(int number);
};