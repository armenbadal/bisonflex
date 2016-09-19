
#include <stdio.h>

#include "slist.h"

int main()
{
  /* Test 0 */
  char* a0 = "a0";
  char* a1 = "a1";
  char* a2 = "a2";

  node* sli = NULL;
  sli = append_to(sli, a0);
  sli = append_to(sli, a1);
  sli = append_to(sli, a2);

  node* p = sli;
  while( p != NULL ) {
	printf("%s\n", (char*)(p->data));
	p = p->next;
  }

  /* Test 1 */
  double d0 = 3.14;
  double d1 = d0 * 2;
  double d2 = d1 * 3;
  
  node* slii = NULL;
  slii = append_to(slii, &d0);
  slii = append_to(slii, &d1);
  slii = append_to(slii, &d2);
  
  p = slii;
  while( p != NULL ) {
	printf("%lf\n", *((double*)(p->data)));
	p = p->next;
  }
  
  return 0;
}

