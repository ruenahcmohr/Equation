#include "eq5.h"

double eq_Eval( const char * equation, varlist_t * vars){

  char      * test;
  eqLL_t    eq;
  eqNode_t  * tempN;
  double      retval = 0;
  
  eqLL_Init(&eq);  
  test  = strdup(equation);
  
  // initinal parse, create linked list of components 
 // printf("Parsing...\n");
  eq_Parse(&eq, test) ;
  
 // eqLL_Show( &eq) ; printf ("\n ------ \n");
  
  // prioritize operations   
//  printf("Prioritizing...\n");
  eq_Prioritize( &eq );  
  
 eqLL_Show( &eq);
  
  //  evaluate 
  while( FindNextOp(&eq, &tempN) == OK) {
  //  eqLL_Show( &eq) ;
  //  printf("High operation is %s\n", tempN->detail);    
    if (doOp(tempN, vars) != OK){ // this could be a switch statement
      printf("Unable to perform calculation, ");     
      return 42;  // oh you had to have fun debugging that!      
    } else {  // doOp will remove the node and its lhs, if thats the tail, we need to change it.
      for (tempN = eq.Head; tempN; tempN = tempN->rhs ) eq.Tail = tempN;
    }
    //eqLL_Show( &eq);
  //  printf ("\n ------ \n");
  }
  retval = resolve_Num(eq.Head, vars);
  //printf("%s result is: %f \n",equation,  retval);
  
  //clean up   
  eqLL_Fini( & eq);
  free(test);

  return retval;
}



// populate DLL
void  eq_Parse( eqLL_t * this, char * e) {
  char      * chunk;
  states_t    state = ERR;
   eqNode_t * iter; 
   
  while( *e ) {
    // seperate components
    getClassChunk( &e, &chunk, &state);
    if (state != WHITE)  eqLL_Append(this, state, chunk, NULL);                           
    free(chunk);           
  }
  
  // next we need to correct variable:paren pairs to functions

 for(iter = this->Head; iter->rhs; iter = iter->rhs) {
    if ((iter->type == VAR) && (iter->rhs->type == PAREN)) {
      if (iter->rhs->detail[0] == '(')   iter->type = FUNC;
    }
 }

}


void getClassChunk( char **end, char ** result, states_t *state) {

    char * beginning;
    
    beginning = *end;
    getClass( state, **end);
     
    switch(*state) {
      case CONST:       while(is_constc(**end)) (*end)++;      break;  
      case VAR:         while(is_var(**end))    (*end)++;      break;  
      case OPER:        while(is_op(**end))     (*end)++;      break;      
      case WHITE:       while(is_white(**end))  (*end)++;      break;      
      case PAREN:       while(is_paren(**end))  (*end)++;      break; // actually we want to break after each one.
      case ERR:                                 (*end)++;      break;            
    } 
    
    *result = strndup(beginning, *end-beginning); 

} 



void getClass( states_t *state, char c) {

  if (0) ;
  else if ( is_op(c))     *state = OPER;
  else if ( is_consts(c)) *state = CONST;
  else if ( is_var(c))    *state = VAR;
  else if ( is_white(c))  *state = WHITE;
  else if ( is_paren(c))  *state = PAREN;
  else                    *state = ERR;    
  
}




//---------------------


Status_t  eq_Prioritize( eqLL_t * this ){
 eqNode_t * iter;
 OpType_t   oper;
 int level;
 
 level = 0;

 for(iter = this->Head; iter; iter = iter->rhs) {
    if (iter->type == PAREN) {
      parenDelta(&level, iter->detail);
      eqLL_Delete(  this,  iter );
    } else if (iter->type == OPER) {       
       if( opResolve(&oper, iter->detail ) == OK) {
         switch (oper) {
	   case IS:     iter->priority = P_IS   + (P_END * level);  break;	   
	   case NE:     iter->priority = P_NE   + (P_END * level);  break;	   	   
	   case GT:     iter->priority = P_GT   + (P_END * level);  break;	   	   
           case GTRE:   iter->priority = P_GTRE + (P_END * level);  break;
   	   case LE:     iter->priority = P_LE   + (P_END * level);  break;
           case LERE:   iter->priority = P_LERE + (P_END * level);  break;	   	   
	   case AND:    iter->priority = P_AND  + (P_END * level);  break;	   	   
	   case OR:     iter->priority = P_OR   + (P_END * level);  break;		   
	   case ADD:    iter->priority = P_ADD  + (P_END * level);  break;           
           case XOR:    iter->priority = P_XOR  + (P_END * level);  break;         
           case EQ:     iter->priority = P_EQ   + (P_END * level);  break;
           case SUB:    iter->priority = P_SUB  + (P_END * level);  break;                     
           case BOR:    iter->priority = P_BOR  + (P_END * level);  break;                     
           case BAND:   iter->priority = P_BAND + (P_END * level);  break;  
           case MUL:    iter->priority = P_MUL  + (P_END * level);  break;
           case DIV:    iter->priority = P_DIV  + (P_END * level);  break;   
           case MODU:   iter->priority = P_MODU + (P_END * level);  break;    
           case SHIFTR: iter->priority = P_SHR  + (P_END * level);  break;      
           case SHIFTL: iter->priority = P_SHL  + (P_END * level);  break;               
	 }
       } else {
         iter->priority = 1;   
       }       
    }
 }

 return OK;
}


