
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "slist.h"

void test_expression_as_lisp()
{
  fprintf(stdout, "~ ~ ~ ~ expression_as_lisp ~ ~ ~ ~\n");

  // case 0
  struct expression* ex0 = malloc(sizeof(struct expression));
  ex0->kind = NUMBER;
  ex0->number = 3.14;

  expression_as_lisp(ex0, stdout);
  puts("");

  // case 1
  struct expression* ex1 = malloc(sizeof(struct expression));
  ex1->kind = VARIABLE;
  ex1->ident = malloc(32);
  strcpy(ex1->ident, "variable");

  expression_as_lisp(ex1, stdout);
  puts("");

  // case 2
  struct expression* ex2 = malloc(sizeof(struct expression));
  ex2->kind = UNARY;
  ex2->oper = NEG;
  ex2->exo = ex1;

  expression_as_lisp(ex2, stdout);
  puts("");
  
  // case 3
  struct expression* ex3 = malloc(sizeof(struct expression));
  ex3->kind = BINARY;
  ex3->oper = POW;
  ex3->exo = ex0;
  ex3->exi = ex2;
  
  expression_as_lisp(ex3, stdout);
  puts("");

  /*
  // case 4
  struct expression* ex4 = malloc(sizeof(struct expression));
  
  expression_as_lisp(ex4, stdout);
  puts("");
  */
}

/**/
void test_statement_as_lisp()
{
  fprintf(stdout, "~ ~ ~ ~ statement_as_lisp ~ ~ ~ ~\n");

  // case 0
  struct statement* st0 = malloc(sizeof(struct statement));
  st0->kind = INPUT;
  struct input_s* ch0 = malloc(sizeof(struct input_s));
  ch0->vari = malloc(32);
  strcpy(ch0->vari, "variable");
  st0->child = ch0;

  statement_as_lisp(st0, stdout);
  puts("");

  // case 1
  struct statement* st1 = malloc(sizeof(struct statement));
  st1->kind = PRINT;
  struct expression* ex0 = malloc(sizeof(struct expression));
  ex0->kind = NUMBER;
  ex0->number = 3.14;
  struct print_s* ch1 = malloc(sizeof(struct print_s));
  ch1->valu = ex0;
  st1->child = ch1;

  statement_as_lisp(st1, stdout);
  puts("");

  // case 2
  struct statement* st2 = malloc(sizeof(struct statement));
  st2->kind = ASSIGN;
  struct assignment_s* ase = malloc(sizeof(struct assignment_s));
  ase->vari = malloc(32);
  strcpy(ase->vari, "xyz");
  ase->valu = malloc(sizeof(struct expression));
  ase->valu->kind = NUMBER;
  ase->valu->number = 3.14;
  st2->child = ase;

  statement_as_lisp(st2, stdout);
  puts("");
}

/**/
void test_function_as_lisp()
{
  fprintf(stdout, "~ ~ ~ ~ function_as_lisp ~ ~ ~ ~\n");

}

/**/
void test_program_as_lisp()
{
  fprintf(stdout, "~ ~ ~ ~ program_as_lisp ~ ~ ~ ~\n");

}

/**/
int main()
{
  test_expression_as_lisp();
  test_statement_as_lisp();
  test_function_as_lisp();
  test_program_as_lisp();
}

