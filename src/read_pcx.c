#include "read_pcx.h"

void		read_pcx(char *uncomp, uint32_t size, char *name)
{
  struct h3pcx_indexed	*h3_img = NULL;
  BMP		*b = NULL;
  char		output_name[500];

  size = size;
  h3_img = (struct h3pcx_indexed*)uncomp;
  printf("Bitmap Size = %d\n", h3_img->bitmap_size);
  printf("Width = %d\n", h3_img->width);
  printf("Height = %d\n", h3_img->height);
  h3_img->bitmap = (uint8_t*)(uncomp + 3 * sizeof(uint32_t));
  b = (BMP*)(uncomp + sizeof(uint32_t));
  printf("b->Width = %d\n", b->width);
  printf("b->heigh = %d\n", b->height);
  b->data = (uint8_t*)(uncomp + 3 * sizeof(uint32_t));
  strcpy(output_name, "./extract/");
  strcat(output_name, name);
  strcat(output_name, ".bmp");
  bmp_save(b, output_name);
}
