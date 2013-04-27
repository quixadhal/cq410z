/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: io.c,v $
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.4  1991/08/09  03:56:01  quixadhal
 * Added relative coordinates to Jump command.
 *
 * Revision 1.3  1991/08/06  00:33:02  quixadhal
 * Added RCS header information for logging.
 *
 *
 */

/*io.c*/
/*Print and io subroutines for game*/

/*conquer : Copyright (c) 1988 by Ed Barlow.
 *  I spent a long time writing this code & I hope that you respect this.
 *  I give permission to alter the code, but not to copy or redistribute
 *  it without my explicit permission.  If you alter the code,
 *  please document changes and send me a copy, so all can have it.
 *  This code, to the best of my knowledge works well,  but it is my first
 *  'C' program and should be treated as such.  I disclaim any
 *  responsibility for the codes actions (use at your own risk).  I guess
 *  I am saying "Happy gaming", and am trying not to get sued in the process.
 *                                                Ed
 */

#include "header.h"
#include "data.h"
#include "patchlevel.h"
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h> /* strlen() */


extern FILE *fexe;

/*offset of upper left hand corner*/
extern short xoffset, yoffset;

/*current cursor postion (relative to 00 in upper corner)*/
/*	position is 2*x,y*/
extern short xcurs, ycurs;
extern short redraw;           /* redraw map in this turn if redraw is a 1 */
extern short hilmode, dismode; /* display state */
extern short country;          /* nation id of owner */

void getspace(void);
void writedata(void);
void readdata(void);
void flee(int x, int y, int isupd, int slaver);
char **m2alloc(int nrows, int ncols, int entrysize);
int get_pass(char *str);


/************************************************************************/
/*	GETSPACE() - malloc all space needed	 			*/
/************************************************************************/
void getspace(void) {
  if (sct != NULL)
    free(sct);
  sct = (struct s_sector **)m2alloc(MAPX, MAPY, sizeof(struct s_sector));
  if (occ != NULL)
    free(occ);
  occ = (char **)m2alloc(MAPX, MAPY, sizeof(char));
  if (movecost != NULL)
    free(movecost);
  movecost = (short **)m2alloc(MAPX, MAPY, sizeof(short));
}

#ifdef CONQUER
char **mapseen;

void mapprep(void);
void printele(void);
void pr_ntns(void);
void pr_desg(void);
void printveg(void);

/************************************************************************/
/*	MAPPREP() - initialize map with what can be seen by nation.	*/
/************************************************************************/
void mapprep(void) {
  int armynum, nvynum;
  int x, y, i, j;

  /* get space for map */
  mapseen = (char **)m2alloc(MAPX, MAPY, sizeof(char));

  /* initialize the array */
  if (country == 0 || magic(country, KNOWALL) == TRUE) {
    armynum = TRUE;
  } else {
    armynum = FALSE;
  }
  for (x = 0; x < MAPX; x++)
    for (y = 0; y < MAPY; y++) {
      mapseen[x][y] = armynum;
    }

  /* done for all knowing */
  if (country == 0 || magic(country, KNOWALL) == TRUE)
    return;

  /* add all visible sectors from owned land */
  for (x = 0; x < MAPX; x++)
    for (y = 0; y < MAPY; y++)
      if (sct[x][y].owner == country) {
        for (i = x - LANDSEE; i <= x + LANDSEE; i++)
          for (j = y - LANDSEE; j <= y + LANDSEE; j++)
            if (ONMAP(i, j)) {
              mapseen[i][j] = TRUE;
            }
      }
  /* now add all visible sections from armies */
  for (armynum = 0; armynum < MAXARM; armynum++)
    if (P_ASOLD > 0) {
      for (i = (int) P_AXLOC - ARMYSEE; i <= (int) P_AXLOC + ARMYSEE; i++)
        for (j = (int) P_AYLOC - ARMYSEE; j <= (int) P_AYLOC + ARMYSEE; j++)
          if (ONMAP(i, j)) {
            mapseen[i][j] = TRUE;
          }
    }
  /* now add sectors visible by navy */
  for (nvynum = 0; nvynum < MAXNAVY; nvynum++)
    if ((P_NMSHP != 0) || (P_NWSHP != 0) || (P_NGSHP != 0)) {
      for (i = (int) P_NXLOC - NAVYSEE; i <= (int) P_NXLOC + NAVYSEE; i++)
        for (j = (int) P_NYLOC - NAVYSEE; j <= (int) P_NYLOC + NAVYSEE; j++)
          if (ONMAP(i, j)) {
            mapseen[i][j] = TRUE;
          }
    }
}

