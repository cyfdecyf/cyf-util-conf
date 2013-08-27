#!/bin/sh

find . -type l | (while read FN ; do test -e "$FN" || ls -ld "$FN"; done)

