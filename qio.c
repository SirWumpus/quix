/*
	qio.c

	880924	ACH		conversion to Atari ST & Acylon C
	880925	ACH		+ add bonus for captured Quixes
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/

#include "defs.h"


void quit()
{
	clearscreen();
	exit( 0 );
} /* quit */



void init()
{
	int     i;

	/* Initialise ALL variables */

	times=sparxnum=quixnum=menleft=area_left=last_killed=0;
	percent=fildes=startdir=siz=maxarea=xmax=ymax=fuse=0;
	bord_min=bord_max=line_min=line_max=lastx=lasty=0;
	bonus_men=fuse_lit=nohighscore=0;

	for( i=0; i<MAXQUIX; i++ ){
		quix[i].x=quix[i].y=quix[i].direction=quix[i].d_time=quix[i].start=0;
		quix[i].cought = FALSE;
	} /* for */

	for(i=0; i<BOUNDARY_LEN; i++) {
		boundary[i].x=boundary[i].y=0;
		temp[i].x=temp[i].y=0;
	} /* for */

	C_UP    = INT_UP;
	C_DOWN  = INT_DOWN;
	C_LEFT  = INT_LEFT;
	C_RIGHT = INT_RIGHT;
	bonus_men = 1;
	bord_min = 2;
	line_max = BOUNDARY_LEN - 2;
	xmax = MAX_X - 3;
	ymax = MAX_Y - 3;
	maxarea = (xmax - 2) * (ymax - 2);
	clearscreen();
}



nap( x )
	unsigned x;
{
	for (x *= NAP_FACTOR; x; x--); 
} /* nap */



terror(msg)
char    *msg;
{
	print(msg);
	qputch('\n');
	exit(1);
} /* terror */

move(x, y)
int     x;
int     y;
{
#ifdef ATARI_ST
	Cconws( "\033Y" );
	Cconout( y +32 );
	Cconout( x +32 );
#endif
} /* move */


clearscreen()
{
	print(CLEARS);
} /* clearscreen */


short ISBORDER(X)
	int X;
{
	return ((X) <= bord_max);
} /* ISBORDER */

short PLAYER_AT(X, Y)
	int     X;
	int     Y;
{
	return (X == player.x && Y == player.y);
} /* PLAYER_AT */
