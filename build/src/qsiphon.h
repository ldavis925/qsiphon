#ifndef QREADER_H
#define QREADER_H

extern int qsiphon(const char *regex, int seconds, const char *, const char *, unsigned int opts);

#define OPT_M_MOVE_MATCHING             1
#define OPT_U_UNLINK_UNMATCHED          2
#define OPT_DEBUG_MESSAGES              4

#endif /*QREADER_H*/
