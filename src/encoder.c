#include <stdio.h>
#include <stdlib.h>
#include "encoder.h"

Encoder *enc_new (void)
{
  Encoder *enc = malloc (sizeof (Encoder));
  List *word = list_new (0);
  uchar j = 0;
  int i;

  enc->dict  = trie_new (0, 0);
  enc->nbits = MIN_BITS;
  enc->pcode = 0;
  enc->bitbuffer.buffer = 0;
  enc->bitbuffer.length = 0;

  /* Se agregan los 256 caracteres iniciales. */
  for (i = 0; i < INITIAL_SIZE; i++, j++) {
    word->data = j;
    enc_add_word (enc, word);
  }

  list_destroy (word);
  return enc;
}

void enc_destroy (Encoder *enc)
{
  trie_destroy (enc->dict);
  free (enc);
}

Encoder *enc_reset (Encoder *enc)
{
  enc_destroy (enc);
  return enc_new ();
}

void enc_add_word (Encoder *enc, List *s)
{
  if (s == NULL) return;

  trie_add (enc->dict, s, enc->pcode++);
  if (enc->pcode > (1 << enc->nbits)) /* pcode > 2^nbits. */
    ++enc->nbits;
}

void write_bits (Encoder *enc, unsigned int code, FILE *fout)
{
  unsigned int buffer = enc->bitbuffer.buffer;
  unsigned int length = enc->bitbuffer.length;
  unsigned int nbits  = enc->nbits;

  /* Los bits antiguos de corren a la izquierda y
   * se agrega el nuevo código a la derecha. */
  buffer  = (buffer << nbits) | code;
  length += nbits;
  
  /* Si hay más de un byte en el buffer se escribe
   * en el archivo y se actualiza el buffer. */
  while (length >= 8) {
    length -= 8;
    fputc (buffer >> length, fout);
    buffer &= (1 << length) - 1;
  }

  enc->bitbuffer.buffer = buffer;
  enc->bitbuffer.length = length;
}

void encode (FILE *fin, int size, FILE *fout)
{
  Encoder *enc = enc_new ();
  List *word = NULL;
  Trie *node;
  uchar chr;

  for (; size > 0; size--) {
    chr  = fgetc (fin);
    node = trie_find (enc->dict, word);
    word = list_prepend (word, chr); /* word = word + chr. */

    if (! trie_find (enc->dict, word)) {
      write_bits (enc, node->code, fout);
      enc_add_word (enc, word);
      list_destroy (word);
      word = list_new (chr);
    }

    if (enc->nbits > MAX_BITS)
      enc = enc_reset (enc);
  }

  node = trie_find (enc->dict, word);
  write_bits (enc, node->code, fout);

  if (enc->bitbuffer.length)
    fputc (enc->bitbuffer.buffer << (8 - enc->bitbuffer.length), fout);

  list_destroy (word);
  enc_destroy  (enc);
}
