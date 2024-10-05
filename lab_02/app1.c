#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

double volume(double a, double b, double c)
{
    return a * b * c;
}

int main(void) {
    double a;
    printf("%d - введите длину прямоугольного параллелепипеда: ", getpid());
    if (scanf("%lf", &a) != 1) {
        printf("%d - Error: некорректный ввод\n", getpid());
        return EXIT_FAILURE;
    }

    double b;
    printf("%d - введите ширину прямоугольного параллелепипеда: ", getpid());
    if (scanf("%lf", &b) != 1) {
        printf("%d - Error: некорректный ввод\n", getpid());
        return EXIT_FAILURE;
    }

    double c;
    printf("%d - введите высоту прямоугольного параллелепипеда: ", getpid());
    if (scanf("%lf", &c) != 1) {
        printf("%d - Error: некорректный ввод\n", getpid());
        return EXIT_FAILURE;
    }

    printf("%d - объем прямоугольного параллелепипеда: %lf\n", getpid(), volume(a, b, c));

    return EXIT_SUCCESS;
}
