/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: spewout.c,v $
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.2  1991/08/06  00:33:02  quixadhal
 * Added RCS header information for logging.
 *
 *
 */

#include <stdio.h>

main(argc, argv)
  int             argc;
  char           *argv[];
{
  FILE           *fp,
                 *fopen();
  int             i,
                  j,
                  l,
                  num;

  for (i = 1; i < argc; i++) {
    switch (argv[i][0]) {
     case '0':
     case '1':
     case '2':
     case '3':
     case '4':
     case '5':
     case '6':
     case '7':
     case '8':
     case '9':
      /* assign new comparison number */
      num = 0;
      l = strlen(argv[i]);
      for (j = 0; j < l && argv[i][j] >= '0' && argv[i][j] <= '9'; j++) {
	num *= 10;
	num += argv[i][j] - '0';
      }
      /* compensate for increment on exit */
      j--;
      break;
     default:
      fprintf(stderr, "%s: invalid option '%c' in <%s>\n",
	      argv[0], argv[i][j], argv[i]);
      exit(1);
      break;
    }
  }

  while (1) {
    fprintf(stdout, "\n");
    makemess(1, stdout);
    fprintf(stdout, "\n");
    sleep(num);
  }
}
