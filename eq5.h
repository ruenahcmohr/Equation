#ifndef __eq3_h
#define __eq3_h

#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "returnValues.h"
#include "eqll.h"
#include "ops.h"
#include "varlib.h"

#define ISCONST(A)
#define ISVAR(A)
#define ISOPER(A)

#define str_op        "!~+-*/=&^<>%|"
// constant start
#define str_consts     "0123456789."
// constant continue
#define str_constc     "0123456789ABCDEF.xb"
#define str_paren     "()"
#define str_var       "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQSRTUVWXYZ0123456789_"
#define str_white     " \t,"

#define is_op(c)       ((strchr(str_op, c)    !=NULL)&(c!=0)) 
#define is_consts(c)   ((strchr(str_consts, c)!=NULL)&(c!=0))
#define is_constc(c)   ((strchr(str_constc, c)!=NULL)&(c!=0))
#define is_var(c)      ((strchr(str_var, c)   !=NULL)&(c!=0))
#define is_white(c)    ((strchr(str_white, c) !=NULL)&(c!=0)) 
#define is_paren(c)    ((strchr(str_paren, c) !=NULL)&(c!=0)) 

void        getClass(      states_t *state, char c);
Status_t    opResolve(     OpType_t * out, char * in );
double        resolve_Num(   eqNode_t * this, varlist_t * vars) ;
Status_t    FindNextOp(    eqLL_t * this, eqNode_t ** result) ;
Status_t    eq_Prioritize( eqLL_t * this );
void        eq_Parse(      eqLL_t * this, char * e) ;
double        eq_Eval(       const char * equation, varlist_t * vars); 
void        getClassChunk( char **end, char ** result, states_t *state) ;
Status_t    doOp(          eqNode_t * this, varlist_t * vars) ;
Status_t    Boilrhs(eqNode_t * this, varlist_t * vars, double lhs, double result);
Status_t    parenDelta(int * delta, char * parenBlock) ;

#endif
