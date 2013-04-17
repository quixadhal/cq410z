/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: newsdigest.c,v $
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.2  1991/08/08  01:43:16  quixadhal
 * patched turn counter and "nice"ned up header.
 *
 * Revision 1.1  1991/08/08  01:00:53  quixadhal
 * Initial revision
 *
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "header.h"

void
add_to_newsdigest(filename, turn, keep)
char *filename;
int turn;
int keep;
{
  FILE *ifp, *ofp;
  char buffer[81];

  if(!(ifp= fopen(filename, "r"))) return;
  if(!(ofp= fopen("digest", "a"))) {
    if(ifp) fclose(ifp);
    return;
  }
  fprintf(ofp, "**** Personal Items and Insults from Conquer Turn %d ****\n",
    turn- keep);
  while(!feof(ifp))
    if(fgets(buffer, 80, ifp))
      if((buffer[0]== '5')&&(buffer[1]== '.'))
        fputs(&buffer[2], ofp);
  fprintf(ofp, "----\n");
  if(ofp) fclose(ofp);
  chmod("digest", (mode_t)FCMASK);
  if(ifp) fclose(ifp);
}


