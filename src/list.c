#include <stdlib.h>
#include "list.h"

List *list_new (uchar data)
{
  List *list = malloc (sizeof (List));

  list->data = data;
  list->next = NULL;
  return list;
}

void list_destroy (List *list)
{
  List *node;

  while (list != NULL) {
    node = list;
    list = list->next;
    free (node);
  }
}

List *list_append (List *list, uchar data)
{
  List *node;

  if (list == NULL) {
    node = list_new (data);
    return node;
  } else {
    node = list;
    while (node->next != NULL)
      node = node->next;
    node->next = list_new (data);
    return list;
  }
}

List *list_prepend (List *list, uchar data)
{
  List *new = list_new (data);

  new->next = list;
  return new;
}

/* La lista no debe ser NULL. */
uchar list_last (List *list)
{
  while (list->next != NULL)
    list = list->next;

  return list->data;
}
