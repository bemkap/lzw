#ifndef __LIST_H__
#define __LIST_H__

typedef unsigned char uchar;

typedef struct _List {
  uchar data;
  struct _List *next;
} List;

List *list_new     (uchar);
void  list_destroy (List *);
List *list_append  (List *, uchar);
List *list_prepend (List *, uchar);
uchar list_last    (List *);

#endif
