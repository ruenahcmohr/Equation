#include "funclib.h"


Status_t initFuncs   (funclist_t * this){
  this->func      = NULL;
  this->funcCount = 0;
}

Status_t addFunc     (funclist_t * this, char * name, double (*fn)(fnParams_t *p) ) {
  
}

Status_t findIndex   (funclist_t * this, char * name, unsigned int * index) {

}

Status_t execFunc    (funclist_t * this, char * name, double * value, fnParams_t * p) {

}

Status_t delFunc     (funclist_t * this, char * name) {

}

Status_t dumpFuncs   (funclist_t * this) {

}

Status_t finiFunc    (function_t * this){

}

Status_t showFunc    (function_t * this ) {

}


