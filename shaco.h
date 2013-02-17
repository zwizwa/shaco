#ifndef _SHACO_
#define _SHACO_

/* Shallow coroutines.

   This is a very simple "multitasker" that solves a very specific problem:

   - parsing a nested data structure from a binary stream, when
   - the nesting level is statically known, and
   - the incoming buffer size does not align with the data structures, and
   - we can't use the "standard unix" approach of pipes + threads

   See test.c for an example.

   Please note that this technique should not be used in functions
   that have local variables.
*/

#include "gensym.h"

struct shaco {
    void **next;
    void *buf;
    int size;
};

#define SC_BEGIN(sc)                              \
    do {if (sc->next) goto *(sc->next);} while(0)

#define SC_WAIT_L(label, sc, var) do {         \
        sc->buf = &(var);                      \
        sc->size = sizeof(var);                \
        sc->next = &&label;                    \
        return;                                   \
      label:                                      \
        while(0); } while(0)
#define SC_WAIT(sc, var) SC_WAIT_L(GENSYM(),sc,var)

#define SC_END(sc) \
    do {sc->next = NULL; } while(0)


#endif
