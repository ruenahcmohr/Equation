#ifndef __funclib
#define __funclib

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "funcs.h"
#include "returnValues.h"


typedef struct fnParams_s {
  unsigned int pCount;
  double *     params;
} fnParams_t;

typedef struct function_s {
  char *   name;
  double (*fn)(fnParams_t *p);
} function_t;

typedef struct funclist_s {
  function_t * func;
  unsigned int funcCount;
} funclist_t;




Status_t initFuncs   (funclist_t * this);
Status_t addFunc     (funclist_t * this, char * name, double (*fn)(fnParams_t *p) ) ;
Status_t findIndex   (funclist_t * this, char * name, unsigned int * index) ;
Status_t execFunc    (funclist_t * this, char * name, double * value, fnParams_t * p) ;
Status_t delFunc     (funclist_t * this, char * name) ;

Status_t dumpFuncs   (funclist_t * this) ;

Status_t finiFunc    (function_t * this);
Status_t showFunc    (function_t * this ) ;


#endif
