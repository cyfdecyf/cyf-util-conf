#/bin/sh

LISTFILE=cscope.files

find . -maxdepth 1 -type f -regex .*\.[hc] | sed -e 's,^./,,' > $LISTFILE
cscope -b $LISTFILE
ctags -L $LISTFILE
