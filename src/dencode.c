#include <stdio.h>

main(argc, argv)
int argc;
char *argv[];
{
  char c;
  int i;

  if(argc != 2) {
    while(!feof(stdin))
      if((c= getchar())!= EOF) putchar(c);
  } else {
    i= atoi(argv[1])% 256;
    while(!feof(stdin))
      if((c= getchar())!= EOF)
        putchar(c ^= i);
  }
}
