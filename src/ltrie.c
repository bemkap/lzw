#include <stdlib.h>
#include "ltrie.h"

Trie *trie_new (uchar data, unsigned int code)
{
  Trie *trie = malloc (sizeof (Trie));
  int i;
  
  trie->data = data;
  trie->code = code;
  trie->bro = trie->son = NULL;

  for (i = 0; i < 16; i++)
    trie->bmap[i] = 0;

  return trie;
}

void trie_destroy (Trie *trie)
{
  if (trie != NULL) {
    trie_destroy (trie->son);
    trie_destroy (trie->bro);
    free (trie);
  }
}

Trie *trie_find (Trie *trie, List *s)
{
  if (s == NULL) {
    return trie;
  } else {
    Trie *node = trie_find (trie, s->next);

    node = node->son;
    while (node != NULL && node->data != s->data)
      node = node->bro;

    return node;
  }
}

void trie_add (Trie *trie, List *s, unsigned int code)
{
  if (s == NULL) return;

  Trie *node = trie_find (trie, s->next);
  Trie *new  = trie_new (s->data, code);
  
  new->bro  = node->son;
  new->son  = NULL;
  node->son = new;
}
