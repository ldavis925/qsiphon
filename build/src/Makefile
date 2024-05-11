CC=gcc
PROG=qsiphon
QLOCK=qlock
SRCS=qmain.c qsiphon.c qdir.c qpath.c qfileio.c qregex.c qstring.c
LSRC=qlock.c qfileio.c
OBJS=$(SRCS:%.c=%.o)
LOBJ=$(LSRC:%.c=%.o)
LIBS=-lpcre
RM=/bin/rm
CFLAGS=-Wall -O2 -I/usr/include/pcre

all: $(PROG) $(QLOCK)

$(PROG): $(OBJS) readme.h
	$(CC) -s $(CFLAGS) $(OBJS) -o $(PROG) $(LIBS)

$(QLOCK): $(LOBJ)
	$(CC) -s $(CFLAGS) $(LOBJ) -o $(QLOCK)

.c.o: $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -f core a.out $(OBJS) $(LOBJ)

realclean: clean
	$(RM) -f $(PROG) $(QLOCK)
