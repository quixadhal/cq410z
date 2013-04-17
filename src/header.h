/*
 * Project:	Conquer 4.10z
 * Modifier:	Dread Quixadhal
 * 		quixadhal@sol.cs.wmich.edu
 * $State: Exp $
 * $Log: header.h,v $
 * Revision 2.2  1991/09/27  02:26:48  cclub
 * Added flag for NO_MAPS to disallow map printing.
 *
 * Revision 2.1  1991/09/20  16:54:46  cclub
 * y
 *
 * Revision 1.11  1991/08/09  21:57:26  quixadhal
 * Added hide-score flag.
 *
 * Revision 1.10  1991/08/09  20:18:39  quixadhal
 * Added flag for hiding unmet nations from people.
 *
 * Revision 1.9  1991/08/09  20:06:03  quixadhal
 * Added flag for restricted adds... off by default.
 *
 * Revision 1.8  1991/08/09  00:04:11  quixadhal
 * Added routines for relative coordinates.
 *
 * Revision 1.7  1991/08/08  01:05:37  quixadhal
 * Added news digest flag.
 *
 * Revision 1.6  1991/08/07  23:42:31  quixadhal
 * Added nomad/savage shutup define.
 *
 * Revision 1.5  1991/08/06  02:18:47  quixadhal
 * added defines for console-checking and time-of-day-checking
 * the console-check routine is in console-check.c
 * the time routine will be in tod.c
 *
 * Revision 1.4  1991/08/06  00:53:15  quixadhal
 * Changed game ownership to quixadhal for testing purposes.
 * Also dropped the tradegood percentage a bit...
 *
 * Revision 1.3  1991/08/06  00:33:02  quixadhal
 * Added RCS header information for logging.
 *
 *
 */

/* conquer : Copyright (c) 1988, 1989 by Ed Barlow.
   MODIFICATION OF THIS FILE IMPLIES THAT THE MODIFIER WILL ACCEPT
	A LIMITED USE COPYRIGHT AS FOLLOWS:


	1) This software is copyrighted and protected by law. The
		sole owner of this software, which hereafter is known as
		"conquer" is Edward M. Barlow, who hereby grants you a
		personal, non-exclusive right to use this software.
		All rights on this software are reserved.
	2) conquer may not be redistributed in any form. Any requests for
		new software shall, for now, be the perogative of the author.
	3) loss or damage caused by this software shall not be
		the responsibility of the author.
	4) Ed Barlow or Adam Bryant shall be notified of enhancements to this
		software via electronic mail and, if there is no response, via
		US mail to:

			Ed Barlow
			115 E 34ths St.
			NY, NY 10016

		My home phone is 212-679-1439. Use it sparingly and call
		before 11PM if it is important.  Note that I no longer have
		access to the arpanet and cant really support conquer like
		I used to.  If somebody knows of a cheap (free) way of getting
		a login somewhere where there is network access - HELP ME!!!!
		Since i am "volunteering" my time for this project, I am not
		about to spend millions of dollars to buy network access...
		I can get a modem for my PC if somebody will help me get
		something to call into. -ED-
	5) no attempt shall be made to make any money from this game or to
		use any portion of this code to make any money without the
		authors permission.
	6) no attempt shall be made to port this software to any form of
		personal computer without the permission of Ed Barlow.
	7) you agree to use your best efforts to see that any user
		of conquer complies with the terms and conditions stated above.
	8) The above copyright agreement will not be tampered with in any form.


   Special thanks to Brian Beuning, Adam Bryant, and any others who helped
	me by making enhancements and bug reports
*/

/* --- MODIFICATION IS REQUIRED OF THE FOLLOWING DEFINE STATEMENTS ---	*/
#define OWNER	"The Mad Hacker"/* administrators name			*/
#define LOGIN	"quix"		/* administrators login id. IMPORTANT!	*/
				/* only this UID may update.		*/
/* #define SYSV		/* uncomment this line on a UNIX SYSV machine	*/
#define BSD		/* uncomment this line on a BSD machine	 	*/
/* #define HPUX		/* uncomment for HP-UNIX			*/
/* #define XENIX	/* this plus SYSV for XENIX machines, untested  */
#define SYSMAIL		/* if your system supports mail			*/
			/* conquer will notify you about system mail	*/
#define COMPRESS "/usr/bin/gzip -3" /* compress utility   */
#define ZCAT     "/usr/bin/zcat"  /* uncompress utility */
#define ZEXT     "gz"				  /* compress extension */
#define FCMASK	0600

/* ------------------MODIFICATION IS OPTIONAL ON THE FOLLOWING----------*/
/* #define CHECK_TIME              /* should we check for valid play times  */
/* #define CHECK_CONSOLE           /* should we go and check user location  */
#define USE_COMPRESS		/* use the compression routines          */
#define SHUT_UP_DORKS           /* stops nomad/savage capture reports    */
#define DIGESTNEWS              /* keep section 5 of old news files      */
#define USE_RELATIVE_COORDS     /* keep players guessing for location    */
#define CHECK_UID		/* make sure you are who you are */
#define ONE_PER_UID		/* one nation per user (unless god helps) */
/*#define RESTRICT_ADDS      /* this will keep people from adding
                                   after turn number 5                   */
