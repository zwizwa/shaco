
#include "shaco.h"
#include <stdlib.h>
#include <stdio.h>


/* This struct contains all "local variables" for function parse().

   It's best to not use real local variables in the function that uses
   the SC_WAIT() macro. */
struct vars {
    int i, nb_i;
    int j, nb_j;
    int a;
};

void parse(struct shaco *sc, struct vars *v) {
    SC_BEGIN(sc);
    SC_WAIT(sc, v->nb_i);
    for (v->i = 0; v->i < v->nb_i; v->i++) {
        SC_WAIT(sc, v->nb_j);
        for (v->j = 0; v->j < v->nb_j; v->j++) {
            SC_WAIT(sc, v->a);
            // Do something with the coordinate + value info.
            printf("%d %d : %d\n", v->i, v->j, v->a);
        }
    }
    SC_END(sc);
}

int test1(void) {
    struct vars vars = {};
    struct shaco sc = {};
    do {
        parse(&sc, &vars);
        *(int*)sc.buf = 3;
    } while(sc.next);
    return 0;
}


int test2(void) {
    struct vars vars = {};
    struct shaco sc = {};
    int stream[] = {3,
                    3,  101, 102, 103,
                    2,  101, 102,
                    1,  101};
    int i = 0;
    do {
        parse(&sc, &vars);
        *(int*)sc.buf = stream[i++];
    } while (sc.next);

    return 0;
}

int main(void) {
    test1();
    test2();
    return 0;
}
