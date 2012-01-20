#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <zlib.h>
#include <string.h>
#include "extension.h"

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

extern int optind;
struct h3_ext *lext;

void	help(char *name)
{
  printf("usage : %s [-hl] <*.lod>\n", name);
  printf("-h : print this help\n");
  printf("-l : print list of extension\n");
}

void		hexdump(char *buf, size_t size)
{
  size_t	i;

  for (i = 0; i < size; i++)
    {
      if (i && i % 8 == 0)
	printf("\n");
      if (isprint(buf[i]))
	/* printf("%02X ", (unsigned char)buf[i]); */
	printf("%c ", buf[i]);
      else
      	printf(".");
    }
  printf("\n");
}

int		uncompress_h3file(FILE	*fp, t_h3_file	*h3file)
{
  char		*uncomp = NULL;
  char		*compress = NULL;
  uLongf	destLen;

  compress = malloc(h3file->comsize * sizeof (char));
  if (!compress)
    {
      perror("[-] malloc()");
      return (-1);
    }
  uncomp = malloc(h3file->orgsize * sizeof (char));
  if (!uncomp)
    {
      perror("[-] malloc()");
      return (-1);
    }
  fseek(fp, h3file->offset, SEEK_SET);
  fread(compress, sizeof (char) * h3file->comsize, 1, fp);
  destLen = h3file->orgsize;
  if (uncompress((Bytef*)uncomp, &destLen,
		 (Bytef*)compress, h3file->comsize) != Z_OK)
    {
      printf("[-] Cannot uncompress file\n");
      free(compress);
      free(uncomp);
      return (-1);
    }
  else
    hexdump(uncomp, 24);
  free(compress);
  free(uncomp);
  return (0);
}

int		read_h3file(FILE	*fp, t_h3_file	*h3file)
{
  uint8_t	read[4];
  char		*ext = NULL;

  ext = strstr(h3file->name, ".");
  if (ext)
    lext = add_extension(ext, lext, h3file->type);
  printf("\nName = %s\n", h3file->name);
  if (fseek(fp, h3file->offset, SEEK_SET) == -1)
    {
      perror("[-] fseek()");
      return (-1);
    }
  if (fread(&read, 4, 1, fp) != 1)
    {
      perror("[-] fread()");
      return (-1);
    }
  printf("Offset = 0x%x    :: %02X %02X %02X %02X\n",
	 h3file->offset,
	 read[0], read[1], read[2], read[3]);
  printf("Original Size = %d\n", h3file->orgsize);
  printf("Type = %d\n", h3file->type);
  printf("Compressed Size = %d\n", h3file->comsize);
  uncompress_h3file(fp, h3file);
  return (0);
}

int		read_lodfile(const char *filename)
{
  FILE		*fp = NULL;
  t_h3lod	header_h3lod;
  size_t	i;

  fp = fopen(filename, "rb");
  if (!fp)
    {
      perror("[-] fopen()");
      return (-1);
    }
  fread(&header_h3lod, sizeof (t_h3lod), 1, fp);
  printf("magic = %x\n", header_h3lod.magic);
  if (header_h3lod.magic != 0x00444f4c)
    {
      fprintf(stderr, "[-] Bad signature, not .lod file\n");
      return (-1);
    }
  printf("Type = %d\n", header_h3lod.type);
  printf("NbFile = %d\n", header_h3lod.nb_file);
  for (i = 0; i < header_h3lod.nb_file; i++)
    {
      read_h3file(fp, &header_h3lod.h3file[i]);
    }
  fclose(fp);
  return (0);
}

int main(int argc, char **argv)
{
  int c;

  while ((c = getopt(argc, argv, "hl")) != -1)
    {
      switch(c)
	{
	case 'h':
	  help(argv[0]);
	  exit(EXIT_SUCCESS);
	case 'l':
	  /* print_list(); */
	  break;
	case '?':
	  printf("unknow option\n");
	  help(argv[0]);
	  exit(EXIT_SUCCESS);
	}
    }
  if (optind >= argc)
    {
      help(argv[0]);
      return (-1);
    }
  while (optind < argc)
    read_lodfile(argv[optind++]);
  print_extension(lext);
  clean_extension(lext);
  return (0);
}
