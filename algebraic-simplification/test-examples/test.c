#include <stdio.h>

int main() {
    int x = 10;
    int a = x + 0;
    int b = 0 + x;
    int c = x + (-x);

    int d = x - 0;
    int f = 0 - x;
    int g = x - x;

    int h = x * 0;
    int i = 0 * x;
    int j = x * 1;
    int k = 1 * x;
    int l = x * (-1);
    int m = (-1) * x;

    int n = x / 1;
    int o = 0 / x;
    int p = x / x;

    return 0;
}