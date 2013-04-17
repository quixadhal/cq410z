-- PostgresSQL (7.4) schema for Conquer v4.11 database
-- Version 0.1
-- Dread Quixadhal
--

CREATE TABLE users (
	user_id				SERIAL NOT NULL PRIMARY KEY,
	system_userid		INTEGER,
	name				TEXT NOT NULL UNIQUE,
	passwd				TEXT NOT NULL,
	active				BOOLEAN,
	created				TIMESTAMP WITH TIMEZONE DEFAULT now(),
	last_login			TIMESTAMP WITH TIMEZONE,
	wins				INTEGER,
	played				INTEGER,
	score				INTEGER
);

COMMENT ON TABLE		users								IS 'The folks playing our game!';
COMMENT ON COLUMN		users.user_id						IS 'A unique ID for each user';
COMMENT ON COLUMN		users.system_userid					IS 'The OS uid, if applicable';
COMMENT ON COLUMN		users.name							IS 'The username one logs in under';
COMMENT ON COLUMN		users.passwd						IS 'Their password (MD5)';
COMMENT ON COLUMN		users.active						IS 'Is this account enabled?';
COMMENT ON COLUMN		users.created						IS 'When the account was created';
COMMENT ON COLUMN		users.last_login					IS 'The last time the user played a game';
COMMENT ON COLUMN		users.wins							IS 'The number of games a user won';
COMMENT ON COLUMN		users.played						IS 'The number of games a user participated in';
COMMENT ON COLUMN		users.score							IS 'The total score of all their games';

CREATE TABLE world (
	world_id			SERIAL NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	description			TEXT,
	god					INTEGER NOT NULL					REFERENCES(users.user_id),
	demigod				INTEGER								REFERENCES(users.user_id),

	mapx				INTEGER NOT NULL,
	mapy				INTEGER NOT NULL,
	nations				INTEGER NOT NULL,
	turn				INTEGER NOT NULL,
	merc_military		INTEGER NOT NULL,
	merc_aplus			INTEGER NOT NULL,
	merc_dplus			INTEGER NOT NULL,
	jewels				INTEGER NOT NULL,
	gold				INTEGER NOT NULL,
	food				INTEGER NOT NULL,
	metal				INTEGER NOT NULL,
	civilians			INTEGER NOT NULL,
	military			INTEGER NOT NULL,
	sectors				INTEGER NOT NULL,
	score				INTEGER NOT NULL
);

COMMENT ON TABLE		world								IS 'Overall world data';
COMMENT ON COLUMN		world.world_id						IS 'Which world are we talking about?';
COMMENT ON COLUMN		world.name							IS 'What do we call it?';
COMMENT ON COLUMN		world.description					IS 'What is it like?';
                                                                
COMMENT ON COLUMN		world.mapx							IS 'Size of world, east to west';
COMMENT ON COLUMN		world.mapy							IS 'Size of world, north to south';
COMMENT ON COLUMN		world.nations						IS 'number of actual nations';
COMMENT ON COLUMN		world.turn							IS 'game turn';
COMMENT ON COLUMN		world.merc_military					IS 'number of mercs available';
COMMENT ON COLUMN		world.merc_aplus					IS 'mercenary attack bonus';
COMMENT ON COLUMN		world.merc_dplus					IS 'mercenary defense bonus';
COMMENT ON COLUMN		world.jewels						IS 'jewels in world';
COMMENT ON COLUMN		world.gold							IS 'gold talons in world';
COMMENT ON COLUMN		world.food							IS 'food in world';
COMMENT ON COLUMN		world.metal							IS 'metal in world';
COMMENT ON COLUMN		world.civilians						IS 'world population';
COMMENT ON COLUMN		world.military						IS 'world military';
COMMENT ON COLUMN		world.sectors						IS 'owned sectors in world';
COMMENT ON COLUMN		world.score							IS 'world score total';

-- For the current code, world_id, x and y will give us a unique sector.
-- For the future, we may need z and the turn number as well.

CREATE TABLE settings (
	world_id			INTEGER								REFERENCES(world.world_id),
	name				TEXT NOT NULL,
	value				TEXT
);

COMMENT ON TABLE		settings							IS 'All your variables are belong to us!';
COMMENT ON COLUMN		settings.world_id					IS 'Is this setting global, or world-specific?';
COMMENT ON COLUMN		settings.name						IS 'What do we call it?';
COMMENT ON COLUMN		settings.value						IS 'What is it?';

COPY settings (name, value) FROM stdin;
\.

CREATE INDEX			ix_settings_world_id				ON settings ( world_id );
CREATE UNIQUE INDEX		ix_settings_world_id_name			ON settings ( world_id, name );
COMMENT ON INDEX		ix_settings_world_id				IS 'Fast lookups by world might be nice';
COMMENT ON INDEX		ix_settings_world_id_name			IS 'Retrieving a specific seting';