#define HIDE_UNMET              /* do not show scores or status for
				   nations which are yet unmet... This
				   also disables the -s option.          */
#define HIDE_SCORE              /* if defined, you can only see your
				   own score.                            */
#define NO_MAPS			/* this is due to the ingenuity of some
				   of the cheaters playing conquer!  It
				   seems that many are printing out maps
				   of their countries and then holding
				   them up to the light so that they
				   can see the relative positions of each
				   nation.... -p is disabled with this.  */
#define ENCODE_EXECS		/* cheating again..... sigh */
#define SPOOLDIR "/usr/spool/mail"	/* location of mail spool	 */
#define FILELOCK		 if your system supports BSD flock()	 */
			/* other file locking is not well implemented	 */
#define LOCKF	 in addition to FILELOCK if you have lockf     */
			/* this is needed on NFS remote file systems     */
/*
 * Unfortunatly, our system manager has a habit of screwing up the lockd
 * and rpc.lockd processes, so we have to contantly swap between the normal
 * version, and one with all system-calls to flock, lockf disabled.
 */

#define TIMELOG			/* if your system has the date command	 */

#define NTOTAL 248	/* max # of nations ( player + npc + monster )	 */
#define MAXPTS	90	/* points for players to buy stuff with at start */
#define MAXARM	248	/* maximum number of armies per nation		 */
#define MAXNAVY	32	/* maximum number of fleets per nation		 */
#define PDEPLETE 40	/* % of armies/sectors depleted without
			 * Capitol	 */
#define PFINDSCOUT 40	/* percentage chance for capturing scouts	 */

/* #define RUNSTOP	/* defined to stop update if players are in	 */
			/* game -- not recommended with automatic update */
#define TRADE		/* defined to allow commerce between nations	 */
#define	TRADEPCT 45	/* percent of sectors with exotic trade goods	 */
#define METALPCT 30	/* percent of tradegoods that are metals	 */
#define JEWELPCT 25	/* percent of tradegoods that are luxury
			 * items	 */
#define HIDELOC		/* defined if news is not to report sectors	*/
#define OGOD		/* defined if you wish to enhance god powers.   */
#define REMAKE		/* may make a world even if datafile exists.	*/
			/* this allows demi-gods the ability to remake  */
			/* their world.                                 */
#define NOSCORE		/* only show full scores to god while in game	*/
#define DERVDESG	/* allow DERVISH to redesignate in DESERT/ICE	 */
#define MONSTER	256	/* defined if pirates/savages/nomads/lzard
			 * exist. represents # of sectors of land
			 * that need to be in world per
			 * pirate/savage/nomad nation	 */
/* #define MORE_MONST	/* defined if destroyed monsters are replaced	 */
#define NPC	50	/* defined if NPC nations should exist. The
			 * numeric represents # of sectors of land
			 * that need to be in world per non-player
			 * character nation	 */
#define CHEAT		/* npcs will cheat to keep up - this is a
			 * very weak form of cheating.  I use good
			 * npc algorithms (i think... comments)		 */
#define NPC_COUNT_ARMIES   /* defined if NPC nations can always count armies
			    * This makes them to cheat by seeing even VOID and
			    * HIDDEN armies when counting enemy units.	*/
#define NPC_SEE_SECTORS    /* defined if NPC nations can always see sectors
			    * This allows them to cheat by being allowed to see
			    * all sector attributes of even VOID sectors. */
#define	NPC_SEE_CITIES		/* defined if NPC nations can always see
				 * cities This allows them to cheat by being
				 * able to see if a VOID sector is a
				 * city/town.  Simulates the players ability
				 * to tell cities via movement.	 */
#define STORMS		/* have storms strike fleets			 */
#define VULCANIZE	/* add in volcano eruptions....			 */
#define PVULCAN 30	/* % chance of eruption each round (see
			 * above)	 */
#define ORCTAKE 50000L		/* comment out if dont want orcs to takeover
				 * orc NPCs.  else is takeover price in
				 * jewels	 */
#define MOVECOST 5L	/* cost to do a move, get a screen...		 */
#define TAKEPOINTS 10	/* spell points for orc takeover		 */
#define PMOUNT 40	/* % of land that is mountains			 */
#define PSTORM 10	/* % chance that a storm will strike a fleet	 */
			/* unless it is in harbor			 */
/* #define CMOVE	/* ifdef NPC; defined for the computer to
			 * move for Player nations if they forget to
			 * move	 */
#define BEEP		/* defined if you wish terminal to beep		 */
#define HILIGHT		/* defined if terminals support inverse video	 */
#define RANEVENT 6	/* comment out if you dont want random events
			 * weather, tax revolts, and volcanoes all
			 * are	considered random events.		 */
#define PWEATHER 0	/* percent for weather disaster -
			 * unimplemented	 */
#define	PREVOLT	15	/* %/turn that a revolt acutally occurs		 */
			/* a turn is 1 season and 25% is a large value	 */
