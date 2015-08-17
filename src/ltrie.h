#ifndef __TRIE_H__
#define __TRIE_H__

#include "list.h"

typedef struct _Trie {
  uchar         data;
  unsigned int  code;
  struct _Trie *bro;
  struct _Trie *son;
} Trie;

Trie *trie_new     (uchar, unsigned int);
void  trie_destroy (Trie *);

/* trie_find: busca un nodo en el trie.
 * return: un puntero al nodo o NULL si no está. */
Trie *trie_find     (Trie *, List *);

/* trie_add: agrega una cadena al trie. */
void  trie_add      (Trie *, List *, unsigned int);

#endif
