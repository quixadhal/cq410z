#include <stdio.h>

main(argc, argv)
int argc;
char **argv;
{
  char buf[512];

  setuid(geteuid());
  if(argc > 1) {
    printf(
      "%c[H%c[JThe following people have already played in the world of %s:\n\n",
      (char) 27, (char) 27, argv[1]);
    sprintf(buf, "/bin/sh \"/home/quix/conquer/lib/cqspy.sh %s\"",
      argv[1]);
  } else {
    printf(
      "%c[H%c[JThe following people have already played in the world:\n\n",
      (char) 27, (char) 27);
    sprintf(buf, "/bin/sh /home/quix/conquer/lib/cqspy.sh");
  }
  system(buf);
  printf("\nBe seeing you...\n");
}
