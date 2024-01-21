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

int main() {
    int a = 1;
    int b = 3;
    int c;
    sum(a, b, c);
    sum(a, b);
    float d = 1.0;
    float e = 2.0;
    sum(d, e);
}