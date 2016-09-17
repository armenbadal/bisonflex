
#ifndef SLIST_H
#define SLIST_H

struct node {
  void* data;
  struct node* next;
};

extern struct node* create_node( void* vl );
extern struct node* append_to( struct node* nd, void* vl );

#endif

