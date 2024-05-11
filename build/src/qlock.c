#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "qfileio.h"

extern int
main (int argc, char **argv)
{
    char *file;
    FILE *fp;

    if (argc < 2) {
        fprintf(stderr, "qlock filename\n");
        exit(-1);
    }
    file = strdup(argv[1]);
    fp = qlockf(file, 1);
    if (!fp) {
        fprintf(stderr, "Couldn't obtain exclusive lock on %s\n", file);
        exit(-1);
    }

    fprintf (stdout, "Holding exclusive lock on '%s' -- Press ENTER to release: ", file);

    (void) qunlockf(fp);
    fclose(fp);
    fprintf(stdout, "qlock: Released lock on '%s'\n", file);
    free((void*)file);

    exit(0);
}