/************************************************************************/
/*	PRINTELE() - print a sector.altitude map 			*/
/************************************************************************/
void printele(void) {
  register int X, Y;

  fprintf(stderr, "doing print of altitude\n");
  if (country == 0) {
    printf("Conquer %s.%d.%s: Altitude Map of the World on Turn %d\n", VERSION,
           PATCHLEVEL, EXTPATCH, TURN);
  } else {
    printf("Conquer %s.%d.%s: Altitude Map for Nation %s on Turn %d\n", VERSION,
           PATCHLEVEL, EXTPATCH, curntn->name, TURN);
  }
  for (Y = 0; Y < MAPY; Y++) {
    for (X = 0; X < MAPX; X++) {
      if (mapseen[X][Y] == TRUE)
        putc(sct[X][Y].altitude, stdout);
      else
        putc(' ', stdout);
    }
    putc('\n', stdout);
  }
}

/************************************************************************/
/*	PR_NTNS() - print nation marks					*/
/************************************************************************/
void pr_ntns(void) {
  register int X, Y;

  fprintf(stderr, "doing print of nations\n");
  if (country == 0) {
    printf("Conquer %s.%d.%s: Nation Map of the World on Turn %d\n", VERSION,
           PATCHLEVEL, EXTPATCH, TURN);
  } else {
    printf("Conquer %s.%d.%s: Nation Map for Nation %s on Turn %d\n", VERSION,
           PATCHLEVEL, EXTPATCH, curntn->name, TURN);
  }
  for (Y = 0; Y < MAPY; Y++) {
    for (X = 0; X < MAPX; X++) {
      if (mapseen[X][Y] == TRUE) {
        if (sct[X][Y].owner == 0)
          putc(sct[X][Y].altitude, stdout);
        else
          putc(ntn[sct[X][Y].owner].mark, stdout);
      } else
        putc(' ', stdout);
    }
    putc('\n', stdout);
  }
}

/************************************************************************/
/*	PR_DESG() - print designations					*/
/************************************************************************/
void pr_desg(void) {
  register int X, Y;

  fprintf(stderr, "doing print of designations\n");
  if (country == 0) {
    printf("Conquer %s.%d.%s: Designation Map of the World on Turn %d\n",
           VERSION, PATCHLEVEL, EXTPATCH, TURN);
  } else {
    printf("Conquer %s.%d.%s: Designation Map for Nation %s on Turn %d\n",
           VERSION, PATCHLEVEL, EXTPATCH, curntn->name, TURN);
  }
  for (Y = 0; Y < MAPY; Y++) {
    for (X = 0; X < MAPX; X++) {
      if (mapseen[X][Y] == TRUE) {
        if ((country == 0) || (sct[X][Y].owner == country) ||
            (magic(country, NINJA) == TRUE) ||
            (magic(sct[X][Y].owner, THE_VOID) != TRUE)) {
          if (sct[X][Y].designation == DNODESIG)
            putc(sct[X][Y].altitude, stdout);
          else
            putc(sct[X][Y].designation, stdout);
        } else
          putc('?', stdout);
      } else
        putc(' ', stdout);
    }
    putc('\n', stdout);
  }
}

/************************************************************************/
/*	PRINTVEG() -	print a vegetation map subroutine		*/
/************************************************************************/
void printveg(void) {
  register int X, Y;

  fprintf(stderr, "doing print of vegetation\n");
  if (country == 0) {
    printf("Conquer %s.%d.%s: Vegetation Map of the World on Turn %d\n",
           VERSION, PATCHLEVEL, EXTPATCH, TURN);
  } else {
    printf("Conquer %s.%d.%s: Vegetation Map for Nation %s on Turn %d\n",
           VERSION, PATCHLEVEL, EXTPATCH, curntn->name, TURN);
  }
  for (Y = 0; Y < MAPY; Y++) {
    for (X = 0; X < MAPX; X++) {
      if (mapseen[X][Y] == TRUE) {
        putc(sct[X][Y].vegetation, stdout);
      } else
        putc(' ', stdout);
    }
    putc('\n', stdout);
  }
}

