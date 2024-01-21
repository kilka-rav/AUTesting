#include <iostream>


void sum(int& a, int& b, int& res) {
    res = a + b;
}

int sum(int a, int b) {
    return a + b;
}

template <typename T>
T sum(T a, T b) {
    return a + b;
}
