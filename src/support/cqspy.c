#include <stdio.h>

main()
{
  setuid(geteuid());
  printf("%c[H%c[JThe following people have already played conquer:\n\n",
    (char) 27, (char) 27);
  system("/bin/csh -c /home/sol3/cs7/quixadhal/conquer4/libro/cqspy.csh");
  printf("\nBe seeing you...\n");
  setuid(getuid());
}
