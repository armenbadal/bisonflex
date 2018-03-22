
/*
*/

#include <stdio.h>

#include <gc.h>

#include "ast.h"

program* prog = NULL;

/**/
int main( int argc, char** argv )
{
  /* */
  if( argc == 1 ) {
    puts("Օգտագործումը։");
    printf("$ %s <ֆայլ>.bas [-o <ֆայլ>.json]\n", argv[0]);
    return 0;
  }

  /* */
  char* infile = NULL;
  char* outfile = "result.json";

  /* */
  if( argc >= 2 ) {
    infile = argv[1];

    if( argc == 3 )
      outfile = argv[2];
  }

  /* */
  extern FILE* yyin;
  yyin = fopen(infile, "r");
  if( yyin == NULL ) {
    fprintf(stderr, "Cannot open input file '%s'.\n", infile);
    return 1;
  }
  FILE* resout = fopen(outfile, "w");
  if( resout == NULL ) {
    fprintf(stderr, "Cannot open output file '%s'.\n", outfile);
    return 2;
  }
  
  prog = GC_MALLOC(sizeof(program));

  extern int yyparse();
  int ok = yyparse();
  if( 0 == ok )
    program_as_json(prog, resout);

  fclose(yyin);
  fclose(resout);
  
  return ok;
}

