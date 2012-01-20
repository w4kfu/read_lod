#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extension.h"

h3_ext		*add_extension(char *name, h3_ext *ext)
{
  h3_ext	*new_ext = NULL;
  h3_ext	*current = NULL;

  if (!ext)
    {
      new_ext = malloc(sizeof (h3_ext));
      if (!new_ext)
	{
	  perror("[-] malloc");
	  exit(EXIT_FAILURE);
	}
      new_ext->name_ext = strdup(name);
      new_ext->nb = 1;
      new_ext->next = NULL;
      ext = new_ext;
      return (ext);
    }
  current = ext;
  while (current)
    {
      if (!strcmp(current->name_ext, name))
	{
	  current->nb++;
	  return (ext);
	}
      current = (h3_ext*)current->next;
    }
  new_ext = malloc(sizeof (h3_ext));
  if (!new_ext)
    {
      perror("[-] malloc");
      exit(EXIT_FAILURE);
    }
  new_ext->name_ext = strdup(name);
  new_ext->nb = 1;
  new_ext->next = (struct h3_ext*)ext;
  ext = new_ext;
  return (ext);
}


void		print_extension(h3_ext *ext)
{
  h3_ext	*current = NULL;

  current = ext;
  while (current)
    {
      printf("Extension Name = %s, Nb = %d\n", current->name_ext, current->nb);
      current = (h3_ext*)current->next;
    }
}
