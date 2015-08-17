#ifndef __COMMON_H__
#define __COMMON_H__

enum { MIN_BITS = 8, MAX_BITS = 16, INITIAL_SIZE = 256 };

typedef struct {
  unsigned int buffer;
  unsigned int length;
} bitbuffer;

#endif
