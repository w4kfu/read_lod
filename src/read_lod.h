#ifndef READ_LOD_H_
# define READ_LOD_H_

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <ctype.h>
# include <zlib.h>
# include <string.h>
# include "extension.h"

typedef struct
{
  char		name[16];
  uint32_t	offset;
  uint32_t	orgsize;
  uint32_t	type;
  uint32_t	comsize;
}		t_h3_file;

typedef struct
{
  uint32_t	magic;
  uint32_t	type;
  uint32_t	nb_file;
  uint8_t	unknown[80];
  t_h3_file	h3file[10000];
}		t_h3lod;

#endif /* !READ_LOD_H_ */
