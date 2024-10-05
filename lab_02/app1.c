#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

double volume(double a, double b, double c)
{
    return a * b * c;
}

int main(void) {
    double a;
    printf("%d - enter length: ", getpid());
    if (scanf("%lf", &a) != 1) {
        printf("%d - Error: invalid input\n", getpid());
        return EXIT_FAILURE;
    }

    double b;
    printf("%d - enter width: ", getpid());
    if (scanf("%lf", &b) != 1) {
        printf("%d - Error: invalid input\n", getpid());
        return EXIT_FAILURE;
    }

    double c;
    printf("%d - enter height: ", getpid());
    if (scanf("%lf", &c) != 1) {
        printf("%d - Error: invalid input\n", getpid());
        return EXIT_FAILURE;
    }

    printf("%d - volume of rectangular parallelepiped: %lf\n", getpid(), volume(a, b, c));

    return EXIT_SUCCESS;
}
