/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: spew.c,v $
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.2  1991/08/06  00:33:02  quixadhal
 * Added RCS header information for logging.
 *
 *
 */

/*
 * SPEW.C
 */

#ifndef lint
static char *cpr[] = {
  "  Copyright 1987 Greg Smith",
  "  Permission is granted to freely use and distribute this software",
  "provided this notice is left attached and no monetary gain is made."
};

#endif

/* This file has been adapted from the "spew" program by Paul Davison,
   pd@cs.qmc.ac.uk, on 1st March 1989. It is part of the conquer game
   and is subject to the same conditions of use as that game. 	*/

/* This in no way supersceedes the copyright notice noted above */

#include "header.h"
#include "data.h"

#ifdef SPEW
#include <stdio.h>
#include <string.h> /* strlen(), strcmp() */
#include <ctype.h>

char *my_alloc();
char *save();

#define index(s, c) strchr((s),(c))
#define rindex(s, c) strrchr((s),(c))

/*--------------- system configuration ------------------*/

/* define some parameters */

#define MAXCLASS 300 /* max # of classes */
#define MAXLINE 256  /* max size of input line */
#define MAXDEF 1000  /* max # bytes in a definition */

/* Define the default rulesfile */

#ifndef DEFFILE
#define DEFFILE "rules"
#endif

/* Define the random number generator */

extern long getpid();

/* ROLL(n) returns integer 0..n-1 */
#define ROLL(n) ((((long) rand() & 0x7ffffff) >> 5) % (n))

/*---------------------------------------------------*/

FILE *InFile;

typedef struct def_struct {
  int cumul;               /* cumulative weights */
  char *string;            /* string which defines it */
  struct def_struct *next; /* link to next */
} defn;
defn *process();

/*
 * within a definition, names of subdefinitions are bracketed in BSLASH
 * and SLASH chars. The whole definition ends with a '\0'.
 * The SLASH character is always follwed by a variant tag - default is ' '.
 */
#define BSLASH '\\'
#define SLASH '/'
#define VBAR '|'

typedef struct {
  int weight; /* total weight of definitions in class */
  defn *list; /* list of them */
  char *name; /* name of this class */
  char *tags; /* pointer to list of tags */
} classrec;

classrec *Classptr;   /* pointer to array of class records */
char *NullTags = " "; /* default tags ( shared ) */
int Classes;          /* number of them */
int HowMany = 1;
int CompIn = FALSE; /* is input file in compressed format? */
int CompMain;       /* class # of MAIN class when compressed */

char InLine[MAXLINE];

void init(void);
void readtext(void);
void display(char *s, int deftag, FILE *fp);
classrec *lookup(char *str);
int namecomp(register char *a, register char *b);
void readline(void);
int clcomp(const void *a, const void *b);
char *save(char *str);
void setup(register classrec *cp);
defn *process(void);
char *my_alloc(unsigned n);
void compdef(register defn *dp);
void readcclass(register classrec *cp);
void instring(register char *where, register int how_many);
void badfile(void);

void makemess(int n, FILE *fp)
{
  char fname[BIGLTH];
  char main_class[20];
  int i;

  HowMany = n;

  /* read in only the rules file in defaultdir */
  sprintf(fname, "%s/%s", RONLY_DIR, DEFFILE);
  InFile = fopen(fname, "r");
  if (InFile == NULL) {
    fprintf(stderr, "Can\'t open: %s\n", fname);
    exit(1);
  }
  init();
  if (CompIn)
    sprintf(main_class, "%d/ ", CompMain);
  else
    strcpy(main_class, "MAIN/ ");
  for (i = 0; i < HowMany; ++i)
    display(main_class, ' ', fp);
}

void init(void) {
  readtext();
}

void readtext(void)
{
  register classrec *cp;
  register defn *dp;
  defn **update;

  Classptr = (classrec *)my_alloc((unsigned)(MAXCLASS * sizeof(classrec)));
  Classes = 0;

  cp = Classptr;
  readline();
  if (InLine[0] != '%') {
    fprintf(stderr, "Class definition expected at: %s\n", InLine);
    exit(1);
  }
  while (InLine[1] != '%') {
    if (Classes == MAXCLASS) {
      fprintf(stderr, "Too many classes -- max = %d\n", MAXCLASS);
      exit(1);
    }
    setup(cp); /* set up the class struct */
    readline();
    if (InLine[0] == '%') {
      fprintf(stderr, "Expected class instance at: %s\n", InLine);
      exit(1);
    }
    update = &(cp->list); /* update pointer */
    do {
      dp = process();
      *update = dp;
      cp->weight += dp->cumul; /* add new stuff */
      dp->cumul = cp->weight;  /* set breakpoint */
      update = &(dp->next);
    } while (readline(), InLine[0] != '%');
    ++Classes; /* count them */
    ++cp;
    *update = NULL;
  }
  qsort((char *)Classptr, Classes, sizeof(classrec), clcomp);
}

