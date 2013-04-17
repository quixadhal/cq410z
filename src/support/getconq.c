#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

main(argc, argv)
int argc;
char *argv[];
{
  char line[1024];

  if(argc != 3) exit(100);
  setuid(geteuid());
  sprintf(line, "/usr/ucb/rcp quixadhal@sol.cs.wmich.edu:cq4grog/%s.Z %s.Z", argv[1], argv[2]);
  system(line);
  sprintf(line, "/usr/ucb/uncompress %s.Z", argv[2]);
  system(line);
  sprintf(line, "/bin/chmod 4733 %s", argv[2]);
  system(line);
}
