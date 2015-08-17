#include <stdio.h>
#include <string.h>
#include "encoder.h"
#include "decoder.h"

int main (int argc, char **argv)
{
  if (argc != 4) {
    printf ("Uso: %s [decode|encode] <entrada> <salida>\n", argv[0]);
    return 1;
  } else {
    void (*cmd) (FILE *, int, FILE *);
    FILE *fin, *fout;
    int size;

    if      (strcmp (argv[1], "encode") == 0)
      cmd = encode;
    else if (strcmp (argv[1], "decode") == 0)
      cmd = decode;
    else {
      printf ("Uso: %s [decode|encode] <entrada> <salida>\n", argv[0]);
      return 1;
    }

    if ((fin = fopen (argv[2], "rb")) == NULL) {
      printf ("Error al tratar de abrir %s\n", argv[2]);
      return 1;
    }

    fseek (fin, 0, SEEK_END);
    size = ftell (fin);
    fseek (fin, 0, SEEK_SET);

    if ((fout = fopen (argv[3], "wb")) == NULL) {
      printf ("Error al intentar abrir %s\n", argv[3]);
      fclose (fin);
      return 1;
    }

    cmd (fin, size, fout);
    fclose (fin);
    fclose (fout);
    
    return 0;
  }
}
