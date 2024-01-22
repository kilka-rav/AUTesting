#include <iostream>
#include <cmath>


class Point {
    float x;
    float y;
public:
    Point(float _x, float _y) : x(_x), y(_y) {}
    double distance(Point& p1, Point& p2);
    float distance(Point& p1, Point& p2);
    bool isEqual(Point& p1, Point& p2) {
        return (p1.x - p2.x < 0.0001) && (p1.y - p2.y < 0.0001);
    }
    bool isNotEqual(Point& p1, Point& p2) const {
        return (p1.x - p2.x < 0.0001) && (p1.y - p2.y < 0.0001);
    }
private:
    double getX() {
        return x;
    }
    double getY() const {
        return y;
    }
};

double Point::distance(Point& p1, Point& p2) {
    return std::sqrt((p1.x - p2.x) *  (p1.x - p2.x) + (p1.y - p2.y) *  (p1.y - p2.y));
}

float Point::distance(Point& p1, Point& p2) {
    return std::sqrt((p1.x - p2.x) *  (p1.x - p2.x) + (p1.y - p2.y) *  (p1.y - p2.y));
}

static int abs(int a) {return a;}
static double abs(double a) 
{
    return a;
}

int max(std::vector<int>& vec) {
    int max = vec[0];
    for(int& t : vec) {
        if (t > max) {
            max = t;
        }
    }
    return max;
}