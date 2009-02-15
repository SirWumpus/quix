/*
	qdefs.h

	880924	ACH		Converted Atari ST & Acylon C
	880925	ACH		+ add bonus for captured Quixes
					+ DEBUG code for setting initial number of Quixes
					+ DEBUG code to give extra men
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/

#if !defined(ATARI_ST)
#define ANSI
#define TERMIOS
#endif

#ifdef ATARI_ST
# include <osbind.h>

# define UDELAY_FACTOR		125
#endif

#undef	DEBUG
#define NAP_FACTOR		3275

/* Terminal Definitions. */
#define TWIDTH	80
#define TLENGTH 25

#if defined(ANSI)
#define HOME		"\033[H"
#define CLEARS  	"\033[2J"
#define INTOREV		"\033[7m"
#define OUTAREV 	"\033[0m"
#endif

#if defined(VT52) || defined(ATARI_ST)
#define HOME		"\033H"
#define CLEARS  	"\033E"
#define INTOREV		"\033p"
#define OUTAREV 	"\033q"
#define CURSOROFF 	"\033f"
#define CURSORON 	"\033e"
#endif

#define ESC		'\033'



#define WIDTH           38      /* Actual number of COLUMNS on your */
                                /* terminal - 2 (80 / 2 - 2)        */
#define HEIGHT          23      /* Actual number of ROWS on your    */
                                /* terminal - 2                     */
#if defined(ATARI_ST)
#define INT_UP          '8'     /* Initial key for UP    movement */
#define INT_DOWN        '2'     /*    "     "   "  DOWN     "     */
#define INT_LEFT        '4'     /*    "     "   "  LEFT     "     */
#define INT_RIGHT       '6'     /*    "     "   "  RIGHT    "     */

#else

#define INT_UP          'w'     /* Initial key for UP    movement */
#define INT_DOWN        'z'     /*    "     "   "  DOWN     "     */
#define INT_LEFT        'a'     /*    "     "   "  LEFT     "     */
#define INT_RIGHT       's'     /*    "     "   "  RIGHT    "     */
#endif

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

extern int QuixCaptured;
extern int times;
extern int sparxnum;
extern int quixnum;
extern int menleft;
extern int area_left;
extern int last_killed;
extern int percent;
extern int fildes;
extern int startdir;
extern int siz;
extern int maxarea;
extern int board[MAX_X][MAX_Y];
extern int xmax;
extern int ymax;
extern int fuse;
extern int bord_min;
extern int bord_max;
extern int line_min;
extern int line_max;
extern int lastx;
extern int lasty;
extern int bonus_men;
extern int C_UP;
extern int C_DOWN;
extern int C_LEFT;
extern int C_RIGHT;
extern int inchar;
extern unsigned score;

extern int     fuse_lit;
extern int     nohighscore;
extern struct  playertype      player;
extern struct  sparxtype       sparx[MAXSPARX];
extern struct  quixtype        quix[MAXQUIX];
extern struct  scoretype       h_score[10];
extern struct  coord           boundary[BOUNDARY_LEN];
extern struct  coord           temp[BOUNDARY_LEN];

extern void help(void);
extern void init_io(void);
extern void fini_io(void);
extern void set_io(int wait);
extern void change_keys(void);



/* End of defintions */
