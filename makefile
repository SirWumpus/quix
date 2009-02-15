.POSIX:

.SUFFIXES :
O = .o
E = 
.SUFFIXES : .c $O $E

CFLAGS = -g
LDFLAGS = 

H = defs.h
SRC = quix.c qarea.c qio.c qputs.c qmoves.c
OBJ = quix$O qarea$O qio$O qputs$O qmoves$O

.MAIN : build

build: quix$E

clean: 
	-rm -f ${OBJ}

distclean: clean
	-rm -f quix$E

quix$E: ${OBJ}
	${CC} ${LDFLAGS} -o quix$E ${OBJ}

quix$O : defs.h quix.c
	${CC} ${CFLAGS} -c quix.c

qarea$O : defs.h qarea.c
	${CC} ${CFLAGS} -c qarea.c

qio$O : defs.h qio.c
	${CC} ${CFLAGS} -c qio.c

qputs$O : defs.h qputs.c
	${CC} ${CFLAGS} -c qputs.c

qmoves$O : defs.h qmoves.c
	${CC} ${CFLAGS} -c qmoves.c