#endif /* CONQUER */

/************************************************************************/
/*	WRITEDATA() - write data to datafile 				*/
/*	trashes/creates datafile in the process				*/
/************************************************************************/
void writedata(void) {
  FILE *fp;
  int n_write, fh, count;
  char line[256], hack[256];
/* I think I can just remove these calls to sigblock(), since the signal will
 * be ignored, delivered or not.
  sigblock(sigmask(SIGTSTP));
  sigblock(sigmask(SIGHUP));
  sigblock(sigmask(SIGINT));
  sigblock(sigmask(SIGTERM)); */
  signal(SIGTSTP, SIG_IGN);
  signal(SIGHUP, SIG_IGN);
  signal(SIGINT, SIG_IGN);
  signal(SIGTERM, SIG_IGN);

  fprintf(stderr, "\ndoing write of data\n");
  count = 0;

  sprintf(hack, "%s-hack", datafile);
  while (access(hack, F_OK) != -1) {
    count++;
    if (count > 6) {
      fprintf(stderr, "*** DATA FILE IS LOCKED!!! ABORTING!!! ***");
      exit(10);
    }
    usleep(random() % 5000 + 100);
  }
  if ((fh = open(hack, O_CREAT | O_WRONLY, 0600)) < 0) {
    fprintf(stderr, "*** LOCK STILL FAILED... ABORTING! ***");
    exit(10);
  }

#ifdef USE_COMPRESS
#ifdef DEBUG
  fprintf(stderr, "Opening pipe to compress %s.%s\n", datafile, ZEXT);
#endif
  sprintf(line, "%s.%s", datafile, ZEXT);
  chmod(line, (mode_t) FCMASK);
  sprintf(line, "%s > %s.%s", COMPRESS, datafile, ZEXT);
  if (!(fp = popen(line, "w"))) {
    fprintf(stderr, "cant open data.  check permissions\n");
    abrt();
  }
#else
  chmod(datafile, (mode_t) FCMASK);
  if (!(fp = fopen(datafile, "w"))) {
    fprintf(stderr, "cant open data.  check permissions\n");
    abrt();
  }
#endif

/* write world structure */

#ifdef DEBUG
  fprintf(stderr, "writing %lu bytes of world data\n", sizeof(struct s_world));
#endif
  if ((n_write = fwrite(&world, sizeof(struct s_world), 1, fp)) != 1) {
    fprintf(stderr, "error writing world data\n");
    fprintf(stderr, "wrong data format (%lu vs. %lu)\n",
            n_write * sizeof(struct s_world), sizeof(struct s_world));
    abrt();
  }
/* write map */

#ifdef DEBUG
  fprintf(stderr, "writing %lu bytes of sector data\n",
          MAPX * MAPY * sizeof(struct s_sector));
#endif
  if ((n_write = fwrite(*sct, sizeof(struct s_sector), MAPX * MAPY, fp)) !=
      (MAPX * MAPY)) {
    fprintf(stderr,
            "Wrong number of bytes (%lu) written for sct (should be %lu)\n",
            n_write * sizeof(struct s_sector),
            MAPX * MAPY * sizeof(struct s_sector));
    abrt();
  }
  ;
/* write nations */

#ifdef DEBUG
  fprintf(stderr, "writing %lu bytes of nation data\n",
          NTOTAL * sizeof(struct s_nation));
#endif
  if ((n_write = fwrite(ntn, sizeof(struct s_nation), NTOTAL, fp)) != NTOTAL) {
    fprintf(
        stderr, "Wrong number of bytes (%lu) written for ntn (should be %lu)\n",
        n_write * sizeof(struct s_nation), NTOTAL * sizeof(struct s_nation));
    abrt();
  }
#ifdef USE_COMPRESS
#ifdef DEBUG
  fprintf(stderr, "Closing pipe to compress %s.%s\n", datafile, ZEXT);
#endif
  pclose(fp);
  sprintf(line, "%s.%s", datafile, ZEXT);
  chmod(line, (mode_t) FCMASK);
#else
  fclose(fp);
  sprintf(line, "%s.%s", datafile, ZEXT);
  chmod(line, (mode_t) FCMASK);
#endif
  sync();
  if (fh)
    close(fh);
  unlink(hack);
}

