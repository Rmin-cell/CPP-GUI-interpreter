#include "tinyexpr.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    double x, y;
    /* Store variable names and pointers. */
    te_variable vars[] = {{"x", &x}, {"y", &y}};

    int err;
    /* Compile the expression with variables. */
    // te_expr *expr = te_compile("sqrt(x^2+y^2)", vars, 2, &err);

    const char *c = "x^2+7^2+11^2+(8-2)^2";
    // double r = te_interp(c, 0);
    te_expr *expr = te_compile(c, vars, 2, &err);

    if (expr) {
        x = 100; y = 4;
        const double h1 = te_eval(expr); /* Returns 5. */

        // x = 5; y = 12;
        // const double h2 = te_eval(expr); /* Returns 13. */

        te_free(expr);

        printf("h1: %f\n", h1);
        // printf("h2: %f\n", h2);
    } else {
        printf("Parse error at %d\n", err);
    }

    // printf("The expression:\n\t%s\nevaluates to:\n\t%f\n", c, r);
    // return 0;
}
