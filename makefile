LIB = \lib
CFLAGS = -O
AS = mac

.s.o:
	$(AS) $*.s


H = defs.h
OBJ = quix.o qarea.o qio.o qputs.o qmoves.o

quix.tos: $(OBJ)
	cc $(CFLAGS) -o $@ $(OBJ)
