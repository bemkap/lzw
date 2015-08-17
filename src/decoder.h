#ifndef __DECODER_H__
#define __DECODER_H__

#include "list.h"
#include "common.h"

typedef struct {
  List **buffer; /* Arreglo dinámico de cadenas. */
  int nelem;     /* Cantidad de elementos en el buffer. */
  int nmax;      /* Tamaño del buffer. */
} charbuffer;

typedef struct {
  unsigned int nbits;
  charbuffer charbuffer;
  bitbuffer bitbuffer;
} Decoder;

Decoder *dec_new      (void);
void     dec_destroy  (Decoder *);
Decoder *dec_reset    (Decoder *);
void     dec_add_word (Decoder *, List *);

/* cb_resize: duplica el tamaño del charbuffer. */
void     cb_resize    (charbuffer *);

/* decode: parámetros: entrada, tamaño
 * de la entrada, salida. */
void     decode       (FILE *, int, FILE *);

#endif
