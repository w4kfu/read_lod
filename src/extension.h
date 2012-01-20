#ifndef EXTENSION_H_
# define EXTENSION_H_

typedef struct
{
  char	*name_ext;
  int	nb;
  struct h3_ext *next;
} h3_ext;

h3_ext		*add_extension(char *name, h3_ext *ext);
void		print_extension(h3_ext *ext);

#endif /* !EXTENSION_H_ */
