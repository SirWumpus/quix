/*
	qmoves.c

	880924	ACH		Converted to Atari ST & Acylon C
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

short had_go;

static void
process_command(void)
{
        int     i;

        i = inchar;
        if (i > 0)
        {
                inchar = EOF;
                had_go = TRUE;
                switch (i)
                {
                        case ESC:
                                score = 0;
                        /*      highscores();   */

#ifdef CURSORON
                                puts (CURSORON);
#endif
                                exit(0);
                                break;

                        case 0x12:
                                change_keys();
				/*@fallthrough@*/

                        case 0x04:
                                drawscreen();
                                break;

#ifdef DEBUG
						case 0x01:
							put_at( menleft++, 0, PLAYER );
							qputch( 7 );
							nap( 5 );
							++bonus_men;
							break;
#endif

                        case '?':
                                help();
			        drawscreen();
                                break;

                        default:
                                player.direction = STILL;
                }

                if (i == C_UP)
                        player.direction = UP;
                else if (i == C_DOWN)
                        player.direction = DOWN;
                else if (i == C_LEFT)
                        player.direction = LEFT;
                else if (i == C_RIGHT)
                        player.direction = RIGHT;
        }
}

void
change_keys(void)
{
        clearscreen();

	set_io(1);
        printf("Redefine keys\n");
        printf("-------------\n");

        printf("\nUP    = %c set to: ", C_UP);
        C_UP=getkey();
        putchar(C_UP);

        printf("\n\nDOWN  = %c set to: ", C_DOWN);
        C_DOWN=getkey();
        putchar(C_DOWN);

        printf("\n\nLEFT  = %c set to: ", C_LEFT);
        C_LEFT=getkey();
        putchar(C_LEFT);

        printf("\n\nRIGHT = %c set to: ", C_RIGHT);
        C_RIGHT=getkey();
        putchar(C_RIGHT);

	set_io(0);
        printf("\n\n----- hit space to continue -----");
        while (getkey() != ' ')
        	;
}

static void
draw_move(int x0, int y0, int x1, int y1, int c0, int c1)
{
        if (y0 != y1)
        {
                put_at(x0, y0, c0);
                put_at(x1, y1, c1);
        }
        else if (x0 < x1)
        {
                put_at(x0, y0, c0);
                qputch(' ');
                qputch(c1);
                board[x1][y1] = c1;
        }
        else if (x0 > x1)
        {
                put_at(x1, y1, c1);
                qputch(' ');
                qputch(c0);
                board[x0][y0] = c0;
        }
}

static int
same_boundary(int x1, int y1, int pos)
{
        register int    pred;
        register int    succ;

        if (ISBORDER(pos))
        {
                pred = pos == bord_min ? bord_max : pos - 1;
                succ = pos == bord_max ? bord_min : pos + 1;
        }
        else
        {
                pred = pos == line_min + 1 ? line_max : pos - 1;
                succ = pos == line_max ? line_min + 1 : pos + 1;
        }
        if (boundary[pred].x == x1 && boundary[pred].y == y1)
                return pred;
        if (boundary[succ].x == x1 && boundary[succ].y == y1)
                return succ;
        return 0;
}

static void
where_move(int direction, int *dx, int *dy)
{
        switch (direction)
        {
                case UP:
                        *dx = 0;
                        *dy = -1;
                        break;

                case DOWN:
                        *dx = 0;
                        *dy = 1;
                        break;

                case LEFT:
                        *dx = -1;
                        *dy = 0;
                        break;

                case RIGHT:
                        *dx = 1;
                        *dy = 0;
                        break;

                case UP_LEFT:
                        *dx = -1;
                        *dy = -1;
                        break;

                case UP_RIGHT:
                        *dx = 1;
                        *dy = -1;
                        break;

                case DOWN_LEFT:
                        *dx = -1;
                        *dy = 1;
                        break;

                case DOWN_RIGHT:
                        *dx = 1;
                        *dy = 1;
        }
}

