#ifndef QSTRING_H
#define QSTRING_H

#include <sys/types.h>

typedef struct _strbuf {
        char    *buf;
        size_t  len;
} QSTRBUF;

extern int          qstr_bufset (char **sll, QSTRBUF *sbp);
extern const char   *qstr_host_list  (const char *host_list);

#endif /*QSTRING_H*/
