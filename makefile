
PROG=braek
OBJS=braek.o

PKGS=cairo cairo-png gtk+-2.0
CFLAGS=`pkg-config --cflags $(PKGS)` -Os
LDFLAGS=`pkg-config --libs $(PKGS)` -s

$(PROG): $(OBJS)

braek.o: braek.c braek.h

clean:
	rm -f $(OBJS) $(PROG)

