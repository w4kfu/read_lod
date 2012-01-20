#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extension.h"

struct h3_type	*new_type(uint32_t type)
{
  struct h3_type	*new_type = NULL;

  new_type = malloc(sizeof (struct h3_type));
  if (!new_type)
    {
      perror("[-] malloc");
      exit(EXIT_FAILURE);
    }
  new_type->type = type;
  new_type->next = NULL;
  return (new_type);
}

struct h3_type	*add_type(struct h3_type *ltype, uint32_t type)
{
  struct h3_type	*current_type = NULL;
  struct h3_type	*new = NULL;

  if (!ltype)
    return (new_type(type));
  current_type = ltype;
  while (current_type)
    {
      /* Already exist */
      if (current_type->type == type)
	return (ltype);
      current_type = current_type->next;
    }
  new = new_type(type);
  new->next = ltype;
  return (new);
}

struct h3_ext	*new_extension(char *name, uint32_t type)
{
  struct h3_ext *new_ext = NULL;

  new_ext = malloc(sizeof (struct h3_ext));
  if (!new_ext)
    {
      perror("[-] malloc");
      exit(EXIT_FAILURE);
    }
  new_ext->name_ext = strdup(name);
  new_ext->nb = 1;
  new_ext->type = new_type(type);
  new_ext->next = NULL;
  return (new_ext);
}

struct h3_ext	*add_extension(char *name, struct h3_ext *ext, uint32_t type)
{
  struct h3_ext	*new_ext = NULL;
  struct h3_ext	*current = NULL;

  if (!ext)
    return (new_extension(name, type));
  current = ext;
  while (current)
    {
      if (!strcmp(current->name_ext, name))
	{
	  current->nb++;
	  current->type = add_type(current->type, type);
	  return (ext);
	}
      current = current->next;
    }
  new_ext = new_extension(name, type);
  new_ext->next = ext;
  return (new_ext);
}

void		print_type(struct h3_ext *ext)
{
  struct h3_type *current = NULL;

  current = ext->type;
  printf("Type = [ ");
  while (current)
    {
      if (current->next)
	printf("%d, ", current->type);
      else
	printf("%d", current->type);
      current = current->next;
    }
  printf(" ]\n");
}

void		print_extension(struct h3_ext *ext)
{
  struct h3_ext	*current = NULL;

  current = ext;
  while (current)
    {
      printf("Extension Name = %s ; Nb = %d ; ", current->name_ext, current->nb);
      print_type(current);
      current = current->next;
    }
}

void	clean_extension(struct h3_ext *ext)
{
  struct h3_ext *current = NULL;

  current = ext;
  while (current)
    {
      free(ext->name_ext);
      current = ext->next;
      free(ext);
      ext = current;
    }
}
