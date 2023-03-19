#include <stdio.h>

unsigned int func4(unsigned int a, unsigned int b, unsigned int c) {
    unsigned int temp = c - b;
	temp += temp >> 31;
	temp >>= 1;
	temp += b;
    if (temp > a) {
        return 2 * func4(a, b, temp - 1);
    }
    else if (temp < a) {
        return 2 * func4(a, temp + 1, c) + 1;
    }
    else return 0;
}

int main() {
    for (unsigned int i = 0; i <= 0xe; i++) {
        printf("func4(%u) reuslt is: %u\n", i, func4(i, 0x0, 0xe));
    }
    return 0;
}