CREATE TABLE altitudes (
	altitude_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		altitudes							IS 'Height levels for the map';
COMMENT ON COLUMN		altitudes.altitude_id				IS 'Unique ID for each altitude';
COMMENT ON COLUMN		altitudes.name						IS 'Display name for the altitude';
COMMENT ON COLUMN		altitudes.symbol					IS 'Display symbol (character)';
COMMENT ON COLUMN		altitudes.color						IS 'What color do we use? (curses)';
COMMENT ON COLUMN		altitudes.bold						IS 'Do we turn bold on for this? (curses)';

COPY altitudes (altitude_id, name, symbol, color, bold) FROM stdin;
0	WATER	~	4	0
1	PEAK	#	1	1
2	MOUNTAIN	^	7	1
3	HILL	%	3	0
4	FLAT	-	2	0
\.

CREATE TABLE vegetations (
	vegetation_id		INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN,
	food				INTEGER
);

COMMENT ON TABLE		vegetations							IS 'Climate levels for the map';
COMMENT ON COLUMN		vegetations.vegetation_id			IS 'Unique ID for each foliage type';
COMMENT ON COLUMN		vegetations.name					IS 'Display name for the foliage type';
COMMENT ON COLUMN		vegetations.symbol					IS 'Display symbol (character)';
COMMENT ON COLUMN		vegetations.color					IS 'What color do we use? (curses)';
COMMENT ON COLUMN		vegetations.bold					IS 'Do we turn bold on for this? (curses)';
COMMENT ON COLUMN		vegetations.food					IS 'How much food can be grown here?';

COPY vegetations (vegetation_id, name, symbol, color, bold, food) FROM stdin;
0	VOLCANO	v	1	1	0
1	DESERT	d	1	0	0
2	TUNDRA	t	6	0	0
3	BARREN	b	3	0	4
4	LT VEG	l	3	1	6
5	GOOD	g	2	1	9
6	WOOD	w	2	1	7
7	FOREST	f	2	0	4
8	JUNGLE	j	2	0	0
9	SWAMP	s	4	1	0
10	ICE	i	7	1	0
11	NONE	~	4	0	0
\.

CREATE TABLE movecost_altitudes (
	altitude_id			INTEGER NOT NULL					REFERENCES(altitudes.altitude_id),
	race_id				INTEGER								REFERENCES(races.race_id),
	cost				INTEGER
);

COMMENT ON TABLE		movecost_altitudes					IS 'What does it cost to move over terrain';
COMMENT ON COLUMN		movecost_altitudes.altitude_id		IS 'Maps an altitude';
COMMENT ON COLUMN		movecost_altitudes.race_id			IS 'Maps a race';
COMMENT ON COLUMN		movecost_altitudes.cost				IS 'Move point deduction';

-- If there is no entry for the altitude/race combo, consider it infinite (IE: not allowed)

COPY movecost_altitudes (altitude_id, race_id, cost) FROM stdin;
2	5	5
2	1	2
2	2	6
2	3	3
3	5	2
3	1	2
3	2	3
3	3	1
4	5	1
4	1	2
4	2	1
4	3	1
\.

CREATE TABLE movecost_vegetations (
	vegetation_id		INTEGER NOT NULL					REFERENCES(vegetations.vegetation_id),
	race_id				INTEGER								REFERENCES(races.race_id),
	cost				INTEGER
);

COMMENT ON TABLE		movecost_vegetations				IS 'What does it cost to move over terrain';
COMMENT ON COLUMN		movecost_vegetations.vegetation_id	IS 'Maps a vegetation';
COMMENT ON COLUMN		movecost_vegetations.race_id		IS 'Maps a race';
COMMENT ON COLUMN		movecost_vegetations.cost			IS 'Move point deduction';

-- If there is no entry for the altitude/race combo, consider it infinite (IE: not allowed)

COPY movecost_vegetations (vegetation_id, race_id, cost) FROM stdin;
0	5	6
0	1	4
0	2	8
0	3	4
1	5	3
1	1	3
1	2	6
1	3	7
2	5	2
2	1	1
2	2	2
2	3	1
3	5	1
3	1	0
3	2	2
3	3	0
4	5	0
4	1	0
4	2	1
4	3	0
5	5	0
5	1	0
5	2	0
5	3	0
6	5	0
6	1	2
6	2	0
6	3	1
7	5	1
7	1	2
7	2	0
7	3	3
8	5	3
8	1	5
8	2	0
8	3	5
9	5	3
9	1	2
9	2	2
9	3	7
10	5	2
10	1	7
10	2	7
10	3	7
\.

CREATE TABLE foods (
	food_id				INTEGER NOT NULL PRIMARY KEY,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		foods								IS 'Mostly color information for food values';
COMMENT ON COLUMN		foods.food_id						IS 'Food level';
COMMENT ON COLUMN		foods.symbol						IS 'Display symbol (character)';
COMMENT ON COLUMN		foods.color							IS 'What color do we use? (curses)';
COMMENT ON COLUMN		foods.bold							IS 'Do we turn bold on for this? (curses)';

COPY foods (food_id, symbol, color, bold) FROM stdin;
0	0	4	0
1	1	1	0
2	2	1	1
3	3	5	0
4	4	3	0
5	5	3	1
6	6	6	0
7	7	6	1
8	8	2	0
9	9	2	1
10	+	7	1
11	/	0	0
\.

CREATE TABLE movecosts (
	movecost_id			INTEGER NOT NULL PRIMARY KEY,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		movecosts							IS 'Mostly color information for move values';
COMMENT ON COLUMN		movecosts.movecost_id				IS 'movement level';
COMMENT ON COLUMN		movecosts.symbol					IS 'Display symbol (character)';
COMMENT ON COLUMN		movecosts.color						IS 'What color do we use? (curses)';
COMMENT ON COLUMN		movecosts.bold						IS 'Do we turn bold on for this? (curses)';

COPY movecosts (movecost_id, symbol, color, bold) FROM stdin;
0	0	0	0
1	1	7	1
2	2	2	1
3	3	2	0
4	4	3	1
5	5	3	0
6	6	5	1
7	7	5	0
8	8	1	1
9	9	1	1
10	+	1	0
11	~	4	0
12	X	1	0
13	/	0	0
\.

CREATE TABLE defenses (
	defense_id			INTEGER NOT NULL PRIMARY KEY,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		defenses							IS 'Mostly color information for defense values';
COMMENT ON COLUMN		defenses.defense_id					IS 'Defense level';
COMMENT ON COLUMN		defenses.symbol						IS 'Display symbol (character)';
COMMENT ON COLUMN		defenses.color						IS 'What color do we use? (curses)';
COMMENT ON COLUMN		defenses.bold						IS 'Do we turn bold on for this? (curses)';

COPY defenses (defense_id, symbol, color, bold) FROM stdin;
0	0	1	0
1	1	1	1
2	2	1	1
3	3	5	0
4	4	5	1
5	5	3	0
6	6	3	1
7	7	3	1
8	8	2	0
9	9	2	1
10	+	7	1
11	~	4	0
12	X	1	0
13	/	0	0
\.

CREATE TABLE populations (
	population_id		INTEGER NOT NULL PRIMARY KEY,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		populations							IS 'Mostly color information for population values';
COMMENT ON COLUMN		populations.population_id			IS 'Population level';
COMMENT ON COLUMN		populations.symbol					IS 'Display symbol (character)';
COMMENT ON COLUMN		populations.color					IS 'What color do we use? (curses)';
COMMENT ON COLUMN		populations.bold					IS 'Do we turn bold on for this? (curses)';

COPY populations (population_id, symbol, color, bold) FROM stdin;
0	0	1	0
1	1	1	1
2	2	1	1
3	3	5	0
4	4	5	1
5	5	3	0
6	6	3	1
7	7	3	1
8	8	6	0
9	9	6	1
10	I	2	0
11	V	2	1
12	X	7	1
13	~	4	0
14	/	0	0
\.

CREATE TABLE metals (
	metal_id			INTEGER NOT NULL PRIMARY KEY,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		metals								IS 'Mostly color information for metal values';
COMMENT ON COLUMN		metals.food_id						IS 'Metal level';
COMMENT ON COLUMN		metals.symbol						IS 'Display symbol (character)';
COMMENT ON COLUMN		metals.color						IS 'What color do we use? (curses)';
COMMENT ON COLUMN		metals.bold							IS 'Do we turn bold on for this? (curses)';

COPY metals (metal_id, symbol, color, bold) FROM stdin;
0	0	1	0
1	1	1	1
2	2	5	0
3	3	5	1
4	4	3	0
5	5	3	1
6	6	6	0
7	7	6	1
8	8	2	0
9	9	2	1
10	+	7	1
11	~	4	0
12	/	0	0
\.

CREATE TABLE designations (
	designation_id		INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		designations						IS 'All the ways a sector can be used';
COMMENT ON COLUMN		designations.designation_id			IS 'Unique ID for each designation';
COMMENT ON COLUMN		designations.name					IS 'Display name for the designation';
COMMENT ON COLUMN		designations.symbol					IS 'Display symbol (character)';
COMMENT ON COLUMN		designations.color					IS 'What color do we use? (curses)';
COMMENT ON COLUMN		designations.bold					IS 'Do we turn bold on for this? (curses)';

COPY designations (designation_id, name, symbol, color, bold) FROM stdin;
0	TOWN	t	1	0
1	CITY	c	1	1
2	MINE	m	3	0
3	FARM	f	2	1
4	DEVASTATED	x	5	1
5	GOLDMINE	$	3	1
6	FORT	!	6	1
7	RUIN	&	5	0
8	STOCKADE	s	6	0
9	CAPITOL	C	1	1
10	SPECIAL	?	3	0
11	LUMBERYD	l	2	0
12	BLKSMITH	b	3	0
13	ROAD	+	7	1
14	MILL	*	2	0
15	GRANARY	g	2	0
16	CHURCH	=	4	0
17	UNIVERSITY	u	4	0
18	NODESIG	-	0	0
19	BASE CAMP	P	1	1
\.

CREATE TABLE races (
	race_id				INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN
);

COMMENT ON TABLE		races								IS 'Every nation belongs to SOME race';
COMMENT ON COLUMN		races.race_id						IS 'Unique ID for each race';
COMMENT ON COLUMN		races.name							IS 'Display name for the race';
COMMENT ON COLUMN		races.symbol						IS 'Display symbol (character)';
COMMENT ON COLUMN		races.color							IS 'What color do we use? (curses)';
COMMENT ON COLUMN		races.bold							IS 'Do we turn bold on for this? (curses)';

COPY races (race_id, name, symbol, color, bold) FROM stdin;
0	GOD	-	0	0
1	ORC	O	1	0
2	ELF	E	2	0
3	DWARF	D	3	0
4	LIZARD	L	7	1
5	HUMAN	H	5	0
6	PIRATE	P	7	1
7	SAVAGE	S	7	1
8	NOMAD	N	7	1
9	UNKNOWN	?	0	0
\.

CREATE TABLE diplomacy_status(
	status_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	binding				BOOLEAN
);

COMMENT ON TABLE		diplomacy_status					IS 'The various diplomatic states'
COMMENT ON COLUMN		diplomacy_status.status_id			IS 'Which one'
COMMENT ON COLUMN		diplomacy_status.name				IS 'Text label'
COMMENT ON COLUMN		diplomacy_status.binding			IS 'Does this cost to change'

COPY diplomacy_status (status_id, name, binding) FROM stdin;
0	UNMET	FALSE
1	TREATY	TRUE
2	ALLIED	FALSE
3	FRIENDLY	FALSE
4	NEUTRAL	FALSE
5	HOSTILE	FALSE
6	WAR	FALSE
7	JIHAD	TRUE
\.

CREATE TABLE army_status (
	status_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		army_status							IS 'The various army states'
COMMENT ON COLUMN		army_status.status_id				IS 'Which one'
COMMENT ON COLUMN		army_status.name					IS 'Text label'

COPY army_status (status_id, name) FROM stdin;
0	?
1	MARCH
2	SCOUT
3	GARRISON
4	TRADED
5	MILITIA
6	FLYING
7	DEFEND
8	MAG_DEF
9	ATTACK
10	MAG_ATT
11	GENERAL
12	SORTIE
13	SIEGE
14	BESIEGED
15	ON_BOARD
16	RULE
17	LED
\.

CREATE TABLE directions (
	direction_id		INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		directions							IS 'The movement directions'
COMMENT ON COLUMN		directions.direction_id				IS 'Which one'
COMMENT ON COLUMN		directions.name						IS 'Text label'

COPY directions (direction_id, name) FROM stdin;
0	here
1	north
2	northeast
3	east
4	southeast
5	south
6	southwest
7	west
8	northwest
\.

CREATE TABLE classes (
	class_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		classes								IS 'What class a nation has'
COMMENT ON COLUMN		classes.class_id					IS 'Which one'
COMMENT ON COLUMN		classes.name						IS 'Text label'

COPY classes (class_id, name) FROM stdin;
0	monster
1	king
2	emperor
3	wizard
4	priest
5	pirate
6	trader
7	warlord
8	demon
9	dragon
10	shadow
\.

CREATE TABLE alignments (
	alignment_id		INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		alignments							IS 'What alignment does a nation follow'
COMMENT ON COLUMN		alignments.alignment_id				IS 'Which one'
COMMENT ON COLUMN		alignments.name						IS 'Text label'

COPY alignments (alignment_id, name) FROM stdin;
0	Other
1	Good
2	Neutral
3	Evil
4	Other
\.

CREATE TABLE units (
	unit_id				INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	short_name			TEXT NOT NULL,
	min_strength		INTEGER NOT NULL,
	attack				INTEGER NOT NULL,
	defend				INTEGER NOT NULL,
	move				INTEGER NOT NULL,
	metal_cost			INTEGER NOT NULL,
	enlist_cost			INTEGER NOT NULL,
	maint_cost			INTEGER NOT NULL,
);

COMMENT ON TABLE		units								IS 'Details about particular unit types'
COMMENT ON COLUMN		units.unit_id						IS 'Which one'
COMMENT ON COLUMN		units.name							IS 'Text label'
COMMENT ON COLUMN		units.short_name					IS 'Main screen label'
COMMENT ON COLUMN		units.min_strength					IS 'Unit must have this many men'
COMMENT ON COLUMN		units.attack						IS 'Attack bonus'
COMMENT ON COLUMN		units.defend						IS 'Defense bonus'
COMMENT ON COLUMN		units.move							IS 'Movement rate'
COMMENT ON COLUMN		units.metal_cost					IS 'Metal cost to draft'
COMMENT ON COLUMN		units.enlist_cost					IS 'Money cost to draft'
COMMENT ON COLUMN		units.maint_cost					IS 'Money per turn to keep'

COPY units (unit_id, name, short_name, min_strength, attack, defend, move, metal_cost, enlist_cost, maint_cost) FROM stdin;
0	Militia	mlta	1	-40	-25	0	0	50	20
1	Goblins	Gob	1	-15	-15	10	80	70	20
2	Orcs	Orc	1	0	0	10	80	85	50
3	Infantry	Inf	1	0	0	10	100	100	50
4	Sailors	Sail	1	0	0	0	100	100	50
5	Marines	XMar	1	5	0	0	100	100	50
6	Archers	Arch	1	0	10	10	100	100	50
7	Uruk-Hai	Uruk	1	5	5	10	150	125	50
8	Ninjas	Ninj	1	20	0	10	150	125	50
9	Phalanx	Phax	1	10	10	10	150	150	50
10	Olog-Hai	olog	1	15	15	10	150	180	75
11	Legionaries	Legn	1	20	20	10	150	180	75
12	Dragoons	Drag	1	10	10	20	100	300	200
13	Mercenaries	Merc	1	0	0	10	0	225	100
14	Trolls	Trol	1	25	15	10	200	225	100
15	Elite	Elt	1	20	20	13	200	225	100
16	Lt_Cavalry	lCav	1	20	20	20	100	300	175
17	Hv_Cavalry	hCav	1	30	30	20	300	450	225
18	Catapults	cat	1	-20	20	5	1000	600	250
19	Siege	sge	1	-20	20	5	1000	600	250
20	Rocs	Roc	1	20	30	10	300	600	250
21	Knights	Kni	1	40	40	20	600	600	250
22	Gryfins	grif	1	40	50	15	400	800	250
23	Elephants	ele	1	50	50	5	600	600	250
24	Zombies	zom	1	-15	-15	10	100	100	0
25	Spy	Spy	1	-30	-30	10	0	10000	2000
26	Scout	Scout	1	-30	-30	20	0	100	100
27	King	King	100	30	30	20	0	0	0
28	Baron	Bar	50	20	20	20	0	0	0
29	Emperor	Emp	100	30	30	20	0	0	0
30	Prince	Prin	50	20	20	20	0	0	0
31	Wizard	Wizd	250	30	30	20	0	0	0
32	Mage	Magi	50	20	20	20	0	0	0
33	Pope	Apos	100	30	30	20	0	0	0
34	Bishop	Bish	50	20	20	20	0	0	0
35	Admiral	Admi	100	30	30	20	0	0	0
36	Captain	Capt	50	20	20	20	0	0	0
37	Warlord	Warl	250	30	30	20	0	0	0
38	Lord	Lord	125	20	20	20	0	0	0
39	Demon	Demn	250	50	50	20	0	0	0
40	Devil	Devl	50	20	20	20	0	0	0
41	Dragyn	Drag	500	50	50	20	0	0	0
42	Wyrm	Wyrm	100	40	40	20	0	0	0
43	Shadow	Shad	250	50	50	20	0	0	0
44	Nazgul	Nazg	125	40	40	20	0	0	0
45	Spirit	spir	50	0	0	10	0	2	1000
46	Assasin	Assn	50	20	20	10	0	2	400
47	Efreet	efr	50	10	10	15	0	2	400
48	Gargoyl	Garg	75	10	10	10	0	2	450
49	Wraith	Wra	75	10	10	10	0	2	450
50	Hero	Hero	75	0	0	10	0	2	200
51	Centaur	Cent	50	10	10	15	0	2	200
52	Giant	gt	150	0	0	10	0	5	2100
53	Suphero	Shro	150	15	15	10	0	5	450
54	Mummy	Mumm	150	15	15	10	0	5	1000
55	Elmentl	Elem	175	5	5	15	0	5	1900
56	Mintaur	mino	150	20	20	10	0	5	2100
57	Daemon	daem	500	50	50	10	0	10	6000
58	Balrog	Bal	500	40	40	15	0	10	6000
59	Dragon	Drgn	1000	50	50	20	0	15	10000
\.

CREATE TABLE seasons (
	season_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		seasons								IS 'Things that change over seasons'
COMMENT ON COLUMN		seasons.season_id					IS 'Season number'

COPY seasons (season_id, name) FROM stdin;
0	Winter
1	Spring
2	Summer
3	Fall
\.

CREATE TABLE months (
	month_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	season_id			INTEGER NOT NULL					REFERENCES(seasons.season_id)
);

COMMENT ON TABLE		months								IS 'Things that change over months'
COMMENT ON COLUMN		months.month_id						IS 'Month number'
COMMENT ON COLUMN		months.season_id					IS 'Season this month belongs to'

COPY months (month_id, name, season_id) FROM stdin;
0	December	0
1	January	0
2	February	0
3	March	1
4	April	1
5	May	1
6	June	2
7	July	2
8	August	2
9	September	3
10	October	3
11	November	3
\.

CREATE TABLE power_styles (
	power_style_id		INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		power_styles						IS 'Categories of magic powers'
COMMENT ON COLUMN		power_styles.style_id				IS 'Which type'
COMMENT ON COLUMN		power_styles.name					IS 'What is it called'

COPY power_styles (style_id, name) FROM stdin;
0	MILITARY
1	CIVILIAN
2	MAGIC
\.

CREATE TABLE powers (
	power_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	style_id			INTEGER NOT NULL					REFERENCES(power_styles.style_id)
);

COMMENT ON TABLE	powers									IS 'Magic powers'
COMMENT ON COLUMN	powers.power_id							IS 'Which power'
COMMENT ON COLUMN	powers.name								IS 'What is it'
COMMENT ON COLUMN	powers.style_id							IS 'What category is it'

COPY powers (power_id, name) FROM stdin;
0	WARRIOR	0
1	CAPTAIN	0
2	WARLORD	0
3	ARCHER	0
4	CAVALRY	0
5	SAPPER	0
6	ARMOR	0
7	AVIAN	0
8	MI_MONST	0
9	AV_MONST	0
10	MA_MONST	0
11	SLAVER	1
12	DERVISH	1
13	HIDDEN	1
14	ARCHITECT	1
15	RELIGION	1
16	MINER	1
17	BREEDER	1
18	URBAN	1
19	STEEL	1
20	NINJA	1
21	SAILOR	1
22	DEMOCRACY	1
23	ROADS	1
24	THE_VOID	2
25	KNOWALL	2
26	DESTROYER	2
27	VAMPIRE	2
28	SUMMON	2
29	WYZARD	3
30	SORCERER	3
\.

-- NULL designation_id means it can be any designation (was 'x' in the code)

CREATE TABLE tradegoods (
	tradegood_id		INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN,
	designation_id		INTEGER								REFERENCES(designations.designation_id),
	value				INTEGER
);

COMMENT ON TABLE		tradegoods							IS 'Tradegoods, or the Special sector attribute'
COMMENT ON COLUMN		tradegoods.tradegood_id				IS 'Which good'
COMMENT ON COLUMN		tradegoods.name						IS 'What is it'
COMMENT ON COLUMN		tradegoods.symbol					IS 'Display symbol (character)';
COMMENT ON COLUMN		tradegoods.color					IS 'What color do we use? (curses)';
COMMENT ON COLUMN		tradegoods.bold						IS 'Do we turn bold on for this? (curses)';
COMMENT ON COLUMN		tradegoods.designation_id			IS 'Which designation will benefit from this';
COMMENT ON COLUMN		tradegoods.value					IS 'How much of a bonus does it give';

COPY tradegoods (tradegood_id, name, symbol, color, bold, designation_id, value) FROM stdin;
0	furs	f	5	0	NULL	1
1	wool	w	5	0	3	3
2	beer	b	5	0	3	3
3	cloth	c	5	0	3	3
4	wine	w	5	0	3	5
5	mules	m	6	0	0	1
6	horses	h	6	0	0	5
7	pigeons	p	6	0	0	7
8	griffons	g	6	0	0	9
9	corn	c	2	0	3	1
10	fish	f	2	0	3	1
11	sugar	s	2	0	3	4
12	honey	h	2	0	3	3
13	fruit	f	2	0	3	3
14	rice	r	2	0	3	4
15	wheat	w	2	0	3	4
16	dairy	d	2	0	3	2
17	peas	p	2	0	3	3
18	bread	b	2	0	3	3
19	cereal	c	2	0	3	1
20	pottery	p	3	0	0	1
21	salt	s	3	0	NULL	3
22	timber	t	3	0	11	1
23	granite	g	3	0	NULL	1
24	pine	p	3	0	11	3
25	oak	o	3	0	11	5
26	nails	n	3	0	0	7
27	papyrus	p	7	0	0	3
28	math	m	7	0	17	4
29	library	l	7	0	17	5
30	drama	d	7	0	1	6
31	paper	p	7	0	0	7
32	literature	l	7	0	1	7
33	law	l	7	0	1	8
34	philosophy	p	7	0	1	9
35	irrigation	i	2	1	3	1
36	oxen	o	2	1	3	2
37	plows	p	2	1	3	3
38	stones	s	5	1	10	9
39	herbs	h	3	1	NULL	3
40	medicine	m	3	1	0	7
41	torture	t	1	1	1	5
42	prison	p	1	1	1	7
43	bronze	b	7	0	2	1
44	copper	c	7	1	2	1
45	lead	l	7	1	2	1
46	tin	t	7	1	2	1
47	iron	i	7	1	2	1
48	steel	s	7	1	2	1
49	mithril	m	7	1	2	1
50	adamantine	a	7	1	2	1
51	spice	s	3	1	5	1
52	silver	s	3	1	5	1
53	pearls	p	3	1	5	1
54	dye	d	3	1	5	1
55	silk	s	3	1	5	1
56	gold	g	3	1	5	1
57	rubies	r	3	1	5	1
58	ivory	i	3	1	5	1
59	diamonds	d	3	1	5	1
60	platinum	p	3	1	5	1
61	NONE	0	0	0	NULL	0
\.

CREATE TABLE locations (
	location_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		locations							IS 'What location quality do you start with';
COMMENT ON COLUMN		locations.location_id				IS 'Identifier';
COMMENT ON COLUMN		locations.name						IS 'What do we call it';

COPY locations (location_id, name) FROM stdin;
0	Random
1	Fair
2	Great
\.

CREATE TABLE strategies (
	strategy_id			INTEGER NOT NULL PRIMARY KEY,
	name				TEXT NOT NULL
);

COMMENT ON TABLE		strategies							IS 'What NPC strategy is being used';
COMMENT ON COLUMN		strategies.strategy_id				IS 'Identifier';
COMMENT ON COLUMN		strategies.name						IS 'What do we call it';

COPY strategies (strategy_id, name) FROM stdin;
0	INACTIVE
1	PC_GOOD
2	PC_NEUTRAL
3	PC_EVIL
4	GOOD_0FREE
5	GOOD_2FREE
6	GOOD_4FREE
7	GOOD_6FREE
8	NEUTRAL_0FREE
9	NEUTRAL_2FREE
10	NEUTRAL_4FREE
11	NEUTRAL_6FREE
12	EVIL_0FREE
13	EVIL_2FREE
14	EVIL_4FREE
15	EVIL_6FREE
16	ISOLATIONIST
17	NPC_PEASANT
18	NPC_PIRATE
19	NPC_LIZARD
20	NPC_NOMAD
21	NPC_SAVAGE
\.

CREATE TABLE nations (
	nation_id			SERIAL NOT NULL PRIMARY KEY,
	world_id			INTEGER NOT NULL					REFERENCES(world.world_id),

	user_id				INTEGER								REFERENCES(users.user_id),
	name				TEXT NOT NULL,
	leader				TEXT NOT NULL,
	symbol				CHAR(1) NOT NULL,
	color				INTEGER,
	bold				BOOLEAN,

	race_id				INTEGER NOT NULL					REFERENCES(races.race_id),
	location_id			INTEGER NOT NULL					REFERENCES(locations.location_id),
	capitol_sector_id	INTEGER NOT NULL					REFERENCES(sectors.sector_id),
	relative_sector_id	INTEGER NOT NULL					REFERENCES(sectors.sector_id),
	strategy_id			INTEGER NOT NULL					REFERENCES(strategies.strategy_id),
	moverate			INTEGER NOT NULL,
	repro				INTEGER NOT NULL,
	score				INTEGER NOT NULL,
	tgold				INTEGER NOT NULL,
	jewels				INTEGER NOT NULL,
	tmil				INTEGER NOT NULL,
	tciv				INTEGER NOT NULL,
	metals				INTEGER NOT NULL,
	tfood				INTEGER NOT NULL,
	powers				INTEGER NOT NULL,
	class				INTEGER NOT NULL,
	aplus				INTEGER NOT NULL,
	dplus				INTEGER NOT NULL,
	spellpts			INTEGER NOT NULL,
	tsctrs				INTEGER NOT NULL,
	tships				INTEGER NOT NULL,
	inflation			INTEGER NOT NULL,
	charity				INTEGER NOT NULL,

	tax_rate			INTEGER NOT NULL,
	prestige			INTEGER NOT NULL,
	popularity			INTEGER NOT NULL,
	power				INTEGER NOT NULL,
	communications		INTEGER NOT NULL,
	wealth				INTEGER NOT NULL,
	eatrate				INTEGER NOT NULL,
	spoilrate			INTEGER NOT NULL,
	knowledge			INTEGER NOT NULL,
	farm_ability		INTEGER NOT NULL,
	mine_ability		INTEGER NOT NULL,
	poverty				INTEGER NOT NULL,
	terror				INTEGER NOT NULL,
	reputation			INTEGER NOT NULL
);

COMMENT ON TABLE		nations								IS 'Nations play on the world map';
COMMENT ON COLUMN		nations.user_id						IS 'What user is playing this nation?';
COMMENT ON COLUMN		nations.name						IS 'What is this nation called?';
COMMENT ON COLUMN		nations.leader						IS 'The name of the allmighty of the land';
                                                            
CREATE UNIQUE INDEX		ix_nation							ON nations ( world_id, nation_id );
COMMENT ON INDEX		ix_nation							IS 'nations are unique by world';

CREATE TABLE sectors (
	sector_id			SERIAL NOT NULL PRIMARY KEY,
	world_id			INTEGER NOT NULL					REFERENCES(world.world_id),
	coordinates			POINT NOT NULL, -- (X,Y)

	designation_id		INTEGER								REFERENCES(designations.designation_id),
	altitude_id			INTEGER NOT NULL					REFERENCES(altitudes.altitude_id),
	vegetation_id		INTEGER NOT NULL					REFERENCES(vegetations.vegetation_id),
	nation_id			INTEGER								REFERENCES(nations.nation_id),
	people				INTEGER NOT NULL,
	i_people			INTEGER NOT NULL,
	jewels				INTEGER NOT NULL,
	fortress			INTEGER NOT NULL,
	metal				INTEGER NOT NULL,
	tradegood_id		INTEGER								REFERENCES(tradegoods.tradegood_id)
);

COMMENT ON TABLE		sectors								IS 'Sectors make up the world map';
COMMENT ON COLUMN		sectors.sector_id					IS 'Unique identifier (convenience)';
COMMENT ON COLUMN		sectors.world_id					IS 'What world is this a part of';
COMMENT ON COLUMN		sectors.coordinates					IS 'Position in world.';
                                                            
COMMENT ON COLUMN		sectors.designation_id				IS 'designation of sector';
COMMENT ON COLUMN		sectors.altitude_id					IS 'sector altitude';
COMMENT ON COLUMN		sectors.vegetation_id				IS 'sector vegetation';
COMMENT ON COLUMN		sectors.nation_id					IS 'nation id of owner';
COMMENT ON COLUMN		sectors.people						IS 'civilians in sector';
COMMENT ON COLUMN		sectors.i_people					IS 'initial civilians in sector';
COMMENT ON COLUMN		sectors.jewels						IS 'jewel production ability';
COMMENT ON COLUMN		sectors.fortress					IS 'fortification level: 0 to 12';
COMMENT ON COLUMN		sectors.metal						IS 'metal produced by sector';
COMMENT ON COLUMN		sectors.tradegood_id				IS 'exotic trade goods in sector';
                                                            
CREATE UNIQUE INDEX		ix_sector							ON sectors ( world_id, coordinates );
COMMENT ON INDEX		ix_sector							IS 'sectors are unique by position in a given world';

CREATE TABLE armies (
	nation_id			INTEGER REFERENCES(nations.nation_id),
	sector_id			INTEGER REFERENCES(sectors.sector_id),
	army_id				INTEGER NOT NULL,

	unittyp				INTEGER NOT NULL,
	smove				INTEGER NOT NULL,
	sold				INTEGER NOT NULL,
	stat				INTEGER NOT NULL
);

CREATE TABLE navies (
	nation_id			INTEGER REFERENCES(nations.nation_id),
	sector_id			INTEGER REFERENCES(sectors.sector_id),
	navy_id				INTEGER NOT NULL,

	warships			INTEGER NOT NULL,
	merchant			INTEGER NOT NULL,
	galleys				INTEGER NOT NULL,
	smove				INTEGER NOT NULL,
	crew				INTEGER NOT NULL,
	people				INTEGER NOT NULL,
	commodity			INTEGER NOT NULL,
	armynum				INTEGER NOT NULL
);

CREATE TABLE diplomacy (
	world_id			INTEGER NOT NULL					REFERENCES(world.world_id),
	nation_id			INTEGER NOT NULL					REFERENCES(nations.nation_id),
	target_id			INTEGER NOT NULL					REFERENCES(nations.nation_id),
	status_id			INTEGER NOT NULL					REFERENCES(diplomacy_status.status_id)
);

