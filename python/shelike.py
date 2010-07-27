from itertools import izip, imap, count, ifilter
import re
import sys
from functools import update_wrapper
from cStringIO import StringIO
from subprocess import Popen, PIPE
from copy import deepcopy

def shellcmd(cmd):
    return Popen(cmd, shell=True, stdout = PIPE).stdout.xreadlines()

def find(topdir, filepat, processfunc):
    for path, dirlst, filelst in os.walk(top):
        for name in fnmatch.filter(filelst, filepat):
            yield name

class pipeable:
    """This decorator makes a function useable in a pipe line.

    Either a function or a class can be given to the constructor.
    The first argument of the function should be a sequence.

    After decoration, if the function is called with
    1. Required number of arguments, the orginal function is called.
    2. One less number of arguments than required, the arguments are stored for
       use when it is called on the pipe line.
    3. Otherwise, raise TypeError exception."""
    def __init__(self, method):
        self.func = method
        self.args = []
        self.kwds = {}
        self.reqlen = 0
        # Since we need to allow classes to be used in pipe, there are cases that
        # method is not a function.
        if hasattr(self.func, 'func_code'):
            self.reqlen = self.func.func_code.co_argcount
        # makes the wrapper object looks like the wrapped function
        update_wrapper(self, method)

    def __call__(self, *args, **kwds):
        curlen = len(args)
        # An ugly hack to handle classes.
        if curlen == self.reqlen or 0 == self.reqlen:
            return self.func(*args, **kwds)
        elif curlen != self.reqlen - 1:
            raise TypeError('Arguments number wrong.')

        cpy = deepcopy(self)
        cpy.args = args
        cpy.kwds = kwds
        return cpy

    def __ror__(self, iter):
        # I want to put iter as the last argument of a function, then it seems
        # like currying a function when calling the decorated function with less
        # arguments. But *args can only come after normal arguments, so it has to
        # be the first argument in the function.
        return self.func(iter, *self.args, **self.kwds)

@pipeable
def cat(files):
    """Either give a file name or list of file."""
    flst = files
    if not hasattr(files, '__iter__'):
        flst = [files]
    for f in flst:
        with open(f) as s:
            for line in s.xreadlines():
                yield line

@pipeable
def shell(iter, cmd=None):
    """Invoke shell command and integrate it in the pipe line."""
    pipe = Popen(cmd, shell=True, stdin = PIPE, stdout = PIPE)
    return pipe.communicate(''.join(iter))[0].splitlines(True)

@pipeable
def grep(iter, match):
    if callable(match):
        fun = match
    else:
        fun = re.compile(match).match
    return ifilter(fun, iter)

@pipeable
def tr(iter, transform):
    return imap(transform, iter)

class trclass:
    """apply arbitrary transform to each sequence element"""
    def __init__(self, transform):
        self.tr=transform
    def __ror__(self, iter):
        return imap(self.tr, iter)

@pipeable
def printlines(iter, sep = ''):
    sys.stdout.write(sep.join(iter))

@pipeable
def notempty(iter):
    """If the iterable sequence is empty"""
    for i in iter:
        return True
    return False

# those objects transform generator to list, tuple, dict or string
aslist   = pipeable(list)
asdict   = pipeable(dict)
astuple  = pipeable(tuple)
asstring = pipeable(''.join)

# this object transforms seq to tuple sequence
enum = pipeable(lambda input: izip(count(), input))

if __name__ == '__main__':
    #######################
    # example 1: equivalent to shell grep ".*/bin/bash" /etc/passwd
    cat('/etc/passwd') | grep('.*/bin/bash') | printlines('')

    #######################
    # example 2: get a list of int's methods beginning with '__r'
    dir(int) | grep('__r') | aslist

    #######################
    # example 3: useless; returns a dict {0:'l', 1:'a', 2:'m', 3:'b', 4:'d', 5:'a'} 
    'lambda' | enum | asdict

