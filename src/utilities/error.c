/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: error.c,v $
 * Revision 1.2  1991/08/06  00:37:59  quixadhal
 * Added RCS header for logging of changes.
 *
 *
 */

#include <stdio.h>

int errno;
char *sys_errlist[];

char *myname="Someone";

/*VARARGS1*/
void
ioerror(str,arg1,arg2,arg3)

char *str;
int arg1, arg2, arg3;

{
fprintf(stderr,"%s: ",myname);
fprintf(stderr,str,arg1,arg2,arg3);
fprintf(stderr," - %s\n",sys_errlist[errno]);
exit(1);
}

/*VARARGS1*/
void
error(str,arg1,arg2,arg3)

char *str;
int arg1, arg2, arg3;

{
fprintf(stderr,"%s: ",myname);
fprintf(stderr,str,arg1,arg2,arg3);
fputc('\n',stderr);
exit(1);
}