/*
 * display is given a class name ( delimited by SLASH, not '\0' ),
 * and will (1) find its class descriptor, by calling lookup
 * (2) pick a definition  (3) output that definition, and
 * recursively display any definitions in it, and convert any escapes.
 * The variant tag after the SLASH is used to pick out the appropriate
 * variants. If that variant tag is '&', the tag 'deftag' is used, which
 * is the active variant of the containing activation.
 */
void display(char *s, int deftag, FILE *fp)
{
  register classrec *cp;
  register defn *dp;
  register char *p;
  classrec *lookup();
  int i, variant, incurly;
  register int c, writing;

  if (CompIn) {              /* input is compressed */
    cp = &Classptr[atoi(s)]; /* explicit class # */
  } else {
    cp = lookup(s);
    if (cp == NULL) { /* none found */
      fprintf(fp, "???");
      while (*s != SLASH)
        putc(*s++, fp);
      fprintf(fp, "???");
      return;
    }
  }
  c = index(s, SLASH)[1]; /* get variant tag */
  if (c != '&')
    deftag = c;                /* use given tag */
  p = index(cp->tags, deftag); /* look it up */
  if (p == NULL) {
    variant = 0;
    fprintf(fp, "?\?/%c??", deftag); /* \? is replaced with ? */
    deftag = ' '; /* for passing as deftag */
  } else
    variant = p - cp->tags;

  i = ROLL(cp->weight);
  dp = cp->list;
  while (dp->cumul <= i) { /* pick one based on cumul. weights */
    dp = dp->next;
  }

  incurly = 0;    /* not in curlies */
  writing = 1;    /* writing */
  p = dp->string; /* this is the string */
  for (;;)
    switch (c = *p++) {
      case '\0':
        return;
      case BSLASH:
        if ((c = *p++) == '\0')
          return; /* ?? */
        else if (c == '!') {
          if (writing)
            putc('\n', fp);      /* \! = newline */
        } else if (isalnum(c)) { /* reference */
          if (writing)
            display(p - 1, deftag, fp); /* recurse */
          while (*p != SLASH)
            ++p;
          p += 2; /* skip variant tag */
        } else {
          if (writing)
            putc(c, fp);
        }
        break;
      case '{':
        if (!incurly) {
          incurly = 1;
          writing = (variant == 0);
        } else {
          if (writing)
            putc('{', fp);
        }
        break;
      case VBAR:
        if (incurly) {
          writing = (variant == incurly++);
        } else {
          putc(VBAR, fp);
        }
        break;
      case '}':
        if (incurly) {
          writing = 1;
          incurly = 0;
        } else
          putc('}', fp);
        break;
      default:
        if (writing)
          putc(c, fp);
    }
}

classrec *lookup(char *str) /* delimited by SLASH, not '\0' */
{
  int first, last, try, comp;
  int namecomp();

  first = 0;
  last = Classes - 1;
  while (first <= last) {
    try = (first + last) >> 1;
    comp = namecomp(str, Classptr[try].name);
    if (comp == 0)
      return &Classptr[try];
    if (comp > 0)
      first = try + 1;
    else
      last = try - 1;
  }
  return NULL;
}

int namecomp(register char *a, register char *b) /* 'a' is delim. by SLASH, 'b' by NULL */
{
  register int ac;

  for (;;) {
    ac = *a++;
    if (ac == SLASH)
      ac = '\0';
    if (ac < *b)
      return -1;
    if (ac > *b++)
      return 1;
    if (ac == '\0')
      return 0;
  }
}

void readline(void)
{
  register char *p;

  do {
    if (fgets(InLine, MAXLINE, InFile) == NULL) {
      InLine[0] = InLine[1] = '%';
      InLine[2] = '\0'; /* create EOF */
    } else if ((p = rindex(InLine, '\n')) != NULL)
      *p = '\0';
    p = InLine;
    while ((p = index(p, BSLASH)) != NULL) {
      if (p[1] == '*') {
        *p = 0; /* kill comment */
        break;
      } else
        ++p;
    }
  } while (InLine[0] == '\0');
}

int clcomp(register const void *a, register const void *b)
{
  if (a == b || !a || !b)
    return 0;
  return strcmp(((classrec*)a)->name, ((classrec*)b)->name);
}

char *save(char *str)
{
  register char *p;

  p = (char *)my_alloc((unsigned)((strlen(str) + 1) * sizeof(char)));
  return strcpy(p, str);
}

/*
 * setup a class record. The 'class' line is in InLine.
 */
