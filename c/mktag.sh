#/bin/sh

LISTFILE=cscope.files

#find . -maxdepth 1 -type f -regex '.*\.[hc]\(pp\)*' | sed -e 's,^./,,' > $LISTFILE
find . -type f -regex '.*\.[hc]\(pp\)*' | sed -e 's,^./,,' > $LISTFILE
cscope -b $LISTFILE
ctags -L $LISTFILE
