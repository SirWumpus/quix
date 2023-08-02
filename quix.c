/*
	quix.c			Qix The Game

	841028			v2.0  -  Converted for CP/M by William (Chomp) King
	880924	ACH		Converted to Atari ST & Acylon C
	880925	ACH		+ bonus score for each captured Quix 250 points
					+ DEBUG code to set initial number of Quixes.
					+ DEBUG code to provide extra men.
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __unix__
#include <unistd.h>
#endif

int QuixCaptured;
int sparxnum;
int quixnum;
int menleft;
int area_left;
int last_killed;
int percent;
int fildes;
int startdir;
int siz;
int maxarea;
int board[MAX_X][MAX_Y];
int xmax;
int ymax;
int fuse;
int bord_min;
int bord_max;
int line_min;
int line_max;
int lastx;
int lasty;
int bonus_men;
int C_UP;
int C_DOWN;
int C_LEFT;
int C_RIGHT;
int inchar;

/* Note : the score is held in an unsigned integer variable and the
 * actual value held here is 10 times less than the score displayed
 * on the screen.  Thus maximum score = 655350.
 */
unsigned score;

int     fuse_lit;
int     nohighscore;
struct  playertype      player;
struct  sparxtype       sparx[MAXSPARX];
struct  quixtype        quix[MAXQUIX];
struct  scoretype       h_score[10];
struct  coord           boundary[BOUNDARY_LEN];
struct  coord           temp[BOUNDARY_LEN];

void
banner()
{
	int c = 0;

	do {
		if (c != EOF) {
			clearscreen();
			print( "\n\n\n\t\t  QQQQQ    UUU    UUU  IIIII  XXXX  XXXX\n" );
			print( "\t\t Q     Q    U      U     I      X    X\n" );
			print( "\t\tQ       Q   U      U     I       X  X\n" );
			print( "\t\tQ       Q   U      U     I        XX\n" );
			print( "\t\tQ       Q   U      U     I        XX\n" );
			print( "\t\tQ     Q Q   U      U     I       X  X\n" );
			print( "\t\t Q     Q     U    U      I      X    X\n" );
			print( "\t\t  QQQQQ Q     UUUU     IIIII  XXXX  XXXX\n" );
			print( "\n\n\n\t\t\t     Version 2.0\n" );
			print( "\t\t\t  25 September 1988\n\n\n" );
			print( "\t\t       Original author unknown\n" );
			print( "\t\tUnix to CP/M port by William (Chomp) King\n" );
			print( "\t\tCP/M to Atari ST port by Anthony C. Howe\n" );
			print( "\n\n\t\t\t  SPACE to Start" );
			print( "\n\n\t\t\t   '?' for Help\n" );
		}

		c = getkey();

		switch (c) {
		case ESC:
			exit(0);

		case 0x12:
			change_keys();
			break;

		case '?':
			help();
			break;
#ifdef DEBUG
		case '+':
			++quixnum;
			break;
#endif
		}
	} while (c != ' ');
} /* banner */


void
udelay(int d)
{
#ifdef UDELAY_FACTOR
	for (d *= UDELAY_FACTOR; 0 < d; --d)
		;
#else
	(void) usleep(d * 1000);
#endif
}


void
clearboard(void)
{
        register int    i;
        register int    j;

        for (i = 2; i < xmax; i++)
                for (j = 2; j < ymax; j++)
                        board[i][j] = NOTHING;

        for (i = 0; i <= xmax+1; i++)
                board[i][0] = board[i][ymax+1] = SOLID;

        for (j = 1; j <= ymax; j++)
                board[0][j] = board[xmax+1][j] = SOLID;

        bord_max = bord_min;
        line_min = line_max;

        for (i = 1; i <= xmax; i++)
        {
                boundary[bord_max].x = i;
                boundary[bord_max].y = 1;
                bord_max++;
        }

        for (j = 2; j < ymax; j++)
        {
                boundary[bord_max].x = xmax;
                boundary[bord_max].y = j;
                bord_max++;
        }

        for (i = xmax; i >= 1; i--)
        {
                boundary[bord_max].x = i;
                boundary[bord_max].y = ymax;
                bord_max++;
        }

        for (j = ymax-1; j > 1; j--)
        {
                boundary[bord_max].x = 1;
                boundary[bord_max].y = j;
                bord_max++;
        }

        bord_max--;

        for (i = bord_min; i <= bord_max; i++)
                board[boundary[i].x][boundary[i].y] = BORDER;

        sparx[0].dir = -1;
        sparx[0].pos = find(3, 1);
        sparx[1].dir = 1;
        sparx[1].pos = find(xmax-2, 1);
        sparxnum = 2;
        quixnum++;

        for (i = 0; i < quixnum; i++) {
			quix[i].x = rand() % (xmax - 8) + 4;
			quix[i].y = rand() % (ymax - 8) + 4;
			quix[i].d_time = 1;
			quix[i].start = 0;
			quix[i].cought = FALSE;
			board[quix[i].x][quix[i].y] = QUIX;

			for (j = 0; j < QUIXLEN; j++) {
				quix[i].posit[j].x = quix[i].x;
				quix[i].posit[j].y = quix[i].y;
			} /* for */
        } /* for */

        player.direction = STILL;
        player.x = xmax / 2;
        player.y = ymax;
        player.pos = find(player.x, player.y);
        board[player.x][player.y] = PLAYER;
        area_left = maxarea;
        percent = 0;
        drawscreen();
}

