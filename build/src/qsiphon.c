/* qsiphon.c: This source file composes most of the qsiphon logic */

/* $Id:$ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pcre.h>

#include "qsiphon.h"
#include "qdir.h"
#include "qfileio.h"
#include "qregex.h"

int
qsiphon(const char *regex, int seconds, const char *indir, const char *outdir, unsigned int opts)
{
    pcre        *pe;
    pcre_extra  *pex;
    int         out_created = 0,
                is_a_match = 0;
    DIR         *mqdir;
    const char  *qfpath;         /* This is the fully qualified path of the qf file */
    char        *qfdir,        /* This is the directory the source qf file lives in */
                *qffile,                              /* This is the qf file's name */
                *dffile;                              /* This is the df file's name */
    ino_t       in_ino;
    dev_t       in_dev;
    FILE        *fp;
    int         wipeout = 0;  /* Wipeout mode unlinks qf/df pairs instead of moving */

    /* Compile the match regex passed from the cmdline */
    qregex_setup(regex, &pe, &pex);

    if (qdir_is_devnull(outdir)) {
        ++wipeout;
    }

    if (!wipeout) 
    {
        /* Sanity check the input directory first before we create the output directory */
        in_ino = qdir_inode(indir);
        in_dev = qdir_device(indir);

        /* Make sure our output directory exists and has same modes as input queue */
        if (qdir_ensure_dir(outdir, &out_created, opts & OPT_DEBUG_MESSAGES) < 0) {
            /* A descriptive message will already print to stderr */
            fprintf(stderr, "[-] Exiting.\n");
            exit(-1);
        }

        /* Sanity check that the output directory isn't the same as the input directory */
        if (in_ino == qdir_inode(outdir)) {
            fprintf(stderr, "[-] Error: input directory and output directory must be different.\n");
            exit(-1);
        }

        /* Since files are being mv'd quickly the directories should be on the same filesystem */
        if (in_dev != qdir_device(outdir)) {
            fprintf(stderr,
                "[-] Error: input directory and output directory need to be on the same filesystem.\n");
            if(out_created) {
                fprintf(stderr, "[-] Removing created directory:[%s]\n", outdir);
                (void)rmdir(outdir);
            }
            exit(-1);
        }

        /* Dupicate directory meta data (ownerships, modes, etc) */
        if (qdir_copy_dirmeta(indir, outdir) < 0) {
            fprintf(stderr, "[-] Exiting.\n");
            exit(-1);
        }
    }

    /* Process the input queue */
    mqdir = qdir_opendir(indir);
    while ((qfpath = qdir_nextqf(mqdir, &qfdir, &qffile))) {

        /* Print a little debug message if applic. */
        if (opts & OPT_DEBUG_MESSAGES)
            fprintf(stdout, "[+] <debug> Found qf file:[%s]\n", qfpath);

        /* Initialize the match each time */
        is_a_match = 0;

        /* Obtain an exclusive lock on each qf file */
        if (!(fp = qlockf(qfpath, 0))) {
            fprintf(stdout, "[-] QF file [%s] locked by another process, skipping...\n", qfpath);
            /* The file is already closed */
            continue;
        }

        /* Make sure there's a QF/DF pair, also set the name of the dffile */
        if (!(qfdf_exists(qfdir, qffile, &dffile))) {
            int clobber = opts & OPT_U_UNLINK_UNMATCHED;
            fprintf(stdout, "[-] QF file [%s] has no matching DF file, %s...\n",
                qfpath, (clobber) ? "removing" : "skipping");
            if (clobber) {
                unlink(qfpath);
            }
            if(dffile)free((void*)dffile);
            qunlockf(fp);
            fclose(fp);
            continue;
        }

        /* If "seconds" is given, determine the age of the qf file and act accordingly */
        if (seconds && qdir_age_seconds(qfpath, seconds)) {
            is_a_match = 1;
        }
        /* Otherwise determine a match the hard way */
        else {
            /* See if it matches against the regex */
            is_a_match = qregex_match(fp, pe, pex);
        }

        if (is_a_match) {
            if (opts & OPT_DEBUG_MESSAGES)
                fprintf(stdout, "[+] <debug> Matched: qf file:[%s].\n", qfpath);

            /* Move matched items to the output dir */
            if (opts & OPT_M_MOVE_MATCHING) {
                if (wipeout) {
                    fprintf(stdout, "[+] Deleting MATCHED pairs [%s/%s]\n",
                        qffile, dffile);
                    qdir_unlink(indir, dffile);
                    qdir_unlink(indir, qffile);
                }
                else {
                    fprintf(stdout, "[+] Moving MATCHED pairs [%s/%s] to %s\n",
                        qffile, dffile, outdir);
                    qdir_move(indir, dffile, outdir);
                    qdir_move(indir, qffile, outdir);
                }
            }
        }
        else {
            /* Move non-matched to the output dir */
            if (!(opts & OPT_M_MOVE_MATCHING)) {
                if (wipeout) {
                    fprintf(stdout, "[+] Deleting UNMATCHED pairs [%s/%s]\n",
                        qffile, dffile);
                    qdir_unlink(indir, dffile);
                    qdir_unlink(indir, qffile);
                }
                else {
                    fprintf(stdout, "[+] Moving UNMATCHED pairs [%s/%s] to %s\n",
                        qffile, dffile, outdir);
                    qdir_move(indir, dffile, outdir);
                    qdir_move(indir, qffile, outdir);
                }
            }
        }

        /* Release the lock and close the qf file */
        if(dffile)free((void*)dffile);
        qunlockf(fp);
        fclose(fp);
    }

    qdir_closedir(mqdir);

    /* RETURN */
    free((void*)pe);
    return 0;
}

