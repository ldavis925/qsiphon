#ifndef QFILEIO_H
#define QFILEIO_H

#include <stdio.h>

extern FILE *qlockf     (const char *path, int block),
            *qfopen     (const char *path);

extern int  qunlockf    (FILE *fp);

#endif /*QFILEIO_H*/
