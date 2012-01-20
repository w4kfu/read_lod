#ifndef EXTENSION_H_
# define EXTENSION_H_

#include <stdint.h>

struct			h3_type
{
  uint32_t		type;
  struct h3_type	*next;
};

struct			h3_ext
{
  char			*name_ext;
  uint32_t		nb;
  struct h3_type	*type;
  struct h3_ext		*next;
};

struct h3_ext	*add_extension(char *name, struct h3_ext *ext, uint32_t type);
void		print_extension(struct h3_ext *ext);
void		clean_extension(struct h3_ext *ext);

#endif /* !EXTENSION_H_ */
