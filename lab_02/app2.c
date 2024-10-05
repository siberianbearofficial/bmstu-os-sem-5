#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"

double volume(double r)
{
    return 0.75 * M_PI * pow(r, 3);
}

int main(void) {
    double r;
    printf("%d - введите радиус сферы: ", getpid());
    if (scanf("%lf", &r) != 1) {
        printf("Error: некорректный ввод\n");
        return EXIT_FAILURE;
    }

    printf("%d - объем сферы: %lf\n", getpid(), volume(r));

    return EXIT_SUCCESS;
}
