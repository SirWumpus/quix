/*
	qio.c

	880924	ACH		conversion to Atari ST & Acylon C
	880925	ACH		+ add bonus for captured Quixes
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __WIN32__
# include <conio.h>

static int wait_key;
#else
# include <sys/types.h>
# include <termios.h>

static struct termios otios;
#endif

void
set_io(int wait)
{
#ifdef __WIN32__
	wait_key = wait;
#else
	struct termios ntios;

	ntios = otios;
	ntios.c_cc[VMIN] = wait;
	ntios.c_cc[VTIME] = 0;
	ntios.c_lflag &= ~(ECHO|ICANON);
/*	ntios.c_oflag &= ~(OPOST); */

	if (tcsetattr(0, TCSANOW, &ntios)) {
		printf("Oops\n");
		exit(2);
	}
#endif
}

void
init_io(void)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

#ifdef TERMIOS
	if (tcgetattr(0, &otios)) {
		printf("Oops\n");
		exit(2);
	}

	set_io(0);
#endif
}

void
fini_io(void)
{
#ifdef TERMIOS
	(void) tcsetattr(0, TCSANOW, &otios);
#endif
}

#ifdef __WIN32__

int
getkey(void)
{
	if (wait_key || kbhit())
		return getch();

	return EOF;
}

#else

int
getkey()
{
	unsigned char ch;

	if (read(STDIN_FILENO, &ch, 1) == 1)
		return ch;

	return EOF;
}

#endif

void
quit()
{
	clearscreen();
	fini_io();
	exit(0);
}

void
init()
{
	int     i;

	init_io();

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

	inchar	= EOF;
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
#ifdef HAVE_USLEEP
	usleep(x * NAP_FACTOR);
#else
	for (x *= NAP_FACTOR; x; x--)
		;
#endif
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
#ifdef VT52
	printf("\033Y%c%c", y+32, x+32);
#endif
#ifdef ANSI
	printf("\033[%d;%dH", y+1, x+1);
#endif
} /* move */


clearscreen()
{
	print(HOME);
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
