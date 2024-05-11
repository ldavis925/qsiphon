/* qfileio.c: General file I/O routines */

/* $Id:$ */

#include <stdio.h>
#include <sys/file.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "qfileio.h"

FILE *
qlockf(const char *path, int block)
{
    FILE *fp = qfopen(path);
    int  ops = LOCK_EX;
    int   ql = 0;
    int   fd = 0;

    if (fp) {
        fd = fileno(fp);
        if (!block)
            ops |= LOCK_NB;

        ql = flock(fd, ops);
        if (ql < 0) {
            fclose(fp);
            return NULL;
        }
    }
    return fp;
}

int
qunlockf(FILE *fp)
{
    int fd = fileno(fp);
    return flock(fd, LOCK_UN | LOCK_NB);
}

FILE *
qfopen(const char *path)
{
    FILE *fp = fopen(path, "r+");
    return fp;
}

