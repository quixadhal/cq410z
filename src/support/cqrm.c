#include <stdio.h>

main(argc, argv)
int argc;
char *argv[];
{
  char line[1024];

  if(argc != 2) exit(10);
  setuid(geteuid());
  sprintf(line, "/tmp/cq%s", argv[1]);
  unlink(line);
}
