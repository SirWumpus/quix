.POSIX:

.SUFFIXES :
O = .o
E =
.SUFFIXES : .c $O $E

CFLAGS =
LDFLAGS =

.c$O :
	${CC} ${CFLAGS} -c $<

.c$E :
	$(CC) $(CFLAGS) $(LDFLAGS) -o $*$E $<

$O$E :
	$(LD) $(LDFLAGS) $(CC_E)$*$E $*$O


H = defs.h
SRC = quix.c qarea.c qio.c qputs.c qmoves.c
OBJ = quix$O qarea$O qio$O qputs$O qmoves$O

.MAIN : build

build: quix$E

clean:
	-rm -f ${OBJ}

distclean: clean
	-rm -f quix$E

tar:
	tar -zcf quix.tar.gz makefile *.h *.c *.md *.png

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

list:
	git ls-tree --full-tree -r --name-only HEAD

