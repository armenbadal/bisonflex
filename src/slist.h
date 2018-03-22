
#ifndef SLIST_H
#define SLIST_H

typedef struct _node node;
struct _node {
  void* data;
  node* next;
};

extern node* create_node( void* vl, node* );
extern node* append_to( node* nd, void* vl );

#endif

