shaco
=====

A collection of macros for implementing shallow co-routines (shaco) in C.

A shaco is a state machine implemented as a C function containing a state dependent entry point.

Caveats:

* No local variables: all state needs to be contained in the state object or static variables.
