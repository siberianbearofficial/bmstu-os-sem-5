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
    printf("%d - enter radius: ", getpid());
    if (scanf("%lf", &r) != 1) {
        printf("%d - Error: invalid input\n", getpid());
        return EXIT_FAILURE;
    }

    printf("%d - volume of sphere: %lf\n", getpid(), volume(r));

    return EXIT_SUCCESS;
}
