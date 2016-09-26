
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stream.h"

stream* create_stream()
{
  stream* sro = malloc(sizeof(stream));
  sro->buffer = malloc(SIZE);
  sro->length = 0;
  return sro;
}

void destroy_stream( stream* sm )
{
  if( sm != NULL ) {
	if( sm->buffer != NULL )
	  free(sm->buffer);
	free(sm);
  }
}

void write_to_stream( stream* sm, const char* ex )
{
  char* place = sm->buffer;
  place += sm->length;
  sprintf(place, "%s", ex);
  size_t count = strlen(ex);
  sm->length += count;
  place[count] = 0;
}

void dump_stream( stream* sm )
{
  puts(sm->buffer);
  printf("%d\n", (int)sm->length);
}



/**/
int main()
{
  stream* sr0 = create_stream();
  write_to_stream(sr0, "text0");
  write_to_stream(sr0, "text1");
  write_to_stream(sr0, "text2");
  dump_stream(sr0);
  destroy_stream(sr0);
	
  printf("%d", 1 << 16);
}

