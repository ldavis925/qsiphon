#ifndef README_H
#define README_H

extern const char *readme[];

const char *readme[] = {
"\n"
"qsiphon is a utility that helps manage large volumes of mqueue (qf and df) files.\n",
"This utility is useful when you have a large corpus of message queue files and would\n",
"like to extract a portion of them out of the sendmail processing queue (e.g., \n",
"/var/spool/mqueue).\n",
"\n",
"This program examines the contents of each qf file, and based on whether it matched a\n",
"regular expression (given with the '-R' or '-L' options), or file age requirements (with\n",
"'-K') it either moves the qf file (and accompanying df file) to the output directory, or\n",
"it moves things that do not match into the output directory.\n",
"\n",
"If the output directory specified on the command line is '/dev/null', then the action\n",
"performed is an unlink operation rather than moving the content.\n",
"\n",
"See the man page for qsiphon (man qsiphon) for more details.\n",
"\n",
NULL,
};

#endif /*README_H*/