#define	SPEW		/* spew random messages from npcs 		 */

/* -BELOW THIS POINT ARE PARAMETERS YOU MIGHT OPTIONALLY WISH TO CHANGE-*/

/* making these numbers large takes more CPU time			*/
#define LANDSEE 2	/* how far you can see from your land		 */
#define NAVYSEE 1	/* how far navies can see			 */
#define ARMYSEE 2	/* how far armies can see			 */
#define PRTZONE 12	/* how far pirates roam from their basecamp	 */
#define MEETNTN 3	/* how close nations must be to adjust status	 */

/* Below taxation rates are in gold talons per unit of product produced	*/
#define TAXFOOD		5L
#define TAXMETAL	8L
#define TAXGOLD		8L
#define TAXOTHR		3L	/* per food point equivalent		 */
/* Town and City/Capitol tax rates based on # of people	*/
#define TAXCITY		100L
#define TAXTOWN		80L

#define SHIPMAINT	4000L	/* ship mainatinance cost		 */
#define TOMANYPEOPLE	6400L	/* too many people in sector - 1/2 repro and
				 * 1/2 production; not in cities/caps	 */
#define ABSMAXPEOPLE	64000L	/* absolute max people in any sector	 */
#define	MILLSIZE	250L	/* min number of people to work a mill	 */
#define TOMUCHMINED	64000L	/* units mined for 100% chance of metal	 */
 /* depletion actual chance is prorated	 */
#define DESFOOD		4	/* min food val to redesignate sector	 */
#define MAXNEWS		3	/* number of news files stored		 */
#define LONGTRIP	100	/* navy trip lth for 100% attrition	 */

/* min soldiers to take sector - either 75 or based on your civilians	*/
/* This is a modified version of the old take sector count.
 * The new version allows you to take any unowned sector with TKSCTMIN
 * men, regardless of how big you are.  It also works a percentage of
 * the defending population into the calculations when taking sectors
 * which are owned.
 */
#define NEWTAKESCT	1	/* define this if you want the modified
				 * version of the sector taking function
				 */
#define TKSCTMAX	1000	/* maximum based on YOUR population     */
#define TKSCTMIN	50	/* minimum army size needed             */
#define TKSCTCIV	400	/* how heavy to weigh YOUR population   */
#define TKSCTPOP	4	/* how heavy to weigh target population */

#define TAKESECTOR min(TKSCTMAX,max(TKSCTMIN,(ntn[country].tciv/TKSCTCIV)))
#define TAKEPOPUL(x)   (TAKESECTOR+((x)/TKSCTPOP))
#define TAKEUNOWN(x,y) (((x)>=TKSCTMIN)&&((y)==0))

#define MAXLOSS		60	/* maximum % of men lost in 1:1 battle	 */
#define	FINDPERCENT	1	/* percent to find gold/metal in sector	 */
#define DESCOST		2000L	/* cost to redesignate and the metal cost for
				 * cities				 */
#define FORTCOST	1000L	/* cost to build a fort point		 */
#define STOCKCOST	3000L	/* cost to build a stockade		 */
#define REBUILDCOST	4000L	/* cost to remove a ruin		 */
#define WARSHPCOST	20000L	/* cost to build one light warship	 */
#define MERSHPCOST	25000L	/* cost to build one light merchant	 */
#define GALSHPCOST	25000L	/* cost to build one light galley	 */
#define N_CITYCOST	4	/* move lost in (un)loading in cities	 */
#define SHIPCREW	100	/* full strength crew on a ship		 */
#define SHIPHOLD	100L	/* storage space of a ship unit		 */
#define CITYLIMIT	8L	/* % of npc pop in sctr before => city	 */
#define CITYPERCENT	20L	/* % of npc pop able to be in cities	 */
/* note that militia are not considered military below	*/
#define MILRATIO	8L	/* ratio civ:mil for NPCs		 */
#define MILINCAP	8L	/* ratio (mil in cap):mil for NPCs	 */
#define	MILINCITY	10L	/* militia=people/MILINCITY in city/cap */
#define NPCTOOFAR	48	/* npcs should not go this far from cap	 */
#define BRIBE		50000L	/* amount of gold/1000 men to bribe	 */
#define METALORE	6L	/* metal/soldier needed for +1% weapons	 */
/* strength value for fortifications	*/
#define DEF_BASE	10	/* base defense value 2 * in city/caps	 */
#define FORTSTR		6	/* percent per fortress point in forts	 */
#define TOWNSTR		4	/* percent per fortress point in towns	 */
#define CITYSTR		8	/* percent per fortress point in city	 */
#define	LATESTART	2	/* new player gets 1 point/LATESTART turns,
				 * when they start late into the game	 */

/*	starting values for mercenaries	*/
#define ST_MMEN	 (NTOTAL*500)	/* a nation may draft ST_MMEN/NTOTAL	 */
 /* mercenaries per turn. Added to when	 */
 /* armies are disbanded.	 */
#define ST_MATT		60	/* mercenary attack bonus	 */
#define ST_MDEF		30	/* mercenary defense bonus	 */

#define VERSION "Version 4"	/* version number of the game	 */


