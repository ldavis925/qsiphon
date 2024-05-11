#ifndef QREGEX_H
#define QREGEX_H

extern int  qregex_setup(const char *regex, pcre **reptr, pcre_extra **rexptr),
            qregex_match(FILE *fp, pcre *pe, pcre_extra *pex);

#endif /*QREGEX_H*/
