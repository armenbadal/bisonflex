
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "slist.h"

void test_expression_as_lisp()
{
  fprintf(stdout, "~ ~ ~ ~ expression_as_lisp ~ ~ ~ ~\n");

  // case 0
  expression* ex0 = create_number(3.14);
  expression_as_lisp(ex0, stdout);
  puts("");

  // case 1
  expression* ex1 = create_variable("xyz");
  expression_as_lisp(ex1, stdout);
  puts("");

  // case 2
  expression* ex2 = create_unary(NEG, ex1);
  expression_as_lisp(ex2, stdout);
  puts("");
  
  // case 3
  expression* ex3 = create_binary(POW, ex0, ex2);
  expression_as_lisp(ex3, stdout);
  puts("");

  /*
  // case 4
  expression* ex4 = malloc(sizeof(expression));
  
  expression_as_lisp(ex4, stdout);
  puts("");
  */
}

/**/
void test_statement_as_lisp()
{
  fprintf(stdout, "~ ~ ~ ~ statement_as_lisp ~ ~ ~ ~\n");

  // case 0
  statement* st0 = create_input("abc12");
  statement_as_lisp(st0, stdout);
  puts("");

  // case 1
  statement* st1 = create_print(create_number(3.14));
  statement_as_lisp(st1, stdout);
  puts("");

  // case 2
  statement* st2 = create_assign("xyz", create_binary(ADD, create_number(3.14), create_number(7)));
  statement_as_lisp(st2, stdout);
  puts("");

  // case 3
  statement* st3 = create_if( create_binary(EQ, create_variable("n"), create_variable("m")),
							  create_assign("n", create_number(777)),
							  create_assign("m", create_number(444)));
  statement_as_lisp(st3, stdout);
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

