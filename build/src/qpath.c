/* qpath.c: Path related subroutines */

/* $Id:$ */

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "qpath.h"

const char *
qfull_path_s(const char *dir, const char *file)
{
    static char     full_path[PATH_MAX + 1];
    int             spv;

    spv = snprintf(full_path, sizeof(full_path), "%s%s%s",
        dir,
        ( dir[strlen(dir)-1] == '/' ) ? "" : "/",
        file
    );
    if (spv >= sizeof(full_path))
        fprintf(stderr, "[-] qfull_path_s: Warning: file:[%s] exceeds max path, its name may be truncated.\n", file);

    return full_path;
}

const char *
qfull_path(const char *dir, const char *file)
{
    char            full_path[PATH_MAX + 1];
    int             spv;

    spv = snprintf(full_path, sizeof(full_path), "%s%s%s",
        dir,
        ( dir[strlen(dir)-1] == '/' ) ? "" : "/",
        file
    );
    if (spv >= sizeof(full_path))
        fprintf(stderr, "[-] qfull_path: Warning: file:[%s] exceeds max path, its name may be truncated.\n", file);

    return (const char *)strdup(full_path);
}

