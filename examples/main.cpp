#include <cassert>
#include <iostream>

int sum(int a, int b) {
    return a + b;
}

int main() {
    assert(sum(3, 4) == 7);
}