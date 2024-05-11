#ifndef QDIR_H
#define QDIR_H

#include <sys/types.h>
#include <dirent.h>

extern int          qdir_ensure_dir     (const char *dir, int *created, int debug),
                    qdir_closedir       (DIR *dir),
                    qdir_copy_dirmeta   (const char *from, const char *to),
                    qfdf_exists         (const char *dir, const char *file, char **dffile),
                    qdir_move           (const char *indir, const char *file, const char *outdir),
                    qdir_unlink         (const char *indir, const char *file),
		    qdir_is_devnull     (const char *file),
                    qdir_age_seconds    (const char *path, int seconds);

extern const char   *qdir_nextqf  (DIR *dir, char **direct, char **file);
extern DIR          *qdir_opendir       (const char *path);

extern ino_t        qdir_inode          (const char *dir);
extern dev_t        qdir_device         (const char *dir);

#endif /*QDIR_H*/
