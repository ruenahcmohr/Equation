
// just include eq3.h
// then get a equation in string form, no pre-parsing req'd
// then evaluate it., thats all, the evaluator will read/write/create variables as required.
// library has no external dependencies

#include "eq5.h"
#include "version.h"

int main(int argc, char **argv) {
  
  varlist_t myvars; // variables
  char * equation;  // an equation.
  double answer, resultBased;
  unsigned int resultBasei, answeri;
  unsigned long i;
  
  if (argc < 2)  {
     printf ("Need formula, genious.\n");
     return 1;
  }
  
  initVars(&myvars);  // initialize variables
  setVar     (&myvars, "Pi", 3.1415926535, 1) ; // set a value to a variable
  setVar     (&myvars, "ResultBase", 10, 1) ;   // default base 10
  setVar     (&myvars, "Version", 5.0, 0) ; // software version
 // dumpVars   (&myvars);  // print all the variables.
  
  equation = strdup(argv[1]);

  answer = eq_Eval(equation, &myvars);
  answeri = answer;
  
  evalVar(&myvars, "ResultBase", &resultBased);
  resultBasei = resultBased;
  
  switch(resultBasei) {
    case 2:      
    	printf("0b");
        for (i = 0x80000000; i > 0; i >>= 1) if ((answeri & i) != 0) break;
        for (; i > 0; i >>= 1) {
		printf("%c", ((answeri & i) ? '1' : '0'));
	}
        printf("\n");
    break;
    
    case 8:
      printf("%s  -->>  %o\n",equation,  answeri); // calculate and show the result
    break;
    
    case 16: 
      printf("%s  -->>  0x%0X\n",equation,  answeri); // calculate and show the result
    break;
        
    default:
      printf("%s  -->>  %g\n",equation,  answer); // calculate and show the result
    break;
  }
  
//  dumpVars   (&myvars); // show the variables again, as the equation could have modified them.
  
  free(equation); // let it go run free
  return 0;
  
}

