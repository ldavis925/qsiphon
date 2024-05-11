QSIPHON(1)                  General Commands Manual                 QSIPHON(1)



[1mNAME[0m
       qsiphon  -  a  utility for extracting qf/df files from an active mqueue
       directory

[1mSYNOPSIS[0m
       [1mqsiphon [-MUhd] [-K seconds] <{ -L host [, ...] | -R regex }> indir/ <{[0m
       [1moutdir/ | /dev/null }>[0m


[1mDESCRIPTION[0m
       [1mqsiphon  [22mis a utility that helps manage large volumes of mqueue (qf and
       df) files. This utility is useful when you have a large corpus of  mes‐
       sage queue files and would like to extract a portion of them out of the
       sendmail processing queue ([1m/var/spool/mqueue[22m) reliably  without  inter‐
       fering with other processes working within the same directory.  [1mqsiphon[0m
       accomplishes this by issuing a call to [1mflock(2)[22m.

       [1mqsiphon [22mexamines the contents of each qf file, and based on whether  it
       matched  a  regular  expression  (given  with the [1m-R [22mor [1m-L [22moptions), or
       whether it should be kept based on its age (specified with [1m-K[22m)  it  ei‐
       ther moves the qf (and associated df) files to the output directory, or
       it moves things that [1mDO NOT MATCH [22minto the output  directory.   If  the
       [1moutdir [22mdirectory is [1m/dev/null[22m, or a dev null character device, then the
       action performed is a delete operation rather than moving the file(s).

       Please note that regular  expression  library  in  use  by  [1mqsiphon  [22mis
       [1mpcre(3)[22m,  the  [1mPerl  Compatible Regular Expressions [22mengine. See the man
       page for [1mpcre(3) [22mfor more information on writing PCRE  regular  expres‐
       sions.


[1mOPTIONS[0m
       [1m-L [4m[22m"host1[24m [4m[,[24m [4mhost2[24m [4m...]"[0m
                 The  [1m-L [22mswitch is used when you want to match a list of hosts
                 against SMTP [1mReceived  [22mheader  fields.   The  list  of  hosts
                 should  be  contained within quotes and seperated with commas
                 or spaces.  Using [1m-L [22mautomatically generates an internal reg‐
                 ular  expression  that is used to match against qf files' [1mRe‐[0m
                 [1mceived [22mentries.

                 Note that the use of one of the [1m-L [22mor [1m-R [22mswitches are  manda‐
                 tory and mutually exclusive (you must use one, but not both).

       [1m-R [4m[22m"expression"[0m
                 The  [1m-R  [22moption  does not match against any particular header
                 field like [1m-L [22mdoes, rather it just performs straight  regular
                 expression matches against any portion of the qf file.

                 Be  aware  that  typical  RE special characters (like \s, \w,
                 etc), or any  backslash  quoted  character  for  that  matter
                 should  be written with [1mtwo [22mbackslashes.  This is because the
                 C programming language also makes use of  the  backslash  for
                 quoting  special  characters.   So,  for  instance, to pass a
                 backslash literal you must  use  two  backslashes  (e.g.,  to
                 match one or more [1mspace [22mcharacters you would write \\s+).

       [1m-K [4m[22mseconds[0m
                 Sometimes you may wish to keep or remove queue files that are
                 a particular age.  This option allows the preservation or re‐
                 moval  of  qf/df  file pairs newer (or older) than a specific
                 number of seconds (depending on whether the [1m-M [22moption is used
                 which inverts the default behavior).  This option has the ef‐
                 fect of causing a match in the same sense that the [1m-L[22m, or  [1m-R[0m
                 switches cause a match.

       [1m-M        [22mThe use of [1m-M [22minverts the action associated with the matching
                 logic.  Normally the default behavior is to  move  any  qf/df
                 pair  that  [1mDO NOT [22mmatch into the output directory.  However,
                 if you only wish to move qf/df files that MATCH then this op‐
                 tion should be used.  This is ideal for extracting vital mes‐
                 sages from an active message queue.

       [1m-U        [22mForces the deletion of qf files that do not have  a  matching
                 df file pair.  The default behavior is to preserve them.

       [1m-h        [22mPrint a short form of the command usage.

       [1m-d        [22mPrint some debugging output when running.

       [1minput-directory[0m
                 The input directory must exist and be readable and writable.

       [1moutput-directory[0m
                 The  output  directory need not exist -- it will be automati‐
                 cally created if it's missing. Therefore its parent directory
                 must  be writable by [1mqsiphon[22m. If the output directory is cre‐
                 ated it will be given the same user and group ownerships  and
                 file  modes  as the input directory.  If the output directory
                 is [1m/dev/null [22mthen the action performed on the files  will  be
                 an unlink operation rathern than moving them.


[1mEXAMPLES[0m
       [4mqsiphon[24m  [4m-L[24m  [4m'mx-submission,[24m  [4mmx-feedback'[24m [4m-K[24m [4m3600[24m [4m-U[24m [4m/var/spool/mqueue[0m
       [4m/var/spool/removed[0m

              Move all qf/df files to [1m/var/spool/removed [22mthat do  not  contain
              "mx-submission",  or "mx-feedback" in the mail headers' Received
              lines, or any file older than one hour.

              Delete any qf files that do not have matching df files.

       [4mqsiphon[24m [4m-L[24m [4m'mx-submission,[24m [4mmx-feedback'[24m [4m-K[24m  [4m3600[24m  [4m-M[24m  [4m/var/spool/mqueue[0m
       [4m/var/spool/saved[0m

              Move  all qf/df files to [1m/var/spool/saved [22mthat match "mx-submis‐
              sion", or "mx-feedback" in the mail header Received entries,  or
              any file newer than an hour.

       [4mqsiphon[24m  [4m-R[24m  [4m'^RPFD:.*me@upt\.org'[24m  [4m-M[24m [4m/var/spool/mqueue[24m [4m/var/spool/my-[0m
       [4mmails[0m

              Move messages matching the regular expression given to the spec‐
              ified output directory (Note that the quoting meta-character (\)
              was only used once because it appears in  single  quotes  rather
              than double quotations).

       [4mqsiphon[24m  [4m-U[24m  [4m-M[24m  [4m-R[24m  [4m'^H\?\?Subject:[24m  [4mInvoice#[24m  [4m\d+$'[24m [4m/var/spool/mqueue[0m
       [4m/dev/null[0m

              Delete messages containing a particular string  in  the  Subject
              header line.

[1mSEE ALSO[0m
       [1mpcre(3)[0m

[1mDIAGNOSTICS[0m
       Exit  status  is  0 unless an errors occurred, in which it exits with a
       non-zero status.

[1mAUTHOR[0m
       Lane Davis <qsiphon@upt.org>

[1mUPDATED[0m
       Last updated: Sep 07 2013



                                                                    QSIPHON(1)
