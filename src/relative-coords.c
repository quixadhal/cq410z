/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: relative-coords.c,v $
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.4  1991/08/09  06:02:54  quixadhal
 * More relative coord stuff...
 *
 * Revision 1.3  1991/08/09  03:26:16  quixadhal
 * Added rel->abs routines.
 *
 * Revision 1.2  1991/08/09  00:34:31  quixadhal
 * Flipped y-axis to be similar to math coordinate system.
 *
 *
 */

#include "header.h"
#include "data.h"

extern struct s_nation *curntn;
extern int country;

int rel_x(int absx)
{
  int relx;

#ifdef DEBUG
  printf("line 35:absx = %d\n", absx);
#endif
  absx %= MAPX;
#ifdef DEBUG
  printf("line 39:absx = %d\n", absx);
#endif
#ifdef USE_RELATIVE_COORDS
#ifdef DEBUG
  printf("line 44:country = %d\n", country);
  printf("line 45:curntn = %s\n", curntn->name);
  printf("line 46:ntn[country].rlx = %d\n", ntn[country].rlx);
#endif
  if ((country != 0) && (curntn != NULL)) {
    relx = absx - curntn->rlx;
#ifdef DEBUG
    printf("line 52:relx = %d\n", relx);
#endif
    if (relx > MAPX / 2) {
      relx -= MAPX;
    }
#ifdef DEBUG
    printf("line 58:relx = %d\n", relx);
#endif
    if (relx <= -(MAPX / 2)) {
      relx += MAPX;
    }
#ifdef DEBUG
    printf("line 64:relx = %d\n", relx);
#endif
  } else {
    relx = absx;
  }
#else
  relx = absx;
#endif
#ifdef DEBUG
  printf("line 73:return relx = %d\n", relx);
#endif
  return (relx);
}

int rel_y(int absy)
{
  int rely;

  absy %= MAPY;
#ifdef USE_RELATIVE_COORDS
  if ((country != 0) && (curntn != NULL)) {
    rely = absy - curntn->rly;
    if (rely > MAPY / 2) {
      rely -= MAPY;
    }
    if (rely <= -(MAPY / 2)) {
      rely += MAPY;
    }
    rely = -rely;
  } else {
    rely = absy;
  }
#else
  rely = absy;
#endif
  return (rely);
}

int abs_x(int relx)
{
  int absx;

#ifdef USE_RELATIVE_COORDS
  if ((country != 0) && (curntn != NULL)) {
    absx = (relx + curntn->rlx);
    while (absx < 0)
      absx = MAPX + absx;
    absx %= MAPX;
  } else {
    absx = relx;
  }
#else
  absx = relx;
#endif
  return (absx);
}

int abs_y(int rely)
{
  int absy;

#ifdef USE_RELATIVE_COORDS
  if ((country != 0) && (curntn != NULL)) {
    absy = ((-rely) + curntn->rly);
    while (absy < 0)
      absy = MAPY + absy;
    absy %= MAPY;
  } else {
    absy = rely;
  }
#else
  absy = rely;
#endif
  return (absy);
}
