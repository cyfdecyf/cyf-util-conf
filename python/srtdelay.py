#!/usr/bin/env python

from __future__ import with_statement

import sys
import datetime
from shelike import cat, tr, printlines

usage = '''Usage: srtdelay.py <srt> <time>

time format:

    1     -- 1 millisecond
    1s    -- 1 second
    1s1   -- 1 second and 1 milliseconds
   -1s    -- negative value means to advance 1 second
'''

# TODO When outputing the result to a pipe and it does not read to the end, and
# python will report and IOError for borken pipe.

# TODO tr can't pass additional argument to the transform function, so I have to
# uses global variable. Fix this.
delta = None

def parse_time(timestr):
    h, m, rest = timestr.strip().split(':')
    h = int(h)
    m = int(m)
    if rest.find(',') != -1:
        s, ml = map(int, rest.split(','))
    else:
        ml = 0
        s = int(rest)
    return datetime.timedelta(hours=h, minutes=m, seconds=s, milliseconds=ml)

def format_time(time):
    s = time.__str__() # this would get string like '1:02:03.123456' or '1:02:03'
    if time.microseconds == 0:
        return s + ',000'
    else:
        return '%s,%s' % (s[:-7], s[-6:-3])

def delayline(line):
    i = line.find('-->')
    if i == -1:
        return line
    else:
        start = parse_time(line[:i]) + delta
        end = parse_time(line[(i + 3):]) + delta
        return '%s --> %s\n' % (format_time(start), format_time(end))

def delaysrt(srtfile):
    cat(srtfile) | tr(delayline) | printlines

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print usage
        sys.exit(1)

    # parse delay time, ugly code...
    timespec = sys.argv[2]
    negative = False
    if timespec[0] == '-': # negative delay
        negative = True
        timespec = timespec[1:]

    sec = 0
    mlsec = 0
    tmp = timespec.split('s')
    if len(tmp) == 1:
        mlsec = int(tmp[0])
    else:
        sec = int(tmp[0])
        if tmp[1]:
            mlsec = int(tmp[1])
    delta = datetime.timedelta(seconds=sec, milliseconds=mlsec)
    if negative:
        delta = -delta

    delaysrt(sys.argv[1])
