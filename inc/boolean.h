#ifndef __BOOLEAN_H__
#define __BOOLEAN_H__

#include <stdint.h>

/*C99 offers the boolean type but I've used non-C99 compatible compilers enough that I always
  define TRUE and FALSE as uint8_t's
*/
#define TRUE 0xFF
#define FALSE 0x00

/*This macro takes the internal boolean representation and transforms it into the type of boolean
 I use in this project
*/
#define BOOL(x) ((x == FALSE)?FALSE:TRUE)

/* The rest of these macros define basic boolean logic functions - note that they always return 
 TRUE or FALSE to maintain boolean type consistency
*/
#define NOT(x) ((x==FALSE)?TRUE:FALSE)
#define AND(x,y) ((FALSE == (x&y))?FALSE:TRUE)
#define OR(x,y) ((FALSE == (x|y))?FALSE:TRUE)

/* Basic bit manipulation macros - everyone should use these.  Please, steal these! Don't not use them and
 don't rewrite them yourself!
*/
#define SET(x,y) x |= (1 << y)
#define CLEAR(x,y) x &= ~(1<< y)
#define READ(x,y) ((FALSE == ((x & (1<<y))>> y))?FALSE:TRUE)
#define TOGGLE(x,y) (x ^= (1<<y))

/*A handy typedef to add boolean support*/
typedef uint8_t boolean;

#endif