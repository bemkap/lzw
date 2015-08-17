#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "ltrie.h"
#include "common.h"

typedef struct {
  Trie *dict;         /* Diccionario. */
  unsigned int nbits; /* Bits usados por los códigos. */
  unsigned int pcode; /* Primer código disponible. */
  bitbuffer bitbuffer;
} Encoder;

Encoder *enc_new      (void);
void     enc_destroy  (Encoder *);
Encoder *enc_reset    (Encoder *);
void     enc_add_word (Encoder *, List *);

/* encode: parámetros: entrada, tamaño
 * de la entrada, salida. */
void     encode       (FILE *, int, FILE *);

#endif
