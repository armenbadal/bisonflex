
#include <stdio.h>

int main()
{
  extern int yyparse();
  int ok = yyparse();
  return ok;
}

