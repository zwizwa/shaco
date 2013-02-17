#ifndef _GENSYM_H_
#define _GENSYM_H_

/* Generate pseudo-unique symbol names.  The __COUNTER__ preprocessor
   symbol is incremented each time it is expanded.

   The indirection is necessary to make sure that __COUNTER__ is
   expanded and not treated as a literal. */
#define __GENSYM2(x,y) __g_##x##y
#define __GENSYM1(x,y) __GENSYM2(x,y)
#define GENSYM(x) __GENSYM1(x,__COUNTER__)

#endif
