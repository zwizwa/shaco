/* Shallow coroutines.

   This is a very simple "multitasker" that solves a very specific
   problem:
   - parsing a nested data structure from a binary stream, when
   - the nesting level is statically known, and
   - the incoming buffer size does not align with the data structures, and
   - we can't use the "standard unix" approach of pipes + threads

   I.e. for 2 levels would be a list of a list of atoms.

   In other words, a statically known structure corresponds to a
   "nested for loop" looking like this in pseudo-C

   header1 = read();
   for (i = 0; i<header1->num; i++) {
      header2 = read();
      for (j = 0; j<header2->num; j++) {
         datum = read();
         do_something_useful(data);\
      }
   }

   Such code would work well with pipes + threads, but doesn't need a
   full stack abstraction, since its context is *shallow*,
   i.e. limited to the location in the loop + the values of the
   variables in scope.



   In practice, this pattern occurs often on small microcontrollers,
   running without an OS or in a situation where pipes + threads are
   not very efficient.

   The pattern is simple enough to deserve a special-purpose
   abstraction.  We'll do this by abstracting "read".

*/

#include "gensym.h"
#include <stdlib.h>
#include <stdio.h>

struct shaco {
    void **next;
    void *buf;
    int size;
};

#define SC_BEGIN(shaco)                              \
    do {if (shaco->next) goto *(shaco->next);} while(0)

#define SC_WAIT(label, shaco, var) do {           \
        shaco->buf = &(var);                      \
        shaco->size = sizeof(var);                \
        shaco->next = &&label;                    \
        return;                                   \
      label:                                      \
        while(0); } while(0)

#define SC_END(shaco) \
    do {shaco->next = NULL; } while(0)

// Bind to the shallow coroutine context `sc'.
#define BEGIN()   SC_BEGIN
#define READ(var) SC_WAIT(GENSYM(label), sc, var)

// "local variables" for function parse()
struct vars {
    int i, nb_i;
    int j, nb_j;
    int a;
};

void parse(struct shaco *sc, struct vars *v) {
    SC_BEGIN(sc);
    READ(v->nb_i);
    for (v->i = 0; v->i < v->nb_i; v->i++) {
        READ(v->nb_j);
        for (v->j = 0; v->j < v->nb_j; v->j++) {
            READ(v->a);
            // Do something with the coordinate + value info.
            printf("%d %d : %d\n", v->i, v->j, v->a);
        }
    }
    SC_END(sc);
}

int main(void) {
    struct vars vars = {};
    struct shaco shaco = {};
    do {
        parse(&shaco, &vars);
        *(int*)shaco.buf = 3;
    } while(shaco.next);
    return 0;
}
