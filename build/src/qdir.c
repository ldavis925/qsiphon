/* qdir.c: directory routines */

/* $Id:$ */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <paths.h>

/* For the major() and minor() calls */
#if defined(__sgi) || defined(__sun)
#include <sys/mkdev.h>
#endif
#if defined(__hpux)
#include <sys/mknod.h>
#endif

#include "qdir.h"
#include "qpath.h"

/* Generic "stat" routine/wrapper function */
static struct stat *
_qdir_stat(const char *path)
{
    static struct stat  sb;
    int                 sv;

    memset((void *)&sb, 0, sizeof(sb));
    sv = stat(path, &sb);
    return (sv < 0 ? NULL : &sb);
}

ino_t
qdir_inode(const char *dir)
{
    struct stat *sb = _qdir_stat(dir);
    if ((!sb) || (!S_ISDIR(sb->st_mode))) {
        fprintf(stderr, "[-] Error: directory:[%s] doesn't exist or is not a directory.\n", dir);
        exit(-1);
    }
    return sb->st_ino;
}

dev_t
qdir_device(const char *dir)
{
    struct stat *sb = _qdir_stat(dir);
    if ((!sb) || (!S_ISDIR(sb->st_mode))) {
        fprintf(stderr, "[-] Error: directory:[%s] doesn't exist or is not a directory.\n", dir);
        exit(-1);
    }
    return sb->st_dev;
}

int
qdir_ensure_dir(const char *path, int *created, int debug)
{
    struct stat *sb = _qdir_stat(path);

    *created = 0;
    if (!sb) {
        if(errno != ENOENT) {
            fprintf(stderr, "[-] stat dir:[%s] error:[%s]\n", path, strerror(errno));
            return -1;
        }
        else {
            /* We'll copy the perms from the original dir later */
            if (mkdir(path, (mode_t)0700) < 0) {
                fprintf(stderr, "[-] mkdir:[%s] error:[%s]\n", path, strerror(errno));
                return -1;
            }
            else {
                if (debug)
                    fprintf(stdout, "[+] <debug> Created output queue directory: (%s)\n", path);
                *created = 1;
            }
        }
    }
    else {
        if (!S_ISDIR(sb->st_mode)) {
            fprintf(stderr, "[-] Error: file:[%s] is not a directory.\n", path);
            return -1;
        }
    }
    return 0;
}

int
qdir_copy_dirmeta(const char *src, const char *dst)
{
    struct stat *sb = _qdir_stat(src);
    if (!sb) {
        fprintf(stderr, "[-] stat dir:[%s] error:[%s]\n", src, strerror(errno));
        return -1;
    }
    /* Just hope it works */
    chmod(dst, sb->st_mode);
    chown(dst, sb->st_uid, sb->st_gid);
    return 0;
}

/* Keep a static copy of the last opendir() path, so we can prepend it to fully-qualify it later */
static char *_qdir_last_open_path = (char *)NULL;

DIR *
qdir_opendir(const char *path)
{
    DIR *dent;

    dent = opendir(path);
    if(!dent) {
        fprintf(stderr, "[-] opendir:[%s] error:[%s]\n", path, strerror(errno));
        exit(-1);
    }

    _qdir_last_open_path = strdup(path);
    return dent;
}

int
qdir_closedir(DIR *dir)
{
    if (_qdir_last_open_path) {
        (void)free(_qdir_last_open_path);
        _qdir_last_open_path = (char *)NULL;
    }
    return closedir(dir);
}

const char *
qdir_nextqf(DIR *dir, char **direct, char **file)
{
    struct dirent   *de;

    if (( ! dir ) || ( ! _qdir_last_open_path )) {
        return NULL;
    }

    while ( 1 ) {
        de = readdir(dir);

        /* End of list, propagate NULL up the caller chain */
        if (!de)
            return (char *)NULL;

        /* We found a file, return it to the caller */
        if (de->d_name[0] == 'q' && de->d_name[1] == 'f')
            break;
    }

    *direct  = _qdir_last_open_path;
    *file    = de->d_name;

    return qfull_path_s(_qdir_last_open_path, de->d_name);
}

int
qfdf_exists(const char *dir, const char *file, char **dffile)
{
    char *qfdf = strdup(file);
    const char *qfpath = (const char *)NULL;

    if (qfdf) {
        qfdf[0] = 'd';
        if (dffile) {
            *dffile = qfdf;
        }
        if ((qfpath = qfull_path(dir, qfdf))) {
            if ((access(qfpath, F_OK)) >= 0) {
                (void)free((void *)qfpath);
                return 1;
            }
        }
    }

    if (qfpath) (void)free((void *)qfpath);
    return 0;
}

int
qdir_move(const char *indir, const char *infile, const char *outdir)
{
    const char *in_path, *out_path;

    in_path  = qfull_path(indir, infile);
    out_path = qfull_path(outdir, infile);
    if (in_path && out_path) {
        if (rename(in_path, out_path) < 0) {
            fprintf(stderr, "[-] Error moving %s => %s: error:[%s]\n",
                in_path, out_path, strerror(errno));
            return -1;
        }
    }
    return 0;
}

int
qdir_unlink(const char *indir, const char *infile)
{
    const char *in_path;

    in_path  = qfull_path(indir, infile);
    return(unlink(in_path));
}

int
qdir_age_seconds(const char *path, int seconds)
{
    time_t      now, mtime;
    struct stat *sb;
    int         diff;

    now = time(NULL);
    sb = _qdir_stat(path);
    if (!sb)
        return 0;

    mtime = sb->st_mtime;   
    diff = (int)(now - mtime);
    return((diff <= seconds) ? 1 : 0);
}

int
_qdir_major_minor (const char *file, long *major, long *minor)
{
    struct stat *sb = _qdir_stat(file);

    if (!(sb) || !(S_ISCHR(sb->st_mode))) {
        return(-1);
    }

    *major = major(sb->st_rdev);
    *minor = minor(sb->st_rdev);
    return 0;
}

int
qdir_is_devnull (const char *file)
{
    const char *real_device = _PATH_DEVNULL;
    long  rd_maj = 0L, rd_min = 0L;
    long  ud_maj = 0L, ud_min = 0L;

    /* This is the major/minor device of the real /dev/null */
    if (_qdir_major_minor(real_device, &rd_maj, &rd_min) < 0) {
        printf("Warning! _PATH_DEVNULL device '%s' does not exist!\n", real_device);
        return(0);
    }

    /* This is the major/minor of the user device */
    if (_qdir_major_minor(file, &ud_maj, &ud_min) < 0) {
        return(0);
    }

    return ( rd_maj == ud_maj && rd_min == ud_min );
}

/*
	extern int
	main (int argc, char **argv)
	{
	    char *file;

	    if(argc > 1) {
	        file = argv[1];
	        if (qdir_is_devnull(file)) {
	            printf("%s is a /dev/null device.\n", file);
	        }
	        else {
	            printf("%s is NOT a /dev/null device.\n", file);
	        }
	    }
	}
*/
