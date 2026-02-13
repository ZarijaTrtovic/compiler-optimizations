#include <stdio.h>

void test(int x) {
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

    
    printf("a = %d\nb = %d\nc = %d\n\nd = %d\nf = %d\ng = %d\n\nh = %d\ni = %d\nj = %d\nk = %d\nl = %d\nm = %d\n\nn = %d\no = %d\np = %d\n", 
       a, b, c, d, f, g, h, i, j, k, l, m, n, o, p);
}

int main() {
    test(10);
    return 0;
}
