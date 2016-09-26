
#ifndef STREAM_H
#define STREAM_H

#define SIZE 4096

typedef struct _stream {
  char* buffer;
  size_t length;  
} stream;
extern stream* create_stream();
extern void destroy_stream( stream* );
extern void write_to_stream( stream*, const char* );
extern void dump_stream( stream* );

#endif

