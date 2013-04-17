/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: psmap.h,v $
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.2  1991/08/06  00:33:02  quixadhal
 * Added RCS header information for logging.
 *
 *
 */

/*
 *   Header file for conqps   by MaF
 *   Feel free to modify
 *   Comments should be sent to d8forma@dtek.chalmers.se
 *
 */

/* Page size hassle */

#ifdef OTHER
#define DEFAULTPAGE 0
#endif

#ifdef A4
#define DEFAULTPAGE 1
#endif

#ifdef LETTER
#define DEFAULTPAGE 2
#endif

/* Printer dependant entries */
#define PAGEWIDTH_A4       540
#define PAGEHEIGHT_A4      820
#define XOFFSET_A4         30
#define YOFFSET_A4         10

#define PAGEWIDTH_LETTER       575
#define PAGEHEIGHT_LETTER      760
#define XOFFSET_LETTER         15
#define YOFFSET_LETTER         10

#define PAGEWIDTH_OTHER       450
#define PAGEHEIGHT_OTHER      700
#define XOFFSET_OTHER         40
#define YOFFSET_OTHER         30

/* Cosmetics */
#define XMARGINS        30
#define YMARGINS        50

/* Version specific defines */
#define VERSION         "1.0"
#define USAGE           "Usage: %s [cghlnvu] [p pagesize] [f font] [o x,y] [s size]\n\t[W n] [L n] [X n] [Y n] [t title] [infile [outfile]]\n"
#define MATCHSTRING     "Conquer Version"

/* Mathematical definitions */
#define TRUE            1
#define FALSE           0

/* Types of maps possible to be printed */
#define SIMPLE          0
#define ALTITUDES       1
#define DESIGNATIONS    2
#define NATIONS         3
#define VEGETATIONS     4
#define FORCED          5
