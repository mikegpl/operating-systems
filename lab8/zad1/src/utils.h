#ifndef ZAD1_UTILS_H
#define ZAD1_UTILS_H

#include <stdlib.h>
#include <stdio.h>

#define ASSERT(FUN, VAL, COMP, ERR) \
  if(VAL == COMP) {\
  fprintf(stderr, "%s: %s\n", FUN, ERR); \
  exit(EXIT_FAILURE); \
  } \

// hehe
#define DESSERT(FUN, VAL, COMP, ERR) \
  if(VAL != COMP) {\
  fprintf(stderr, "%s: %s\n", FUN, ERR); \
  exit(EXIT_FAILURE); \
  } \

#endif //ZAD1_UTILS_H
