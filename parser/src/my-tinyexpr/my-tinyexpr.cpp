#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <vector>

using namespace std;

#ifndef NAN
#define NAN (0./0.)
#endif
#ifndef INFINITY
#define INFINITY (1./0.)
#endif
#ifndef PI
#define PI 3.14159265358979323846
#endif
#ifndef E
#define E 2.71828182845904523536
#endif

enum TOKENS {
    TOK_NULL, TOK_ERROR, TOK_END, TOK_SEP,
    TOK_OPEN, TOK_CLOSE, TOK_NUMBER, TOK_VARIABLE, 
    TOK_INFIX
};

enum NODE_TYPE {
    VARIABLE,
    FUNCTION,
    CLOSURE,
    FLAG_PURE
};

double pi() {return PI;}
double e() {return E;}
double fac(unsigned int a) {
    if (a <= 0) {
        return NAN;
    }
    unsigned long int result = 1;
    
    for (unsigned int i = 1; i <= a; i++) {
        result *= i;
    }

    return (double)result;
}

double add(double a, double b) {return a + b;}
double sub(double a, double b) {return a - b;}
double mul(double a, double b) {return a * b;}
double divide(double a, double b) {return a / b;}
double negate(double a) {return -a;}

class expr_parser {
private:
    typedef struct variable {
        const char *name;
        const void *address;
        int type;
    } variable;

    typedef struct state {
        char *start;
        char *next;
        int type;
        union {double value; const double *bound; const void *function;};
        void *context;
        variable *lookup;
        int lookup_len;
    } state;
    
    vector<variable> functions;

public:
    expr_parser();
    ~expr_parser();
    void define_functions();
    int get_len_of_predefined_functions();
    vector<char *> get_all_predefined_functions_name();
    variable* find_lookup(const state *, const char *, int);
    void next_token(state *);
};

expr_parser::expr_parser() {
    define_functions();
}

expr_parser::~expr_parser() {}

void expr_parser::define_functions() {
    functions = {
        {"abs", (const void*) (double (*)(double))fabs,          FUNCTION},
        {"acos", (const void*) (double (*)(double))acos,         FUNCTION},
        {"asin", (const void*) (double (*)(double))asin,         FUNCTION},
        {"atan", (const void*) (double (*)(double))atan,         FUNCTION},
        {"ceil", (const void*) (double (*)(double))ceil,         FUNCTION},
        {"cos", (const void*) (double (*)(double))cos,           FUNCTION},
        {"cosh", (const void*) (double (*)(double))cosh,         FUNCTION},
        {"e", (const void*) e,                                   FUNCTION},
        {"exp", (const void*) (double (*)(double))exp,           FUNCTION},
        {"fac", (const void*) fac,                               FUNCTION},
        {"floor", (const void*) (double (*)(double))floor,       FUNCTION},
        {"log", (const void*) (double (*)(double))log,           FUNCTION},
        {"pi", (const void*) pi,                                 FUNCTION},
        {"pow", (const void*) (double (*)(double, double))pow,   FUNCTION},
        {"sin", (const void*) (double (*)(double))sin,           FUNCTION},
        {"sinh", (const void*) (double (*)(double))sinh,         FUNCTION},
        {"sqrt", (const void*) (double (*)(double))sqrt,         FUNCTION},
        {"tan", (const void*) (double (*)(double))tan,           FUNCTION},
        {"tanh", (const void*) (double (*)(double))tanh,         FUNCTION},
    };
}

int expr_parser::get_len_of_predefined_functions() {
    return functions.size();
}

vector<char *> expr_parser::get_all_predefined_functions_name() {
    int len_of_predefined_functions = get_len_of_predefined_functions();
    vector<char *> name_of_predefined_functions(len_of_predefined_functions);
    
    for (int i = 0; i < len_of_predefined_functions; i++) {
        name_of_predefined_functions[i] = strdup(functions[i].name);
    }

    return name_of_predefined_functions;
}

expr_parser::variable* expr_parser::find_lookup(const state *s, const char *name, int len) {
    int iters;
    variable *var;
    if (!s->lookup) return NULL;

    for (var = s->lookup, iters = s->lookup_len; iters; ++var, --iters) {
        if (strncmp(name, var->name, len) == 0 && var->name[len] == '\0') {
            return var;
        }
    }
    return NULL;
}

void expr_parser::next_token(state *s) {
    s->type = TOK_NULL;
    
    while (s->type == TOK_NULL) {
        if (!*s->next) { // the string is finished
            s->type = TOK_END;
            return;
        }

        // reading a number
        if (('0' <=  s->next[0] && s->next[0] <= '9') || (s->next[0] == '.')) {
            s->value = strtod(s->next, (char**)&s->next);
            s->type = TOK_NUMBER;
        }
        else {
            // variable or bultin function
            if (isalpha(s->next[0])) {
                char *start = s->next;
                unsigned int size = 1;

                while (isalpha(s->next[0])) {
                    s->next++;
                    size++;
                }
                
                variable *var = find_lookup(s, start, size);

                if (var) {
                    // var;
                }
            }
        }
    }
}


void get_variable_names(const char *expr) {
    while (*expr) {
        while (*expr && (*expr < 'a' || *expr > 'z')) ++expr;
        const char *start = expr;
        if (*expr >= 'a' && *expr <= 'z') {
            while ((*expr >= 'a' && *expr <= 'z') || (*expr >= '0' && *expr <= '9')) ++expr;
            printf("%.*s\n", (int)(expr-start), start);
            ++expr;
        }
    }
}

int main() {
    // print_identifiers("a+b17*cat-dog(-3)*x+dd");
    // int len_of_predefined_functions = sizeof(functions) / sizeof(variable);
    // vector<char *> name_of_predefined_functions = get_all_predefined_functions_name(functions, len_of_predefined_functions);
    // for (int i = 0; i < len_of_predefined_functions; i++) {
    //     printf("%s\n", name_of_predefined_functions[i]);
    // }
    expr_parser ep = expr_parser();
    printf("%d", ep.get_len_of_predefined_functions());
    // printf("kiiir-0");
}
