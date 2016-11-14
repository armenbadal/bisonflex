
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "slist.h"

void test_expression_as_json()
{
  fprintf(stdout, "~ ~ ~ ~ expression_as_json ~ ~ ~ ~\n");

  // case 0
  expression* ex0 = create_number(3.14);
  expression_as_json(ex0, stdout);
  puts("");

  // case 1
  expression* ex1 = create_variable("xyz");
  expression_as_json(ex1, stdout);
  puts("");

  // case 2
  expression* ex2 = create_unary(NEG, ex1);
  expression_as_json(ex2, stdout);
  puts("");
  
  // case 3
  expression* ex3 = create_binary(POW, ex0, ex2);
  expression_as_json(ex3, stdout);
  puts("");

  /*
  // case 4
  expression* ex4 = malloc(sizeof(expression));
  
  expression_as_json(ex4, stdout);
  puts("");
  */
}

/**/
void test_statement_as_json()
{
  fprintf(stdout, "~ ~ ~ ~ statement_as_json ~ ~ ~ ~\n");

  // case 0
  statement* st0 = create_input("abc12");
  statement_as_json(st0, stdout);
  puts("");

  // case 1
  statement* st1 = create_print(create_number(3.14));
  statement_as_json(st1, stdout);
  puts("");

  // case 2
  statement* st2 = create_assign("xyz", create_binary(ADD, create_number(3.14),
                                                      create_number(7)));
  statement_as_json(st2, stdout);
  puts("");

  // case 3
  statement* st3 = create_if( create_binary(EQ, create_variable("n"),
                                            create_variable("m")),
                              create_assign("n", create_number(777)),
                              create_assign("m", create_number(444)),
							  NULL);
  statement_as_json(st3, stdout);
  puts("");

  // case 4
  statement* st4 = create_for( "i", create_number(0), create_number(9),
                               create_number(2), create_print(create_variable("i")));
  statement_as_json(st4, stdout);
  puts("");

  // case 5
  // while

  // case 6
  // call

  // case 7
  node* cm0 = create_node(create_assign("x", create_number(1)), NULL);
  cm0 = append_to(cm0, create_assign("y", create_number(2)));
  cm0 = append_to(cm0, create_assign("z", create_number(3)));
  statement* st7 = create_sequence(cm0);
  statement_as_json(st7, stdout);
  puts("");
}

/**/
void test_function_as_json()
{
  fprintf(stdout, "~ ~ ~ ~ function_as_json ~ ~ ~ ~\n");

  // case 0
  node* ps0 = create_node("x", NULL);
  ps0 = append_to(ps0, "y");
  statement* be0 = create_assign("f", create_binary(ADD, create_variable("x"),
                                                    create_variable("y")));
  function* su0 = create_function("f", ps0, be0);

  function_as_json(su0, stdout);
  puts("");
}

/**/
void test_program_as_json()
{
  fprintf(stdout, "~ ~ ~ ~ program_as_json ~ ~ ~ ~\n");

}

/**/
int main()
{
  test_expression_as_json();
  test_statement_as_json();
  test_function_as_json();
  test_program_as_json();
}

