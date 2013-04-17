#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

main(argc, argv)
int argc;
char *argv[];
{
  register int i;
  char line[1024];

  setuid(geteuid());
  sprintf(line, "/usr/ucb/rsh sol.cs.wmich.edu -l quixadhal /usr/local/bin/newspy ");
  for(i= 1; i< argc; i++) {
    strcat(line, argv[i]);
    strcat(line, " ");
  }
  system(line);
  setuid(getuid());
}
