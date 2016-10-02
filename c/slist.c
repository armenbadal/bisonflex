
#include <stdlib.h>
#include <gc.h>

#include "slist.h"

/**/
node* create_node( void* vl, node* nx )
{
  node* res = GC_MALLOC(sizeof(node));
  res->data = vl; res->next = nx;
  return res;
}

/**/
node* append_to( node* nd, void* vl )
{
  if( NULL == nd )
    return create_node(vl, NULL);

  nd->next = append_to(nd->next, vl);
  return nd;
}


