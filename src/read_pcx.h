#ifndef READ_PCX_H_
# define READ_PCX_H_

# include "read_lod.h"

struct h3pcx_color_indexed
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct h3pcx_indexed
{
  uint32_t bitmap_size;
  uint32_t width;
  uint32_t height;
  uint8_t  *bitmap;
};

void		read_pcx(char *uncomp, uint32_t size, char *name);

#endif /* !READ_PCX_H_ */
