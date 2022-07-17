#include <stdio.h>

extern int func(int, int);

int add(int a, int b) {
    return (a + b);
}

int main(int argc, const char *argv[]) {
    int ret = 0;
    ret = func(1, 10);
    printf("The return value is %d. \n", ret);
    return 0;
}