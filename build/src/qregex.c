#include <stdio.h>
#include <string.h>

#include <pcre.h>

#include "qregex.h"

int
qregex_setup(const char *regex, pcre **reptr, pcre_extra **rexptr)
{
    int eoff;
    const char *err;

    *reptr = pcre_compile(regex, 0, &err, &eoff, NULL);
    if (*reptr == NULL) {
        fprintf(stderr, "[-] PCRE compilation failed at offset %d: %s\n",
             eoff, err);
        return -1;
    }

    *rexptr = pcre_study(*reptr, 0, &err);
    return 0;
}

int
qregex_match(FILE *fp, pcre *pe, pcre_extra *pex)
{
    /*
    ** qf files are not usually very long, 1K should cover anything in a
    ** Received: line
    */
    char            qf_buffer[1024];
    register int    match = 0;

    while(fgets(qf_buffer, sizeof(qf_buffer), fp)) {
        match = pcre_exec(
            pe,
            pex,
            qf_buffer,
            (int)strlen(qf_buffer),
            0,
            0,
            NULL,
            0
        );
        /* Return with the answer as quick as possible */
        if (match >= 0)
            return 1;
    }

    return 0;
}

