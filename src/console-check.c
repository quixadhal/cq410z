/*
 * Project:	Console-Check
 * Author:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: console-check.c,v $
 * Revision 2.2  1991/09/26  06:30:23  cclub
 * Changed spy code to now use the -u option.
 * This may provide a slight increase in speed.
 *
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * *** empty log message ***
 *
 * Revision 1.5  1991/08/07  23:29:12  quixadhal
 * Reversed the return code of console_checking_active to match console_check.
 *
 * Revision 1.4  1991/08/07  23:20:52  quixadhal
 * Moved active checking code to a seperate routine for efficiency in calling.
 *
 * Revision 1.3  1991/08/07  22:58:41  quixadhal
 * Added provision for run-time enabling via tracking file.
 *
 * Revision 1.2  1991/08/06  02:16:07  quixadhal
 * Added ifdef's around main for testing purposes...
 *
 * Revision 1.1  1991/08/05  19:25:47  quixadhal
 * Initial revision
 *
 *
 */

#ifdef TEST_MAIN
static char *rcsid =
    "$Id: console-check.c,v 2.2 1991/09/26 06:30:23 cclub Exp cclub $";
    /* RCS auto ident string. */
#endif

/*
 * This program was created to allow for use by any type of application which
 * needs to determine the physical location of the user.  In particular, it
 * is often desirable to disallow users from playing games during a given
 * range of hours if they are sitting at a worstation console.  But there is
 * reason to prevent people from using them from a remote port...
 * Because of the inability to obtain information from the DEC-NET cluster of
 * machines on campus, it is nescessary to check each public-access sun
 * workstation on campus.  This is exceedingly slow to do via rsh and is also
 * quite a pain to have to shell-off so many processes.  Fortunately, there
 * is a program available which already does this, it is called "spy" and was
 * written by Chris Oliver.  All credit to him for doing the hard part!
 *
 * The routine console_check() may be used in your own programs.  It is
 * called with the uid number you wish to scan for, and returns a value of
 * 0 if that uid is logged into any workstation which spy knows about.
 * A 0 is also returned if there was a problem (such as not being able to
 * connect to spy, or a username not having a password entry (!?)).  This
 * ensures safety for security applications.  A 1 is returned if all machines
 * were checked and the user was not sitting at any of them.
 *
 * -Dread Quixadhal [cclub:19910805.0220EST]
 */

#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

int console_check(int id);
int console_checking_active(char *dirarea);

char console_check_name[21];

#ifdef TEST_MAIN
int main(void)
{
  if (!console_check(getuid()))
    printf("You are at console %s.\n", console_check_name);
  else
    puts("You are not logged into a workstation console.");
}
#endif

int console_check(int id)
{
  struct passwd *pw;
  char buffer[81];
  FILE *fp;
  int ok;

  ok = 0;
  if (!(pw = getpwuid(id)))
    return 0;
  setuid(geteuid());
  sprintf(buffer, "/usr/local/bin/spy -u %s publab", pw->pw_name);
#ifdef DEBUG
  fprintf(stderr, "Opening pipe to spy program...\n");
#endif
  if ((fp = popen(buffer, "r")))
    if (!feof(fp)) {
      ok = 1;
      while (!feof(fp))
        if (fgets(buffer, 80, fp))
          if ((buffer[44] == 'c') && (buffer[45] == 'o')) {
            ok = 0;
            strtok(buffer, " ");
            strcpy(console_check_name, strtok(NULL, " "));
          }
    }
  if (fp)
    pclose(fp);
#ifdef DEBUG
  fprintf(stderr, "Closed pipe to spy program\n");
#endif
  setuid(getuid());
  return ok;
}

int console_checking_active(char *dirarea)
{
  char buffer[256];
  FILE *fp;

  if (dirarea) {
    sprintf(buffer, "%s/tracking", dirarea);
    if (!(fp = fopen(buffer, "r")))
      return 0;
    if (fgetc(fp) == '1') {
      if (fp)
        fclose(fp);
      return 1;
    }
    if (fp)
      fclose(fp);
  }
  return 0;
}
