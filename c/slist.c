
#include <stdlib.h>

#include "slist.h"

/**/
struct node* create_node( void* vl )
{
  struct node* res = malloc(sizeof(struct node));
  res->data = vl; res->next = NULL;
  return res;
}

/**/
struct node* append_to( struct node* nd, void* vl )
{
  if( NULL == nd )
	return create_node(vl);

  nd->next = append_to(nd->next, vl);
  return nd;
}


