#include "tinyexpr.h"
#include <stdio.h>

// #ifndef MAX_VARIABLES
// #define MAX_VARIABLES 10
// #endif

// #ifndef MAX_INPUT_SIZE
// #define MAX_INPUT_SIZE 256
// #endif

int main(int argc, char *argv[]) {
    // double x, y;
    char *c = "x + y";

    // char **tokens = parse_tokens(c);

    // if (tokens != NULL) {
    //     // Loop through the tokens until NULL is encountered
    //     for (size_t i = 0; tokens[i] != NULL; i++) {
    //         printf("Token %zu: %s\n", i, tokens[i]);
    //     }

    //     // Free memory after printing
    //     for (size_t i = 0; tokens[i] != NULL; i++) {
    //         free(tokens[i]); // Free each string
    //     }
    //     free(tokens); // Free the array itself
    // }

    ui_variable ui_v[] = {
        {"x", 0, 1},
        {"y", 1, 2}
    };

    size_t number_of_variables = sizeof(ui_v) / sizeof(ui_variable);

    double **data_point_grids = generate_point_grids(c, number_of_variables, ui_v);

    // for (size_t i = 0; data_point_grids[i] != NULL; i++) {
    //     printf("ui_v[%d] -> [", i);
    //     for (size_t j = 0; j < number_of_variables; j++) {
    //         printf("[%f]", data_point_grids[i][j]);
    //         if (j != number_of_variables - 1) printf(" ");
    //     }
    //     printf("]\n");
    // }

    // double *points = generate_data_points(c, number_of_variables, ui_v, data_point_grids);

    double *points = get_data_points(c, number_of_variables, ui_v);

    for (size_t i = 0; i < 10000; i++) {
        if (points[i] > 0.0) {
            printf("[%f]\n", points[i]);
        }
    }

    return 0;
}