/************************************************************************/
/*	READDATA()	-	read data & malloc space		*/
/************************************************************************/
void readdata(void) {
  FILE *fp;
  int n_read;
  char line[256];

  /* read in existing nation army and navy data */
  /* check if file openable */
  fprintf(stderr, "reading data file\n");

#ifdef USE_COMPRESS
#ifdef DEBUG
  fprintf(stderr, "Opening pipe to read %s.%s\n", datafile, ZEXT);
#endif
  sprintf(line, "%s < %s.%s", ZCAT, datafile, ZEXT);
  if (!(fp = popen(line, "r")) || feof(fp)) {
    fprintf(stderr, "can not open %s \n", datafile);
    fprintf(stderr, "for help with conquer, type conquer -h\n");
    exit(FAIL);
  }
#else
  if (!(fp = fopen(datafile, "r"))) {
    fprintf(stderr, "can not open %s \n", datafile);
    fprintf(stderr, "for help with conquer, type conquer -h\n");
    exit(FAIL);
  }
#endif

#ifdef DEBUG
  fprintf(stderr, "reading %lu bytes of world data\n", sizeof(struct s_world));
#endif /* DEBUG */

  /* read world structure */
  if ((n_read = fread(&world, sizeof(struct s_world), 1, fp)) != 1) {
    fprintf(stderr, "error reading world data\n");
    fprintf(stderr, "wrong data format (%lu vs. %lu)\n",
            n_read * sizeof(struct s_world), sizeof(struct s_world));
    abrt();
  }

  getspace();

#ifdef DEBUG
  fprintf(stderr, "reading %lu bytes of sector data\n",
          MAPX * MAPY * sizeof(struct s_sector));
#endif /* DEBUG */

  if ((n_read = fread(*sct, sizeof(struct s_sector), MAPX * MAPY, fp)) !=
      (MAPX * MAPY)) {
    fprintf(stderr, "error reading sector data (sct)\n");
    fprintf(stderr, "wrong data format (%lu vs. %lu)\n",
            n_read * sizeof(struct s_sector),
            MAPX * MAPY * sizeof(struct s_sector));
    abrt();
  }

#ifdef DEBUG
  fprintf(stderr, "reading %lu bytes of nation data\n",
          NTOTAL * sizeof(struct s_nation));
#endif /* DEBUG */

  if ((n_read = fread(ntn, sizeof(struct s_nation), NTOTAL, fp)) != NTOTAL) {
    fprintf(stderr, "error reading s_nation data (ntn)\n");
    fprintf(stderr, "wrong data format (%lu vs. %lu)\n",
            n_read * sizeof(struct s_nation), NTOTAL * sizeof(struct s_nation));
    abrt();
  }

#ifdef USE_COMPRESS
#ifdef DEBUG
  fprintf(stderr, "Closing read pipe to %s.%s\n", datafile, ZEXT);
#endif
  pclose(fp);
#else
  fclose(fp);
#endif
} /* readdata() */

#ifdef CONQUER

#ifdef XYZZY

void offmap(void);

