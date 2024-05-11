
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "qstring.h"

int
qstr_bufset (char **sll, QSTRBUF *sbp)
{
        register size_t         tot, slen;
        register char           **sllp, *cp;

        /* Calculate buffer size */
        for (tot = 0,sllp = sll; sllp && *sllp; sllp++)
                tot += strlen (*sllp);

        /* Length is always at least ONE char("") */
        if (++tot > sbp->len) {
                sbp->buf = realloc (sbp->buf, (sbp->len = tot));
                if (!sbp->buf) {
                        perror ("qstr_bufset: realloc");
                        return -1;
                }
        }

        for (sllp = sll,cp = sbp->buf; sllp && *sllp; sllp++) {
                slen = strlen (*sllp);
                strncpy (cp, *sllp, slen);
                cp[slen] = '\0';
                cp += slen;
        }

        cp[0] = '\0';
        return tot;
}

#define MAX_REGEX_ENTS  64

const char *
qstr_host_list(const char *host_list)
{
    char            *host_string,
                    *cp,
                    *hosts[MAX_REGEX_ENTS + 1] = {NULL};
    QSTRBUF         regex_str = { 0, 0 };
    register int    i = 0;

    host_string = strdup(host_list);
    cp = strtok(host_string, " \t\r\n,");
    do {
        if (cp) {
            if (i < MAX_REGEX_ENTS)
                hosts[i++] = strdup(cp);
            if (i < MAX_REGEX_ENTS)
                hosts[i++] = strdup("|");
        }
    } while ((cp = strtok(NULL, " \t\r\n,")));

    if (!(strcmp(hosts[i - 1], "|")))
        i -= 1;

    hosts[i] = (char *)NULL;
    
    (void) qstr_bufset(hosts, &regex_str);

    /* cleanup */
    free((void*)host_string);
    for(i = 0; hosts[i]; i++)
        free((void*)hosts[i]);

    return(regex_str.buf);
}

