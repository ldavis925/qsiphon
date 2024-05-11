QSIPHON(1)                  General Commands Manual                 QSIPHON(1)



NAME
       qsiphon  -  a  utility for extracting qf/df files from an active mqueue
       directory

SYNOPSIS
       qsiphon [-MUhd] [-K seconds] <{ -L host [, ...] | -R regex }> indir/ <{
       outdir/ | /dev/null }>


DESCRIPTION
       qsiphon  is a utility that helps manage large volumes of mqueue (qf and
       df) files. This utility is useful when you have a large corpus of  mes‐
       sage queue files and would like to extract a portion of them out of the
       sendmail processing queue (/var/spool/mqueue) reliably  without  inter‐
       fering with other processes working within the same directory.  qsiphon
       accomplishes this by issuing a call to flock(2).

       qsiphon examines the contents of each qf file, and based on whether  it
       matched  a  regular  expression  (given  with the -R or -L options), or
       whether it should be kept based on its age (specified with -K)  it  ei‐
       ther moves the qf (and associated df) files to the output directory, or
       it moves things that DO NOT MATCH into the output  directory.   If  the
       outdir directory is /dev/null, or a dev null character device, then the
       action performed is a delete operation rather than moving the file(s).

       Please note that regular  expression  library  in  use  by  qsiphon  is
       pcre(3),  the  Perl  Compatible Regular Expressions engine. See the man
       page for pcre(3) for more information on writing PCRE  regular  expres‐
       sions.


OPTIONS
       -L "host1 [, host2 ...]"
                 The  -L switch is used when you want to match a list of hosts
                 against SMTP Received  header  fields.   The  list  of  hosts
                 should  be  contained within quotes and seperated with commas
                 or spaces.  Using -L automatically generates an internal reg‐
                 ular  expression  that is used to match against qf files' Re‐
                 ceived entries.

                 Note that the use of one of the -L or -R switches are  manda‐
                 tory and mutually exclusive (you must use one, but not both).

       -R "expression"
                 The  -R  option  does not match against any particular header
                 field like -L does, rather it just performs straight  regular
                 expression matches against any portion of the qf file.

                 Be  aware  that  typical  RE special characters (like \s, \w,
                 etc), or any  backslash  quoted  character  for  that  matter
                 should  be written with two backslashes.  This is because the
                 C programming language also makes use of  the  backslash  for
                 quoting  special  characters.   So,  for  instance, to pass a
                 backslash literal you must  use  two  backslashes  (e.g.,  to
                 match one or more space characters you would write \\s+).

       -K seconds
                 Sometimes you may wish to keep or remove queue files that are
                 a particular age.  This option allows the preservation or re‐
                 moval  of  qf/df  file pairs newer (or older) than a specific
                 number of seconds (depending on whether the -M option is used
                 which inverts the default behavior).  This option has the ef‐
                 fect of causing a match in the same sense that the -L, or  -R
                 switches cause a match.

       -M        The use of -M inverts the action associated with the matching
                 logic.  Normally the default behavior is to  move  any  qf/df
                 pair  that  DO NOT match into the output directory.  However,
                 if you only wish to move qf/df files that MATCH then this op‐
                 tion should be used.  This is ideal for extracting vital mes‐
                 sages from an active message queue.

       -U        Forces the deletion of qf files that do not have  a  matching
                 df file pair.  The default behavior is to preserve them.

       -h        Print a short form of the command usage.

       -d        Print some debugging output when running.

       input-directory
                 The input directory must exist and be readable and writable.

       output-directory
                 The  output  directory need not exist -- it will be automati‐
                 cally created if it's missing. Therefore its parent directory
                 must  be writable by qsiphon. If the output directory is cre‐
                 ated it will be given the same user and group ownerships  and
                 file  modes  as the input directory.  If the output directory
                 is /dev/null then the action performed on the files  will  be
                 an unlink operation rathern than moving them.



EXAMPLES
       qsiphon  -L  'mx-submission,  mx-feedback' -K 3600 -U /var/spool/mqueue
       /var/spool/removed

              Move all qf/df files to /var/spool/removed that do  not  contain
              "mx-submission",  or "mx-feedback" in the mail headers' Received
              lines, or any file older than one hour.

              Delete any qf files that do not have matching df files.

       qsiphon -L 'mx-submission, mx-feedback' -K  3600  -M  /var/spool/mqueue
       /var/spool/saved

              Move  all qf/df files to /var/spool/saved that match "mx-submis‐
              sion", or "mx-feedback" in the mail header Received entries,  or
              any file newer than an hour.

       qsiphon  -R  '^RPFD:.*me@upt\.org'  -M /var/spool/mqueue /var/spool/my-
       mails

              Move messages matching the regular expression given to the spec‐
              ified output directory (Note that the quoting meta-character (\)
              was only used once because it appears in  single  quotes  rather
              than double quotations).

       qsiphon  -U  -M  -R  '^H\?\?Subject:  Invoice#  \d+$' /var/spool/mqueue
       /dev/null

              Delete messages containing a particular string  in  the  Subject
              header line.

SEE ALSO
       pcre(3)

DIAGNOSTICS
       Exit  status  is  0 unless an errors occurred, in which it exits with a
       non-zero status.

AUTHOR
       Lane Davis <qsiphon@upt.org>

UPDATED
       Last updated: Sep 07 2013



                                                                    QSIPHON(1)