/************************************************************************/
/*	OFFMAP()	deal if cursor is off the map			*/
/************************************************************************/
void offmap(void) {
  /* set offset offsets can not be < 0 */
  if (xcurs < 1) {
    if (XREAL <= 0) {
      xoffset = 0;
      xcurs = 0;
    } else {
      redraw = PART;
      xoffset -= 15;
      xcurs += 15;
    }
  } else if (xcurs >= SCREEN_X_SIZE - 1) {
    if (XREAL < MAPX) {
      redraw = PART;
      xoffset += 15;
      xcurs -= 15;
    }
  }
  if (XREAL >= MAPX)
    xcurs = MAPX - 1 - xoffset;
  if (xoffset < 0) {
    xcurs += xoffset;
    xoffset = 0;
  }
  if (xcurs < 0) {
    xoffset += xcurs;
    xcurs = 0;
  } else if (xcurs >= SCREEN_X_SIZE - 1) {
    redraw = PART;
    xoffset += 15;
    xcurs -= 15;
  }
  if (ycurs < 1) {
    if (YREAL <= 0) {
      yoffset = 0;
      ycurs = 0;
    } else {
      redraw = PART;
      ycurs += 15;
      yoffset -= 15;
    }
  } else if (ycurs >= SCREEN_Y_SIZE - 1) {
    if (YREAL < MAPY) {
      redraw = PART;
      yoffset += 15;
      ycurs -= 15;
    }
  }
  if (YREAL >= MAPY)
    ycurs = MAPY - 1 - yoffset;
  if (yoffset < 0) {
    ycurs += yoffset;
    yoffset = 0;
  }
  if (ycurs < 0) {
    yoffset += ycurs;
    ycurs = 0;
  } else if (ycurs >= SCREEN_Y_SIZE - 1) {
    redraw = PART;
    yoffset += 15;
    ycurs -= 15;
  }
  whatcansee();
}

#endif /* XYZZY */

void centermap(void);
void jump_to(int home);
void printscore(void);

/************************************************************************/
/*	CENTERMAP()	- redraws screen so that cursor is centered	*/
/************************************************************************/
void centermap(void) {
  int xx, yy;

  xx = XREAL;
  yy = YREAL;
  xoffset = xx - (SCREEN_X_SIZE / 2);
  yoffset = yy - (SCREEN_Y_SIZE / 2);
  if (xoffset < 0)
    xoffset = 0;
  if (yoffset < 0)
    yoffset = 0;
  xcurs = xx - xoffset;
  ycurs = yy - yoffset;
  whatcansee();
}

/************************************************************************/
/*   JUMP_TO()      - move screen position to a specific location       */
/*                    home indicates just go to capitol sector.         */
/************************************************************************/
void jump_to(int home)
{
  int i, j, done;
  static int next_ntn;

  /* find location to jump to */
  if (home) {
    if (country == 0) {
      /* check if in sequence */
      if ((XREAL != ntn[next_ntn].capx) || (YREAL != ntn[next_ntn].capy)) {
        next_ntn = 0;
      }
      /* find next capitol */
      done = FALSE;
      do {
        next_ntn++;
        if (next_ntn == NTOTAL) {
          j = 0;
          for (i = 0; i < NTOTAL; i++)
            if (isntn(ntn[i].active)) {
              j = i;
              i = NTOTAL;
            }
          next_ntn = j;
          done = TRUE;
        } else {
          if (isntn(ntn[next_ntn].active)) {
            done = TRUE;
          }
        }
      } while (done == FALSE);
      /* default location; or next capitol */
      if (next_ntn == 0) {
        i = MAPX / 2 - 1;
        j = MAPY / 2 - 1;
      } else {
        i = ntn[next_ntn].capx;
        j = ntn[next_ntn].capy;
      }
    } else {
      /* go to capitol */
      i = curntn->capx;
      j = curntn->capy;
    }
  } else {
    /* entered location */
    clear_bottom(0);
    mvaddstr(LINES - 3, 0, "Jump to what X location? ");
    refresh();
    i = abs_x(get_number());
    if (i == (-1))
      return;
    if (i >= MAPX) {
      errormsg("That location is out of this world!");
      return;
    }
    mvaddstr(LINES - 2, 0, "Jump to what Y location? ");
    refresh();
    j = abs_y(get_number());
    if (j == (-1))
      return;
    if (j >= MAPY) {
      errormsg("That location is out of this world!");
      return;
    }
  }
  /* now center location about given position */
  xcurs = i;
  ycurs = j;
  xoffset = yoffset = 0;
  centermap();
}

