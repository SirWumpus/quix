/*
	qputs.c

	880924	ACH		converted to Atari ST & Acylon C
	891216	ACH		Tidied up code for Sozobon C and general release of source
*/


#include "defs.h"
#include <stdio.h>


/*
	Special version of qputch that check and collects input before displaying
	given character on the screen.
*/
int
qputch(int c)
{
#ifdef ATARI_ST
	if( Cconis() ){
		inchar = Cnecin() & 0177;
	} /* if */
	Cconout( c );
	if( c == '\n' ) Cconout( '\r' );
#endif
#ifdef CPM
	if (bios(2))
		inchar = bios(3) & 0177;
	bios(4,c);
	if (c == '\n') qputch('\r');
#endif
#ifdef TERMIOS
	if (inchar == EOF)
		inchar = getkey();
	fputc(c, stdout);
#endif
} /* qputch */


void
print(const char *s)
{
	for (; *s != '\0'; *s++) qputch(*s);
} /* print */

void
putint(unsigned i, int n)
{
        int    a;
        int    j;
        char            s[20];

        a = 0;
        do
        {
                s[a++] = i % 10 + '0';
                i /= 10;
        }
        while (i > 0);
        for (j = 0; j < n; j++)
                qputch(' ');
        for (j = a - 1; j >= 0; j--)
                qputch(s[j]);
}