void
drawscreen(void)
{
        register int    i;
        register int    j;

        clearscreen();
        lastx = 0;

        for (j = 1; j <= ymax; j++)
                for (i = 1; i <= xmax; i++)
                        if (board[i][j] != NOTHING)
                                mvaddch(i, j, board[i][j]);

        for (i = 0; i < menleft; i++)
                put_at(i, 0, PLAYER);

        move(20, 0);
        print("SCREEN FILLED:");
        putint(percent, 1);
        qputch('%');
        move(40, 0);
        print("SCORE:");
        putint(score, 1);
        qputch('0');
}


static void
death(void)
{
        register int    i;

        if (percent >= 75)
                return;

        qputch( 7 );
        nap( 5 );
        put_at(--menleft, 0, NOTHING);
        last_killed = 0;
        fuse_lit = FALSE;
        player.direction = STILL;

        if (!ISBORDER(player.pos))
        {
                lastx = 0;
                for (i = line_max; i > line_min; i--)
                        mvaddch(boundary[i].x, boundary[i].y, NOTHING);

                player.x = boundary[line_max+1].x;
                player.y = boundary[line_max+1].y;
                player.direction = STILL;
                player.pos = find(player.x, player.y);
                mvaddch(player.x, player.y, PLAYER);
                line_min = BOUNDARY_LEN - 2;
        }
}

void
add_life(void)
{
	while( score > BONUS_MAN * bonus_men && menleft <= MAXMEN ){
		put_at( menleft++, 0, PLAYER );
		qputch( 7 );
		nap( 5 );
		++bonus_men;
	}
}

static void
get_bonus(void)
{
	clearscreen();
	move( 8, 4 );
	print( "AREA COVERED =" );
	putint( percent, 1 );
	qputch( '%' );
	move( 8, 6 );
	print( "AREA BONUS = (" );
	putint( percent, 1 );
	print( " - 75) *" );
	putint( (quixnum * 500), 1 );
	move( 8, 8 );
	print( "           =" );
	putint( ((percent - 75) * 500 * quixnum), 1 );
	score += (percent - 75) * 50 * quixnum;

	/* Bonus for quixes captured. */
	move( 8, 10 );
	print( "CAPTURE BONUS = " );
	putint( QuixCaptured, 1 );
	print( " * 250" );
	move( 8, 12 );
	print( "              =" );
	putint( QuixCaptured * 250, 1 );
	score += QuixCaptured * 25;

	move( 8, TLENGTH -4 );
	print( "--- press space bar ---" );
	while( getkey() != ' ' );
	add_life();
}

void
help(void)
{
        clearscreen();
        print( "Try to fill in over 75% of the screen\n");
        print( "without being killed. You are the '" );
		qputch( PLAYER );
        print( "'\nanything else that moves will kill you,\n" );
        print( "until you are dead!!!  ]:>\n\n" );
		print( "\tup\t\t\t'" );
		qputch( C_UP );
		print( "'\n\tdown\t\t\t'" );
		qputch( C_DOWN );
		print( "'\n\tleft\t\t\t'" );
		qputch( C_LEFT );
        print( "'\n\tright\t\t\t'" );
		qputch( C_RIGHT );
        print( "'\n\tquit\t\t\tESC\n" );
		print( "\thelp\t\t\t'?'\n" );
		print( "\tredraw screen\t\t'^D'\n" );
		print( "\tredefine keys\t\t'^R'\n" );
		print( "\tstop\t\t\tany other key\n" );
        print( "\nI'd give up if I were you.");
        move(1, ymax );
        print( "----- hit space to continue -----" );
        while (getkey() != ' ')
        	;
}

void
mvaddch(int x, int y, int ch)
{
        board[x][y] = ch;
        move(2 * x, y);
        qputch(ch);
}

void
put_at(int x, int y, int ch)
{
	if (ch != SPARX) {
		board[x][y] = ch;
	}
	move(2 * x, y);
	qputch(ch);
}

int
main(int atrgc, char **argv)
{
	register char   ch;

	(void) atexit(quit);

#ifdef CURSOROFF
	puts (CURSOROFF);
#endif
	init();
	banner();

	do {
		set_io(0);
		menleft = INITMEN;
		score = 0;

		for(;;){
			QuixCaptured = 0;
			clearboard();
			while( percent < 75 && menleft ){
				if( moveplayer() == DEAD || movequix() == DEAD ||
					movesparx() == DEAD || movefuse() == DEAD )
					death();
				last_killed++;
				udelay((10 - quixnum) * 30);
			} /* while */

		if( !menleft ) break;

			get_bonus();
		} /* for */

/*      highscores();           */
		move(1, ymax+1);
		set_io(1);
		printf("Are you so utterly mad as to have another game? (y/n) ");

		quixnum = 0;
		do {
			ch = getkey();
#ifdef DEBUG
			if( ch == '+' ) ++quixnum;
#endif
		} while (ch != 'y' && ch != 'n' && ch != 'Y' && ch != 'N');
	} while (ch != 'n' && ch != 'N');

	return 0;
}