/************************************************************************/
/*	PRINTSCORE()	- like it says					*/
/************************************************************************/
void printscore(void) {
  int i;
  int nationid; /* current nation id */

#ifdef TIMELOG
  FILE *timefp, *fopen();
  char timestr[LINELTH + 1];

#endif /* TIMELOG */

  printf("Conquer %s.%d.%s: %s of Year %d, Turn %d\n", VERSION, PATCHLEVEL,
         EXTPATCH, PMONTH(TURN), YEAR(TURN), TURN);

#ifdef TIMELOG
  if ((timefp = fopen(timefile, "r")) != NULL) {
    fgets(timestr, 50, timefp);
    printf("Last Update: %s", timestr);
    fclose(timefp);
  }
#endif /* TIMELOG */

  printf("id      name   race    class    align  score    talons military  "
         "civilians sect\n");
  for (nationid = 1; nationid < NTOTAL; nationid++) {
    if (!isactive(ntn[nationid].active))
      continue;
    printf("%2d ", nationid);
    printf("%9s ", ntn[nationid].name);
    /* this check for old 'B' for barbarians; removed eventually */
    if (ntn[nationid].race == 'B') {
      printf("%6s ", "SAVAGE");
    } else {
      for (i = 1;(*(races + i)[0]) != 'U'; i++)
        if (ntn[nationid].race == *(races + i)[0])
          printf("%6s ", *(races + i));
    }

    printf("%8s ", *(Class + ntn[nationid].class));
    printf(" %7s ", alignment[npctype(ntn[nationid].active)]);
    if (isntn(ntn[nationid].active)) {

#ifdef NOSCORE
      printf("%6ld  %8s %8s   %8s %4s\n", ntn[nationid].score, "-----", "----",
             "-----", "--");
#else
      printf("%6ld  %8ld %8ld   %8ld %4d\n", ntn[nationid].score,
             ntn[nationid].tgold, ntn[nationid].tmil, ntn[nationid].tciv,
             ntn[nationid].tsctrs);
#endif /* NOSCORE */
    } else {
      printf("%6s  %8s %8s   %8s %4s\n", "---", "-----", "----", "-----", "--");
    }
  }
}

#endif /* CONQUER */

/************************************************************************/
/*	FLEE() - civilains in x,y flee from somebody			*/
/*	slaver means 25% of populace stays				*/
/* 	isupd is TRUE if it is update					*/
/************************************************************************/
void flee(int x, int y, int isupd, int slaver)
{
  int count = 0; /* count is number of acceptable sectors to
				 * go to */
  int svcountry = country;
  int slaves = 0;
  int i, j;
  int people_to_add;

  country = sct[x][y].owner;
  if (slaver == TRUE) {
    slaves = sct[x][y].people / 4;
    sct[x][y].people -= slaves;
  }
  /* flee */
  sct[x][y].people *= 6;
  sct[x][y].people /= 10;
  /* check if next to anybody of the sectors owners race */
  for (i = x - 2; i <= x + 2; i++)
    for (j = y - 2; j <= y + 2; j++)
      if (ONMAP(i, j) &&
          (ntn[sct[i][j].owner].race == ntn[sct[x][y].owner].race))
        count++;

  if (count > 0) {

#ifdef CONQUER
    if (isupd == 0) {
      if (slaver == TRUE) {
        mvprintw(LINES - 2, 20, "CIVILIANS ABANDON SECTOR (%d slaves)", slaves);
      } else {
        mvaddstr(LINES - 2, 20, "CIVILIANS ABANDON SECTOR");
      }
    }
#endif /* CONQUER */

    for (i = x - 2; i <= x + 2; i++)
      for (j = y - 2; j <= y + 2; j++)
        if (ONMAP(i, j) &&
            (ntn[sct[i][j].owner].race == ntn[sct[x][y].owner].race)) {
          people_to_add = sct[x][y].people / count;
          /* don't show until next turn if player move */
          if (isupd == 0)
            SADJCIV3;
          else
            sct[x][y].people += people_to_add;
        }
  } else {
    sct[x][y].people /= 2;
    for (i = x - 4; i <= x + 4; i++)
      for (j = y - 4; j <= y + 4; j++)
        if (ONMAP(i, j) &&
            (ntn[sct[i][j].owner].race == ntn[sct[x][y].owner].race))
          count++;
    if (count > 0) {

#ifdef CONQUER
      if (isupd == 0)
        mvaddstr(LINES - 2, 20, "PEOPLE FLEE SECTOR AND HALF DIE");
#endif /* CONQUER */

      for (i = x - 4; i <= x + 4; i++)
        for (j = y - 4; j <= y + 4; j++)
          if (ONMAP(i, j) &&
              (ntn[sct[i][j].owner].race == ntn[sct[x][y].owner].race)) {
            sct[i][j].people += sct[x][y].people / count;
            if (isupd == 0)
              SADJCIV2;
          }
    }

#ifdef CONQUER
        else if (isupd == 0)
      mvaddstr(LINES - 2, 20, "PEOPLE IN SECTOR DIE");
#endif /* CONQUER */
  }

  sct[x][y].people = slaves;
  if (isupd == 0)
    SADJCIV;
  sct[x][y].fortress = 0;
  /* SINFORT; */
  if (tofood(&sct[XREAL][YREAL], sct[XREAL][YREAL].owner) != 0) {
    DEVASTATE(x, y);
    if (isupd == 0)
      SADJDES2;
  }
  country = svcountry;
}

