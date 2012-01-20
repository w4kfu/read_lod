#include "read_lod.h"

struct h3_ext *lext;

void	help(char *name)
{
  printf("usage : %s [-hl] <*.lod>\n", name);
  printf("-h : print this help\n");
  printf("-l : print list of extension\n");
  printf("-x <path> : extract files to <path>\n");
  printf("-t <nb> : Choose type of file you want (ex : 1, 16, 17, 73, 80 ... )");
  printf("-v : verbose mode\n");
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

int		extract_file(t_conf *config, char *uncomp,
			     uint32_t size, char *name)
{
  char		*file_path = NULL;
  FILE		*fp;

  file_path = malloc(config->size_path * sizeof(char)
		     + strlen(name) * sizeof(char) + 3);
  if (!file_path)
    {
      perror("[-] malloc()");
      exit(EXIT_FAILURE);
    }
  strcpy(file_path, config->path_extract);
  strcat(file_path, "/");
  strcat(file_path, name);
  fp = fopen(file_path, "wb");
  if (!fp)
    {
      perror("[-] fopen()");
      exit(EXIT_FAILURE);
    }
  fwrite(uncomp, size, 1, fp);
  fclose(fp);
  free(file_path);
  return (0);
}

int		uncompress_h3file(FILE	*fp, t_h3_file	*h3file,
				  t_conf *config)
{
  char		*uncomp = NULL;
  char		*compress = NULL;
  uLongf	destLen;

  compress = malloc(h3file->comsize * sizeof (char));
  if (!compress)
    {
      perror("[-] malloc()");
      exit(EXIT_FAILURE);
    }
  uncomp = malloc(h3file->orgsize * sizeof (char));
  if (!uncomp)
    {
      perror("[-] malloc()");
      exit(EXIT_FAILURE);
    }
  fseek(fp, h3file->offset, SEEK_SET);
  fread(compress, sizeof (char) * h3file->comsize, 1, fp);
  destLen = h3file->orgsize;
  if (uncompress((Bytef*)uncomp, &destLen,
		 (Bytef*)compress, h3file->comsize) != Z_OK)
    {
      if (config->verbose)
	printf("[-] Cannot uncompress file\n");
      free(compress);
      free(uncomp);
      return (-1);
    }
  if (config->verbose)
    hexdump(uncomp, 24);
  if (config->extract)
    {
      if ((config->type && config->type == h3file->type) || !config->type)
	extract_file(config, uncomp, h3file->orgsize, h3file->name);
    }
  free(compress);
  free(uncomp);
  return (0);
}

int		read_h3file(FILE	*fp, t_h3_file	*h3file, t_conf *config)
{
  uint8_t	read[4];
  char		*ext = NULL;

  ext = strstr(h3file->name, ".");
  if (ext)
    lext = add_extension(ext, lext, h3file->type);
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
  if (config->verbose)
    {
      printf("\nName = %s\n", h3file->name);
      printf("Offset = 0x%x    :: %02X %02X %02X %02X\n",
	     h3file->offset,
	     read[0], read[1], read[2], read[3]);
      printf("Original Size = %d\n", h3file->orgsize);
      printf("Type = %d\n", h3file->type);
      printf("Compressed Size = %d\n", h3file->comsize);
    }
  uncompress_h3file(fp, h3file, config);
  return (0);
}

int		read_lodfile(const char *filename, t_conf *config)
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
  if (header_h3lod.magic != 0x00444f4c)
    {
      fprintf(stderr, "[-] Bad signature, not .lod file\n");
      return (-1);
    }
  if (config->verbose)
    {
      printf("magic = %x\n", header_h3lod.magic);
      printf("Type = %d\n", header_h3lod.type);
      printf("NbFile = %d\n", header_h3lod.nb_file);
    }
  for (i = 0; i < header_h3lod.nb_file; i++)
    read_h3file(fp, &header_h3lod.h3file[i], config);
  fclose(fp);
  return (0);
}

int		check_path(t_conf *conf)
{
  struct stat	sbuf;

  if (stat(conf->path_extract, &sbuf) == -1)
    {
      perror("[-] stat()");
      return (-1);
    }
  if ((sbuf.st_mode & S_IFMT) != S_IFDIR)
    {
      printf("%s is not a directory\n", conf->path_extract);
      return (-1);
    }
  if (access(conf->path_extract, W_OK) == -1)
    {
      printf("No write access on path : %s\n", conf->path_extract);
      return (-1);
    }
  return (0);
}

int		main(int argc, char **argv)
{
  int		c;
  extern char	*optarg;
  extern int	optind, optopt;
  struct conf	config;

  config.print = 0;
  config.extract = 0;
  config.verbose = 0;
  config.type = 0;
  while ((c = getopt(argc, argv, "hlx:vt:")) != -1)
    {
      switch(c)
	{
	case 'h':
	  help(argv[0]);
	  exit(EXIT_SUCCESS);
	case 'l':
	  config.print = 1;
	  break;
	case 'v':
	  config.verbose = 1;
	  break;
	case 'x':
	  config.extract = 1;
	  config.path_extract = optarg;
	  config.size_path = strlen(optarg);
	  break;
	case ':':
	  printf("-%c without path\n", optopt);
	  exit(EXIT_FAILURE);
	case 't':
	  config.type = atoi(optarg);
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
  if (config.extract && check_path(&config))
    exit(EXIT_FAILURE);
  while (optind < argc)
    read_lodfile(argv[optind++], &config);
  if (config.print)
    print_extension(lext);
  clean_extension(lext);
  return (0);
}
