/*
 * Project:	Console-Check
 * Author:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: tod.c,v $
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.2  1991/08/06  04:25:48  quixadhal
 * fixed silly bug with fopen call.... oops!
 *
 * Revision 1.1  1991/08/06  04:04:16  quixadhal
 * Initial revision
 *
 *
 */

/*
 * This program simply matches the current time against times stored in
 * a text file called times.  This file is expected in the format:
 * Day oh ch uh
 * where Day is the day of the week (Sun-Sat) and these must all be present.
 * oh is open-hour, ch is close-hour, and uh is update-hour.  These should
 * be in 24 hour format.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

int time_check(char *dirarea, int owneruid);

char time_check_value[27];

int time_check(char *dirarea, int owneruid)
{
  register int i;
  char buffer[256];
  FILE *fp, *mfp;
  int ok;
  time_t mytime;
  int hour, day;
  static char *day_names[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
                               NULL };
  int time_values[7][3];

  ok = 0;
  time(&mytime);
  strcpy(time_check_value, ctime(&mytime));
  time_check_value[3] = '\0';
  for (i = 0; i < 7; i++)
    if (!strcmp(day_names[i], time_check_value)) {
      ok = 1;
      break;
    }
  if (!ok)
    return 0;
  day = i;
  time_check_value[13] = '\0';
  hour = atoi(&time_check_value[11]);
  if (!dirarea)
    sprintf(buffer, "times");
  else
    sprintf(buffer, "%s/times", dirarea);
  if (!(fp = fopen(buffer, "r")))
    return 0;
  for (i = 0; i < 7; i++)
    if (feof(fp)) {
      if (fp)
        fclose(fp);
      return 0;
    } else
      fscanf(fp, "%d%d%d", &time_values[i][0], &time_values[i][1],
             &time_values[i][2]);
  if (hour == time_values[day][2])
    return 0;
  if (!dirarea)
    sprintf(buffer, "members");
  else
    sprintf(buffer, "%s/members", dirarea);
  if ((mfp = fopen(buffer, "r"))) {
    char nameline[256];
#ifdef DEBUG
    fprintf(stderr, "opened member file...\n");
#endif
    while (!feof(mfp)) {
      if (!feof(mfp)) {
        memset(nameline, 0, sizeof nameline);
        fscanf(mfp, "%s", nameline);
        if (strcmp(nameline, "")) {
          struct passwd *theiruid = getpwnam(nameline);
#ifdef DEBUG
          fprintf(stderr, "We found a name... is it yours?\n");
#endif
          if (theiruid) {
            if (owneruid == theiruid->pw_uid) {
              fclose(mfp);
              return 1;
            }
          }
#ifdef DEBUG
          fprintf(stderr, "Nope... next name...\n");
#endif
        }
      }
    }
    if (mfp)
      fclose(mfp);
  }
#ifdef DEBUG
  fprintf(stderr, "Done with member file...\n");
#endif

  printf("Awfully sorry %s, you are NOT a cclub member so you have \
to "
         "sit\nand watch this drivel scroll.  It's only $5 bucks you know!  "
         "Then \
you\nwouldn't have to look at this stupid message every time "
         "you play...\n\n",
         (getpwuid(getuid()))->pw_name);
  fflush(stdout);
  sleep(20);
  if (time_values[day][0] < 0)
    return 1;
  if (time_values[day][0] < time_values[day][1]) {
    if (hour < time_values[day][0])
      return 0;
    if (hour >= time_values[day][1])
      return 0;
  } else if (hour < time_values[day][0])
    if (hour >= time_values[day][1])
      return 0;
  return 1;
}
