#include "Operation.h"


double add(double a, double b) {
    return a + b;
}

float add(float a, float b) {
    return a + b;
}

int add(int a, int b) {
    return a + b;
}

double sub(double a, double b) {
    return a - b;
}

float sub(float a, float b) {
    return a - b;
}

int sub(int a, int b) {
    return a - b;
}

double square_distance(struct Point* a, struct Point* b) {
    return (a->x - b->x) * (a->x - b->x) + (a->x - b->x) * (a->x - b->x);
}
