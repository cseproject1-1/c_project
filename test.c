#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

static int factorial(int n) {
    if (n <= 1) return 1;
    int r = 1;
    for (int i = 2; i <= n; ++i) r *= i;
    return r;
}

int main(void) {
    puts("Compiler test program");

    printf("Source: %s\n", __FILE__);
#ifdef __VERSION__
    printf("Compiler version: %s\n", __VERSION__);
#endif
#ifdef __STDC__
    printf("ISO C (__STDC__) = %d\n", __STDC__);
#endif
#if defined(__STDC_VERSION__)
    printf("C standard (__STDC_VERSION__) = %ld\n", (long)__STDC_VERSION__);
#endif
    printf("Build date/time: %s %s\n", __DATE__, __TIME__);

    
    printf("Type sizes (bytes): char=%zu short=%zu int=%zu long=%zu long long=%zu float=%zu double=%zu size_t=%zu\n",
           sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(long long),
           sizeof(float), sizeof(double), sizeof(size_t));

    int f5 = factorial(5);
    printf("factorial(5) = %d\n", f5);
    assert(f5 == 120);

    int a = 42, b = 17;
    printf("%d + %d = %d\n", a, b, a + b);

    puts("All tests passed.");
    return 0;
}