void setup(register classrec *cp)
{
  char temp[100];
  register char *p, *p2;

  p = &InLine[1]; /* point after the % */
  while (*p == ' ')
    ++p;
  if (!isalnum(*p))
    goto baddec;
  p2 = temp;
  do
    *p2++ = *p++;
  while (isalnum(*p));
  *p2 = '\0';
  cp->weight = 0; /* save the name of it */
  cp->name = save(temp);
  cp->list = NULL;
  cp->tags = NullTags; /* by default */
  for (;;)
    switch (*p++) {
      case '\0':
        return; /* all done; */
      case ' ':
        break;  /* allowed those */
      case '{': /* tags list */
        if (cp->tags != NullTags)
          goto baddec; /* already */
        p2 = temp;
        *p2++ = ' '; /* provide null tag */
        while (*p != '}') {
          if (!isalnum(*p))
            goto baddec;
          *p2++ = *p++;
        }
        ++p; /* junk rh brace */
        *p2 = 0;
        cp->tags = save(temp);
        break;
      default:
        goto baddec;
    }
baddec:
  fprintf(stderr, "Bad class header: %s\n", InLine);
  exit(1);
}

/*
 * create a 'processed' version of the InLine, and return a pointer to
 * the definition. The 'cumul' field is temporarily used to hold the
 * assigned weight of the line.
 */
defn *process(void) {
  static char stuff[MAXDEF];
  register char *p, *pout;
  register defn *dp;
  register int c;

  dp = (defn *)my_alloc((unsigned) sizeof(defn));

  p = InLine;
  pout = stuff;
  if (*p == '(') { /* get a weight */
    while (*++p == ' ')
      ; /* scan */
    if (!isdigit(*p))
      goto badweight;
    c = *p - '0';
    while (isdigit(*++p))
      c = c * 10 + (*p - '0');
    while (*p == ' ')
      ++p;
    if (*p != ')')
      goto badweight;
    ++p;
    dp->cumul = c;
  } else {
    dp->cumul = 1; /* default weight */
  }
  while ((c = *p++) != '\0')
    switch (c) {
      case BSLASH:
        *pout++ = BSLASH;
        if (isalnum(*p)) { /* is a ref */
          do {
            *pout++ = *p++;
          } while (isalnum(*p));
          *pout++ = SLASH;   /* delimit */
          if (*p == SLASH) { /* get variant char */
            ++p;
            if (!isalnum(*p) && *p != ' ' && *p != '&') {
              *pout++ = ' ';
            } else
              *pout++ = *p++;
          } else
            *pout++ = ' ';
        } else {
          *pout++ = *p;
          if (*p != '\0') {
            ++p;
          } else {
            --pout;     /* delete spurious '\' */
            readline(); /* get new line */
            p = InLine; /* point to it */
          }
        }
        break;
      default:
        *pout++ = c;
    }
  *pout = '\0';
  dp->string = save(stuff);
  return dp;

badweight:
  fprintf(stderr, "Bad line weight: %s\n", InLine);
  exit(1);
  /* NOTREACHED */
}

char *my_alloc(unsigned n)
{
  register char *p;

  p = (char *)malloc(n);
  if (p == NULL) {
    fprintf(stderr, "Out Of Memory\n");
    exit(1);
  }
  return p;
}

void compdef(register defn *dp)
{
  register char *p;
  register int c;

  putw(dp->cumul, stdout); /* write its cumul weight */
  p = dp->string;
  while ((c = *p++) != '\0') {
    if (c == BSLASH) {
      if (!CompIn && isalnum(*p)) { /* a ref */
        classrec *cp;

        cp = lookup(p); /* find it */
        if (cp == NULL) {
          fprintf(stderr, "Undefined class: ");
          while (*p != SLASH)
            fputc(*p++, stderr);
          fputc('\n', stderr);
          exit(1);
        } else {
          printf("%c%ld", BSLASH, cp - Classptr);
          while (*p != SLASH)
            ++p;
        }
      } else { /* is escape seq */
        putchar(BSLASH);
        putchar(*p++);
      }
    } else {
      putchar(c);
    }
  }
  putchar(0);
}

void readcclass(register classrec *cp)
{
  register int n;
  register defn *dp;
  defn **dput;

  char store[MAXDEF]; /* for tags */

  cp->weight = getw(InFile);
  instring(store, MAXDEF);
  cp->tags = (store[0] == '\0') ? NullTags : save(store);
  n = getw(InFile);
  if (n <= 0)
    badfile();
  dput = &(cp->list); /* link on here */
  while (n--) {
    dp = (defn *)my_alloc((unsigned) sizeof(defn));
    *dput = dp;
    dp->cumul = getw(InFile);
    instring(store, MAXDEF);
    dp->string = save(store);
    dput = &(dp->next);
  }
  *dput = NULL; /* last one */
}

void instring(register char *where, register int how_many)
{
  register int c;

  do {
    c = getc(InFile);
    if (c == EOF)
      badfile();
    *where++ = c;
    if (c == '\0')
      return;
  } while (--how_many);
  badfile();
}

void badfile(void)
{
  fprintf(stderr, "Bad file format\n");
  exit(1);
}

#endif /* SPEW */
