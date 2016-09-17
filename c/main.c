
#include <stdio.h>

#include "slist.h"

int main()
{
  extern int yyparse();
  int ok = yyparse();
  return ok;
}

