/*
	qarea.c

	880924	ACH		convert to Atari ST & Acylon
	880925	ACH		add bonus for Quixes captured, 250 points each.
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/


#include "defs.h"

#define LINE_BORDER     'L'
#define BORDER_SOLID    'B'

int     min;
int     max;
int     min_min;

short near_border(a, b)
	int     a;
	int     b;
{
	return ((a - b == 1 || b - a == 1) ||
			(a == bord_max && b == bord_min) ||
			(b == bord_max && a == bord_min));
} /* near_border */


short line_near(i, j, at_border)
	register int    i;
	register int    j;
	short           at_border;
{
	register char   c0;
	register char   c1;
	register char   border;

	c0 = board[i][j];
	c1 = board[i][j+1];
	border = (at_border ? BORDER : BORDER_SOLID);

	if ((c0 == LINE_BORDER || c0 == border) &&
		(c1 == LINE_BORDER || c1 == border) ){
		register int    f0;
		register int    f1;

		f0 = bound_pos(i, j);
		f1 = bound_pos(i, j+1);

		if (c0 == LINE_BORDER) {
			if (c1 == LINE_BORDER)
				return f0 - f1 == 1 || f1 - f0 == 1 ||
					   ((f0 == line_max + 1 || f0 == line_min) &&
					   (f1 == line_max + 1 || f1 == line_min));
			else if (boundary[max].x == i && boundary[max].y == j)
				return near_border(max, f1);
			else if (boundary[min].x == i && boundary[min].y == j)
				return near_border(min, f1);
			else
				return FALSE;
		} else if (c1 == LINE_BORDER) {
			if (boundary[max].x == i && boundary[max].y == j + 1)
				return near_border(max, f0);
			else if (boundary[min].x == i && boundary[min].y == j + 1)
				return near_border(min, f0);
			else
				return FALSE;
		} else {
			return near_border(f0, f1);
		} /* if */
	} /* if */
	return FALSE;
} /* line_near */


int find(x1, y1)
	register int    x1;
	register int    y1;
{
        register int            i;
        register struct coord   *b;

        for (i = bord_min, b = boundary + i; i <= bord_max; i++, b++)
                if (b->x == x1 && b->y == y1)
                        return i;
        print("\nError in find()\n");
        quit();
} /* find */


void fill_area()
{
	unsigned        tpercent;       /* for percent calculation */
	register int    i;
	register int    j;
	register struct quixtype *q;

	min = find(boundary[line_max+1].x, boundary[line_max+1].y);
	max = find(player.x, player.y);
	boundary[line_min].x = player.x;
	boundary[line_min].y = player.y;

	if (max < min) {
		i = max;
		max = min;
		min = i;
	} /* if */

	for (i = 0; i < sparxnum; i++) {
		sparx[i].x = boundary[sparx[i].pos].x;
		sparx[i].y = boundary[sparx[i].pos].y;
		board[sparx[i].x][sparx[i].y] = BORDER;
	} /* for */

	for (i = min + 1; i < max; i++)
		board[boundary[i].x][boundary[i].y] = BORDER_SOLID;

	for (i = line_min; i <= line_max+1; i++)
		board[boundary[i].x][boundary[i].y] = LINE_BORDER;

	for (i = 0; i < BOUNDARY_LEN; i++) {
			temp[i].x = boundary[i].x;
			temp[i].y = boundary[i].y;
	} /* for */

	min_min = temp[line_max+1].x == temp[max].x &&
			  temp[line_max+1].y == temp[max].y;

	if (scan_screen(FALSE, FALSE) > area_left / 2) {
		area_left -= scan_screen(TRUE, TRUE);
		for (j = min, i = bord_min; j <= max; i++, j++) {
			boundary[i].x = temp[j].x;
			boundary[i].y = temp[j].y;
		} /* for */

		if (min_min) {
			for (j = line_max; j > line_min; j--, i++) {
				boundary[i].x = temp[j].x;
				boundary[i].y = temp[j].y;
			} /* for */
		} else {
			for (j = line_min + 1; j <= line_max; j++, i++) {
				boundary[i].x = temp[j].x;
				boundary[i].y = temp[j].y;
			} /* for */
		} /* if */
	} else {
		area_left -= scan_screen(TRUE, FALSE);
		for (i = j = bord_min; j <= min; i++, j++) {
			boundary[i].x = temp[j].x;
			boundary[i].y = temp[j].y;
		} /* for */

		if (!min_min) {
			for (j = line_max; j > line_min; j--, i++) {
				boundary[i].x = temp[j].x;
				boundary[i].y = temp[j].y;
			} /* for */
		} else {
			for (j = line_min + 1; j <= line_max; j++, i++) {
				boundary[i].x = temp[j].x;
				boundary[i].y = temp[j].y;
			} /* for */
		} /* if */

		for (j = max; j <= bord_max; j++, i++) {
			boundary[i].x = temp[j].x;
			boundary[i].y = temp[j].y;
		} /* for */
	} /* if */

	bord_max = i - 1;
	player.pos = find(player.x, player.y);

	for (i = 0; i < sparxnum; i++) {
		if (board[sparx[i].x][sparx[i].y] != BORDER) {
			if (sparx[i].dir == 1) {
				sparx[i].pos = (player.pos == bord_max) ?
								bord_min : player.pos +1;
			} else {
				sparx[i].pos = (player.pos == bord_min) ?
								bord_max : player.pos -1;
			} /* if */
		} else {
			sparx[i].pos = find(sparx[i].x, sparx[i].y);
		} /* if */
	} /* for */

	/*
	 * DON'T alter the way "percent" is calculated or the intermeadiate
	 * results in the calculation will OVERFLOW without you knowing it
	 * and weird results will occur!!
	 */

	tpercent = (((maxarea/10)-(area_left/10))*100)/(maxarea/10);
	percent = tpercent;
	if (percent < 75) {
		move(34, 0);
		putint(percent, 1);
		qputch('%');
	} /* if */

	/* Check for captures. */
	for( i = quixnum, q = quix; i; --i, ++q ){
		if( !q->cought && board[q->x][q->y] == SOLID ){
			++QuixCaptured;
			q->cought = TRUE;
		} /* if */
	} /* for */
} /* fill_area */