#ifdef ADMIN
int readmap(void);

/************************************************************************/
/*	READMAP()	- read a map in from map files 			*/
/*	returns TRUE for success, FALSE for fail			*/
/************************************************************************/
int readmap(void) {
  FILE *mapfile;
  char line[BIGLTH + 1];
  register int x, y;

  /* read in ele.map */
  strcpy(line, scenario);
  strcat(line, ".ele");
  if ((mapfile = fopen(line, "r")) == NULL) {
    fprintf(stderr, "error on read of %s file\n", line);
    return (TRUE);
  } else
    fprintf(stderr, "reading elevation map file from %s\n", line);

  y = 0;
  while (TRUE) {
    if (fgets(line, 128, mapfile) == NULL)
      break;
    for (x = 0; x < MAPX; x++)
      sct[x][y].altitude = line[x];
    y++;
    if (y >= MAPY)
      break;
  }
  fprintf(stderr, "done reading %d lines of %lu characters\n", y, strlen(line));

  /* read in veg.map */
  strcpy(line, scenario);
  strcat(line, ".veg");
  if ((mapfile = fopen(line, "r")) == NULL) {
    fprintf(stderr, "error on read of %s file\n", line);
    return (TRUE);
  } else
    fprintf(stderr, "reading vegetation map file from %s\n", line);
  y = 0;
  while (TRUE) {
    if (fgets(line, BIGLTH, mapfile) == NULL)
      break;
    for (x = 0; x < MAPX; x++)
      sct[x][y].vegetation = line[x];
    y++;
    if (y >= MAPY)
      break;
  }
  fprintf(stderr, "done reading %d lines of %lu characters\n", y, strlen(line));

  return (TRUE);
}

#endif /* ADMIN */

/*********************************************************************/
/* M2ALLOC() - two dimensional array allocator (because C is stupid) */
/*********************************************************************/
/* row dimension, column dimension, # bytes in items to be stored */
char **m2alloc(int nrows, int ncols, int entrysize)
{
  char **baseaddr;
  int j;

  entrysize *= ncols;
  baseaddr = (char **)malloc((unsigned)(nrows * (sizeof(char *) + entrysize)));

  if (baseaddr == (char **)NULL) {
    printf("OOPS - cant allocate %d by %d blocks of %d bytes\n", nrows, ncols,
           entrysize);
    abrt();
  }
  if (nrows > 0) {
    *baseaddr = (char *)(baseaddr + nrows);
    for (j = 1; j < nrows; j++)
      baseaddr[j] = baseaddr[j - 1] + entrysize;
  }
  return (baseaddr);
}

/* If the string entered is too long, then a truncated */
/* string is returned.  Length entered is returned.    */
int get_pass(char *str)
{
  char ch;
  int done = FALSE, count = 0;

  while (done == FALSE) {
    ch = getch();
    if (ch == '\b' || ch == '\177') {
      /* delete any entered characters */
      if (count > 0) {
        count--;
      }
    } else if (ch == '\025') {
      /* make sure that ^U works */
      count = 0;
    } else if (ch == '\n' || ch == '\r') {
      done = TRUE;
    } else if (ch != '\0') {
      /* add any other character to the string */
      if (count < PASSLTH) {
        /* don't try adding too many */
        str[count] = ch;
      }
      count++;
    }
  }
  /* truncate too long a password and end others properly */
  if (count > PASSLTH) {
    str[PASSLTH] = '\0';
  } else {
    str[count] = '\0';
  }
  return (count);
}