Status_t parenDelta(int * delta, char * parenBlock) {
  char * i;
  
  for(i = parenBlock; *i != '\0'; i++) {
    if (0) {
    } else if (*i == '(') {  (*delta)++;
    } else if (*i == ')') {  (*delta)--;
    } else {
      return BadData;
    }
  }
  
  return OK;
}


// make the mods to perform the operation
Status_t doOp(eqNode_t * this, varlist_t * vars) {

  OpType_t oper;
  double lhs, rhs, result;
  long   ilhs, irhs, iresult;
  
  if ((this->lhs == NULL) || (this->rhs == NULL)) return BadArg;

  lhs = resolve_Num(this->lhs, vars);
  rhs = resolve_Num(this->rhs, vars);
  ilhs = lhs;   irhs = rhs;  // gonna let C do the conversion.
  
  if( opResolve(&oper, this->detail ) == OK) {
  //  printf("lhs: %f, Operator:%s rhs: %f  ", lhs, this->detail, rhs);
    switch (oper) {

       case IS:	    opDassignD (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;       
       case NE:	    opDneqD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;              
       case GT:	    opDgtD     (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;                            
       case GTRE:   opDgteD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;   
       case LE:	    opDlessD   (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;                      
       case LERE:   opDlesseD  (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;     
       case AND:    opDandD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;                            
       case OR:	    opDorD     (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;                    
       case ADD:    opDaddD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;
       case EQ:     opDeqD     (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;       
       case SUB:    opDsubD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;
       case MUL:    opDmulD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;
       case DIV:    opDdivD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;
       case MODU:   opDmoduD   (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;
       case SHIFTR: opDshrD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;
       case SHIFTL: opDshlD    (&result,  &lhs, &rhs) ;         Boilrhs( this,  vars,  lhs, result);         break;
       case BAND:   opIbandI   (&iresult, &ilhs, &irhs); result = iresult; lhs = ilhs;  Boilrhs( this,  vars,  lhs, result);      break;
       case XOR:    opIbxorI   (&iresult, &ilhs, &irhs); result = iresult; lhs = ilhs;  Boilrhs( this,  vars,  lhs, result);      break;
       case BOR:    opIborI    (&iresult, &ilhs, &irhs); result = iresult; lhs = ilhs;  Boilrhs( this,  vars,  lhs, result);      break;
       default:        return BadData;
    }
   // printf("result %f\n", result);
  } 
  return OK;
} 

/*
   This function may destray the tail!
   but we dont know that, were not on the right level!
*/
Status_t Boilrhs(eqNode_t * this, varlist_t * vars, double lhs, double result) {

  char textbuff[40];
  
  if (this->lhs->type == VAR)  setVar(vars, this->lhs->detail, lhs, 1); // !!!???!!! writable ?
  // set it to be a constant
  eqNode_SetType(     this->lhs, CONST);
  // rewrite its value with a literal
  free(this->lhs->detail); snprintf(textbuff, 35, "%g", result); eqNode_SetDetail(   this->lhs, textbuff) ;
  // carry the priority over
  eqNode_SetPriority( this->lhs, this->priority) ;

  // destroy operation and rhs
  eqNode_DeleteNode( this->rhs);  
  eqNode_DeleteNode( this);
  return OK;
}




double resolve_Num(eqNode_t * this, varlist_t * vars) {
  double temp;
  
  temp = 0;
  if (0);
  else if (this->type == CONST)  {    
    if (this->detail[0] == '\'') { // pick off ascii
      temp = this->detail[1];
    }else if (strncmp(this->detail, "0b", 2)==0) { // pick off binary
      temp =  strtol(this->detail+2, NULL, 2);
    } else {
    //  printf("%s -> %d\n", this->detail, strtol(this->detail, NULL, 0));
      temp = strtod(this->detail, NULL);    
    }               
  } else if (this->type == VAR) {
      evalVar(vars, this->detail, &temp);  
  } else if (this->type == FUNC) {
      temp = 42;
      printf("%s\n", this->detail); // function name
      printf("EXTREME WARNING, Functions not supported yet.\n");
  }
  return temp; 
}


Status_t FindNextOp(eqLL_t * this, eqNode_t ** result) {

 eqNode_t * iter;
 long high = 0;
 int hits  = 0;
 
 //for(iter = this->Head; iter; iter = iter->rhs) {
 for(iter = this->Tail; iter; iter = iter->lhs) {
    if (iter->type == OPER) {
      if (iter->priority >= high) {
        high   = iter->priority;
        if (result != NULL)  *result = iter; 
      }
      hits++;
    }
 }
 if (hits) return OK;
 else      return NoMatch;

}

// depreciated
int eq_isDone(eqLL_t * this) {
  if (this->Head == this->Tail)  {
    if((this->Head->type == CONST) || (this->Head->type == VAR)) return 1;
    else return 0;
  } else {
    return 0;
  }  
}



