/*
	qdefs.h

	880924	ACH		Converted Atari ST & Acylon C
	880925	ACH		+ add bonus for captured Quixes
					+ DEBUG code for setting initial number of Quixes
					+ DEBUG code to give extra men
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/

#ifdef ATARI_ST
#include <osbind.h>
#endif

#undef	DEBUG
#define NAP_FACTOR		3275
#define UDELAY_FACTOR	125

/* Terminal Definitions. */
#define TWIDTH	80
#define TLENGTH 25
#define CLEARS  "\033E"
#define INTOREV	"\033p"
#define OUTAREV "\033q"
#define CURSOROFF "\033f"
#define CURSORON "\033e"
#define ESC		'\033'


#define WIDTH           38      /* Actual number of COLUMNS on your */
                                /* terminal - 2 (80 / 2 - 2)        */
#define HEIGHT          23      /* Actual number of ROWS on your    */
                                /* terminal - 2                     */

#define INT_UP          '8'     /* Initial key for UP    movement */
#define INT_DOWN        '2'     /*    "     "   "  DOWN     "     */
#define INT_LEFT        '4'     /*    "     "   "  LEFT     "     */
#define INT_RIGHT       '6'     /*    "     "   "  RIGHT    "     */

#define INITMEN         5
#define BONUS_MAN       1000    /* NOTE : means every 20000! */
#define MAXMEN          8
#define QUIXLEN         8
#define MAXQUIX         10
#define MAXSPARX        2
#define MAX_X           (WIDTH + 2)     /* DON'T TOUCH!! */
#define MAX_Y           (HEIGHT + 2)    /* DON'T TOUCH!! */
#define BOUNDARY_LEN    (MAX_X * MAX_Y)
#define TRUE            1
#define FALSE           0
#define ALIVE           1
#define DEAD            0
#define SPARX_SPEED     1
#define QUIX_SPEED      2
#define STILL           10
#define UP              0
#define DOWN            1
#define LEFT            2
#define RIGHT           3
#define UP_LEFT         4
#define DOWN_RIGHT      5
#define DOWN_LEFT       6
#define UP_RIGHT        7
#define NASTYNUM        6
#define LINE            '.'
#define SOLID           ':'
#define BORDER          '+'
#define NOTHING         ' '
#define QUIX            'Q'
#define PLAYER          '@'
#define SPARX           'X'
#define FUSE            '*'

struct  playertype
        {
                char    x;
                char    y;
                int     pos;
                int     direction;
        };

struct  sparxtype
        {
                char    x;
                char    y;
                int     pos;
                int     dir;
        };

struct  scoretype
        {
                int     value;
                char    name[80];
        };

struct  coord
        {
                char    x;
                char    y;
        };

struct  quixtype
        {
                char    x;
                char    y;
                int     cought;
                int     direction;
                int     d_time;
                int     start;
                struct  coord   posit[QUIXLEN];
        };

int		QuixCaptured;
int     times;
int     sparxnum;
int     quixnum;
int     menleft;
int     area_left;
int     last_killed;
int     percent;
int     fildes;
int     startdir;
int     siz;
int     maxarea;
int     board[MAX_X][MAX_Y];
int     xmax;
int     ymax;
int     fuse;
int     bord_min;
int     bord_max;
int     line_min;
int     line_max;
int     lastx;
int     lasty;
int     bonus_men;
int     C_UP;
int     C_DOWN;
int     C_LEFT;
int     C_RIGHT;
int     inchar;
unsigned        score;

/*      Note : the score is held in an unsigned integer variable and the
 *      actual value held here is 10 times less than the score displayed
 *      on the screen.  Thus maximum score = 655350.
 */

int     fuse_lit;
int     nohighscore;
struct  playertype      player;
struct  sparxtype       sparx[MAXSPARX];
struct  quixtype        quix[MAXQUIX];
struct  scoretype       h_score[10];
struct  coord           boundary[BOUNDARY_LEN];
struct  coord           temp[BOUNDARY_LEN];


/* End of defintions */
