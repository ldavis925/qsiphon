/* qmain.c: This holds the main() function and entrypoint into the application */

/* $Id:$ */

/* By Lane Davis <qsiphon@upt.org> */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "qsiphon.h"
#include "qstring.h"
#include "readme.h"

#define QSIPHON_OPTS    "R:L:K:MUhd"
#define QSIPHON_USAGE   "Usage: qsiphon [-MUh] [-K seconds] <{ -L 'host [, ...]' | -R 'regex' }> q-in-dir <{ q-out-dir | /dev/null }>\n"

static void
usage (int long_help, int exit_value)
{
    register int i = 0;
    FILE *p_out;

    if (!long_help)
        p_out = (FILE *)NULL;
    else
        p_out = popen("less", "w");

    if (!p_out) p_out = stderr;

    fprintf(p_out, QSIPHON_USAGE);
    fprintf(p_out, "\n");
    fprintf(p_out, "The q-in-dir directory must exist and be readable and writable.\n");
    fprintf(p_out, "The q-out-dir directory need not necessarily exist; if missing it'll be automatically\n");
    fprintf(p_out, "created.  If it's specified as '/dev/null' then the qf/df file pairs are deleted rather\n");
    fprintf(p_out, "than moved.\n");
    fprintf(p_out, "\n");
    fprintf(p_out, "Options:\n");
    fprintf(p_out, " -h              Show short-form of the command line usage\n");
    fprintf(p_out, " -M              Move matched qf/df pairs to the output mqueue directory (default is non-matched)\n");
    fprintf(p_out, " -U              Unlink qf files that do not have a matching df pair\n");
    fprintf(p_out, " -K sec          Consider any qf file created in the last 'sec' seconds a MATCH\n");
    fprintf(p_out, "One of the following is required:\n");
    fprintf(p_out, " -R 'regex'      Match against strings in the qf file (regex can be perl REs)\n");
    fprintf(p_out, " -L 'h1, h2'     Alt. to -R: auto-creates a qf-specific expression based on comma-separated list.\n");
    fprintf(p_out, "\n");

    if (long_help) {
        fprintf(p_out, "Description:\n");
        for (i = 0; readme[i]; i++)
            fputs(readme[i], p_out);
    }

    if (p_out != stderr)
        pclose(p_out);

    exit(exit_value);

    /* NOTREACHED */
    return;
}

static char *
qhost_build_regex(const char *hosts)
{
    char        *host_regex   = (char *)NULL;
    const char  *regex_prefix = "^(\\s+by\\s+|H\\?\\?Received:\\s+from\\s+).*";
    QSTRBUF     regex_string  = { 0, 0 };
    char        *catbuf[5];

    if (!(hosts && *hosts))
        return (char *)NULL;

    host_regex = (char *)qstr_host_list(hosts);
    catbuf[0]  = (char *)regex_prefix;
    catbuf[1]  = "(";
    catbuf[2]  = host_regex;
    catbuf[3]  = ")";
    catbuf[4]  = (char *)NULL;

    (void) qstr_bufset(catbuf, &regex_string);
    return(regex_string.buf);
}

extern int
main(int argc, char **argv)
{
    int             optval;
    int             seconds = 0;
    int             wants_help = 0;
    unsigned int    opts = (unsigned)0;
    const char      *match_regex = (const char *)NULL;
    extern int      optind, opterr, optopt;

    opterr = 0;
    while ((optval = getopt(argc, argv, QSIPHON_OPTS)) != EOF) {
        switch (optval) {
        case 'd':
            opts |= OPT_DEBUG_MESSAGES;
            break;
        case 'R':
            match_regex = optarg;
            break;
        case 'M':
            opts |= OPT_M_MOVE_MATCHING;
            break;
        case 'U':
            opts |= OPT_U_UNLINK_UNMATCHED;
            break;
        case 'L':
            match_regex = qhost_build_regex(optarg);
            break;
        case 'K':
            seconds = atoi(optarg);
            break;
        case 'h':
        default:
            ++wants_help;
            break;
        }
    }

    argc -= optind;
    argv += optind;

    if (argc < 2 || wants_help || !(match_regex)) {
        usage((wants_help) ? 0 : 1, -1);
    }

    if (match_regex && opts & OPT_DEBUG_MESSAGES)
        fprintf(stdout, "[+] <debug> Using regex:[%s]\n", match_regex);

    qsiphon(match_regex, seconds, argv[0], argv[1], opts);
    exit(0);
}

