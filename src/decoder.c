#include <stdio.h>
#include <stdlib.h>
#include "decoder.h"

Decoder *dec_new (void)
{
  Decoder *dec = malloc (sizeof (Decoder));
  List *word = list_new (0);
  uchar j = 0;
  int i;

  dec->charbuffer.buffer = malloc (INITIAL_SIZE * sizeof (List *));
  dec->nbits             = MIN_BITS;
  dec->charbuffer.nelem  = 0;
  dec->charbuffer.nmax   = INITIAL_SIZE;
  dec->bitbuffer.buffer  = 0;
  dec->bitbuffer.length  = 0;

  /* Se agregan los 256 caracteres iniciales. */
  for (i = 0; i < INITIAL_SIZE; i++, j++) {
    word->data = j;
    dec_add_word (dec, word);
  }

  list_destroy (word);
  return dec;
}

void dec_destroy (Decoder *dec)
{
  int i;
  
  for (i = 0; i < dec->charbuffer.nelem; i++)
    list_destroy (dec->charbuffer.buffer[i]);

  free (dec->charbuffer.buffer);
  free (dec);
}

Decoder *dec_reset (Decoder *dec)
{
  dec_destroy (dec);
  return dec_new ();
}

void dec_add_word (Decoder *dec, List *s)
{
  if (s == NULL) return;

  /* Agrega una copia de la lista al buffer. La cadena en el buffer
   * se guarda en su orden, pero la del parámetro está al revés. */
  dec->charbuffer.buffer[dec->charbuffer.nelem] = list_new (s->data);
  for (s = s->next; s != NULL; s = s->next) {
    dec->charbuffer.buffer[dec->charbuffer.nelem] = 
      list_prepend (dec->charbuffer.buffer[dec->charbuffer.nelem], s->data);
  }
  
  if (++dec->charbuffer.nelem >= (1 << dec->nbits)) /* nelem+1 >= 2^nbits. */
    dec->nbits++;
  
  if (dec->charbuffer.nelem >= dec->charbuffer.nmax)
    cb_resize (&dec->charbuffer);
}

void cb_resize (charbuffer *cb)
{
  cb->nmax  *= 2;
  cb->buffer = realloc (cb->buffer, cb->nmax * sizeof (List *));
}

void fputl (List *list, FILE *fout)
{
  for (; list != NULL; list = list->next)
    fputc (list->data, fout);
}

void decode (FILE *fin, int size, FILE *fout)
{
  Decoder *dec = dec_new ();
  List *word, *entry;
  unsigned int code;
  uchar chr;

  chr = fgetc (fin);
  fputc (chr, fout);

  word = list_new (chr);
  for (; size > 1; size--) {
    chr = fgetc (fin);
    if (dec->nbits > MAX_BITS) {
      dec = dec_reset (dec);
      list_destroy (word);
      word = list_new (chr);
      fputc (chr, fout);
    } else {
      /* Se agrega el nuevo caracter al buffer. */
      dec->bitbuffer.buffer  = (dec->bitbuffer.buffer << 8) | chr;
      dec->bitbuffer.length += 8;
      
      while (dec->bitbuffer.length >= dec->nbits) {
	dec->bitbuffer.length -= dec->nbits;
	code = dec->bitbuffer.buffer >> dec->bitbuffer.length;
	dec->bitbuffer.buffer &= (1 << dec->bitbuffer.length) - 1;

	if (code < dec->charbuffer.nelem) { /* El código está en el diccionario. */
	  entry = dec->charbuffer.buffer[code];
	  fputl (entry, fout);

	  /* Agrega word + entry[0] al diccionario. */
	  word = list_prepend (word, entry->data);
	  dec_add_word (dec, word);

	  list_destroy (word);
	  word = NULL;

	  for (; entry != NULL; entry = entry->next)
	    word = list_prepend (word, entry->data);
	} else {
	  /* Agrega word + word[0] al diccionario. */
	  word = list_prepend (word, list_last (word));
	  dec_add_word (dec, word);

	  fputl (dec->charbuffer.buffer[dec->charbuffer.nelem-1], fout);
	}
      }
    }
  }

  list_destroy (word);
  dec_destroy  (dec);
}