int scan_screen(draw, border)
	register short  draw;
	register short  border;
{
	register int    i;
	register int    j;
	register short  in;
	register int    area;
	int             quix_in;
	int             quix_out;

	area = -2;
	quix_in = 0;
	quix_out = 0;

	lastx = 0;
	for (j = 1; j <= ymax; j++) {
		in = FALSE;
		for (i = 1; i <= xmax; i++) {
			int     surface;

			surface = board[i][j];

			if (surface != SOLID && surface != NOTHING &&
				j < ymax && line_near(i, j, border))
				in = !in;

			if (draw) {
				switch (surface) {
					case NOTHING:
					case QUIX:
						if (in) {
							area++;
							mvaddch(i, j, SOLID);
						} /* if */
						break;

					case BORDER:
						if (border)
							mvaddch(i, j, SOLID);
						break;

					case BORDER_SOLID:
						if (border)
							board[i][j] = BORDER;
						else
							mvaddch(i, j, SOLID);
						break;

					case LINE_BORDER:
						area++;
						mvaddch(i, j, BORDER);
				} /* switch */
			} else {
				switch (surface) {
					case NOTHING:
						if (in) area++;
						break;

					case LINE_BORDER:
						area++;
						break;

					case QUIX:
						if (in) {
							area++;
							quix_in++;
						} else
							quix_out++;
				} /* switch */
			} /* if */
		} /* for */
	} /* for */

	if (!draw) {
		if (quix_in == 0)
			return 0;
		else if (quix_out == 0)
			return maxarea;
		else if (quix_in - quix_out > 3)
			return maxarea;
		else if (quix_out - quix_in > 3)
			return 0;
	} else {
		score += (area/2) * quixnum;
		add_life();
		move(46, 0);
		putint(score, 1);
		qputch('0');
	} /* if */
	return area;
} /* scan_screen */


int bound_pos(x1, y1)
	register int    x1;
	register int    y1;
{
	register int            i;
	register struct coord   *b;

	if (board[x1][y1] == BORDER || board[x1][y1] == BORDER_SOLID) {
		for (i = bord_min, b = boundary + i; i <= bord_max; i++, b++)
			if (b->x == x1 && b->y == y1)
				return i;
	} else {
		for (i = line_max + 1, b = boundary + i; i >= line_min; i--, b--)
			if (b->x == x1 && b->y == y1)
				return i;
	} /* if */

	print("\nError in bound_pos()\n");
	quit();
}
