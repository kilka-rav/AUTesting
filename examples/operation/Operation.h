#include <stdio.h>

double add(double a, double b);

float add(float a, float b);

int add(int a, int b);

double sub(double a, double b);

float sub(float a, float b);

int sub(int a, int b);

struct Point {
    double x;
    double y;
};

double square_distance(Point* a, Point* b);
