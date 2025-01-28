#ifdef __cplusplus
extern "C" {
#endif

#ifndef NUMBER_OF_DATA_POINTS
#define NUMBER_OF_DATA_POINTS 100
#endif

typedef struct te_expr {
    int type;
    union {double value; const double *bound; const void *function;};
    void *parameters[1];
} te_expr;

typedef struct te_variable {
    const char *name;
    const void *address;
    int type;
    void *context;
} te_variable;

typedef struct {
    const char *name;
    double min;
    double max;
} ui_variable;

/* Parses the input expression, evaluates it, and frees it. */
/* Returns NaN on error. */
double te_interp(const char *expression, int *error);

/* Parses the input expression and binds variables. */
/* Returns NULL on error. */
te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error);

/* Evaluates the expression. */
double te_eval(const te_expr *n);

/* Prints debugging information on the syntax tree. */
void te_print(const te_expr *n);

/* Frees the expression. */
/* This is safe to call on NULL pointers. */
void te_free(te_expr *n);

int extract_variables(const char *, char [][256], int *);

char** parse_tokens(const char *);

double** generate_point_grids(const char *, int, const ui_variable *);

double* generate_data_points(const char *, int, const ui_variable *, double**);

double* get_data_points(const char *, int, const ui_variable *);

#ifdef __cplusplus
}
#endif