int
moveplayer(void)
{
        register int    oldx;
        register int    oldy;
        register int    newx;
        register int    newy;
        register int    nextb;
        char            last_ch;
        int             dx;
        int             dy;

        oldx = player.x;
        oldy = player.y;
        process_command();
        if (player.direction == STILL)
        {
                if (!ISBORDER(player.pos) && !fuse_lit &&
                    line_max - line_min > 2)
                {
                        fuse_lit = TRUE;
                        fuse = line_max;
                }
                return ALIVE;
        }
        where_move(player.direction, &dx, &dy);
        newx = oldx + dx;
        newy = oldy + dy;
        nextb = board[newx][newy];
        if (nextb == LINE || nextb == SOLID)
        {
                player.direction = STILL;
                return ALIVE;
        }
        if (nextb == QUIX && !ISBORDER(player.pos))
                return DEAD;
        if (nextb == BORDER)
        {
                register int    position;

                last_ch = BORDER;
                position = same_boundary(newx, newy, player.pos);
                if (position == 0)              /* fill in area */
                        if ((newx != boundary[line_max+1].x ||
                             newy != boundary[line_max+1].y) &&
                             !ISBORDER(player.pos))
                        {
                                player.x = newx;
                                player.y = newy;
                                fill_area();
                                fuse_lit = FALSE;
                        }
                        else
                        {
                                player.direction = STILL;
                                return ALIVE;
                        }
                else
                {
                        player.x = newx;
                        player.y = newy;
                        player.pos = position;
                }
        }
        else                            /* nextb == LINE */
        {
                if (ISBORDER(player.pos))
                {
                        last_ch = BORDER;
                        if (!had_go)
                        {
                                player.direction = STILL;
                                return ALIVE;
                        }
                        else
                        {
                                boundary[line_max+1].x = oldx;
                                boundary[line_max+1].y = oldy;
                                player.pos = line_min = line_max;
                        }
                }
                else
                {
                        last_ch = LINE;
                        player.pos--;
                }
                player.x = newx;
                player.y = newy;
                boundary[line_min].x = newx;
                boundary[line_min].y = newy;
                line_min--;
        }
        draw_move(oldx, oldy, newx, newy, last_ch, PLAYER);
        had_go = FALSE;
        return ALIVE;
}

int
PLAYER_AT(int X, int Y)
{
	return (X == player.x && Y == player.y);
}

int
movesparx(void)
{
	register struct sparxtype *s;
	register int    i;
	register int    dx;
	register int    x1;
	register int    y1;

	for (i = 0, s = sparx; i < sparxnum; i++, s++) {
		x1 = boundary[s->pos].x;
		y1 = boundary[s->pos].y;
		if (PLAYER_AT(x1, y1) && last_killed > 10)
			return DEAD;
		put_at(x1, y1, board[x1][y1] == PLAYER ? PLAYER : BORDER);
		for (int speed = 0; speed < SPARX_SPEED; speed++) {
			if (s->dir == 1)
				s->pos = s->pos == bord_max ? bord_min : s->pos + 1;
			else
				s->pos = s->pos == bord_min ? bord_max : s->pos - 1;
			x1 = boundary[s->pos].x;
			y1 = boundary[s->pos].y;
			if (PLAYER_AT(x1, y1) && last_killed > 10)
				return DEAD;
		} /* for */
		put_at(x1, y1, SPARX);
	} /* for */
	return ALIVE;
}

int
movefuse(void)
{
        if (fuse_lit)
        {
                put_at(boundary[fuse].x, boundary[fuse].y, LINE);
                fuse--;
                if (PLAYER_AT(boundary[fuse].x, boundary[fuse].y))
                        return DEAD;
                put_at(boundary[fuse].x, boundary[fuse].y, FUSE);
        }
        return ALIVE;
}



int sgn(x)
	int     x;
{
	if (x>0)
		return (1);
	else if (x)
		return(-1);
	else
		return(0);
} /* sgn */

static int
there_move(int dx, int dy)
{
        if (dx>0)
                if (dy>0)
                        return(UP_RIGHT);
                else if (dy)
                        return(UP_LEFT);
                else
                        return(UP);
        else if (dx)
                if (dy>0)
                        return(DOWN_RIGHT);
                else if (dy)
                        return(DOWN_LEFT);
                else
                        return(DOWN);
        else if (dy>0)
                return(RIGHT);
        else if (dy)
                return(LEFT);
        else
                return(UP);
}

int
movequix(void)
{
	register struct quixtype *q;
	register int             i;
	register int             j;

	for (i = 0, q = quix; i < quixnum; i++, q++) {
		if (board[q->x][q->y] == SOLID) continue;

		for (j = 0; j < QUIX_SPEED; j++) {
			int     dx;
			int     dy;

			if (--q->d_time < 0) {
				q->direction = rand() % 8;
				q->d_time = rand() % 20;
			} /* if */
			if (rand()%NASTYNUM)
				where_move(q->direction, &dx, &dy);
			else {
				dx = sgn(player.x - q->x);
				dy = sgn(player.y - q->y);
				q->direction = there_move(dx, dy);
			} /* if */
			switch (board[q->x+dx][q->y+dy]) {
				register int    i1;
				register int    n;

				case LINE:
					return DEAD;

				case BORDER:
				case PLAYER:
					q->direction += (q->direction %2 == 0) ? 1 : -1;
					break;

				default:
					n = 0;
					for (i1 = 0; i1 < QUIXLEN; i1++)
						if (q->posit[i1].x == q->posit[q->start].x &&
							q->posit[i1].y == q->posit[q->start].y )
								n++;
					if (n == 1)
						put_at(q->posit[q->start].x,
							   q->posit[q->start].y,
							   NOTHING);
					q->posit[q->start].x = q->x += dx;
					q->posit[q->start].y = q->y += dy;
					q->start = (q->start + 1) % QUIXLEN;
					if (board[q->x][q->y] != QUIX)
						put_at(q->x, q->y, QUIX);
			} /* switch */
		} /* for */
	} /* for */
	return ALIVE;
} /* movequix */
