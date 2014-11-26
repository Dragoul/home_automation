#!/usr/bin/python

import os
import time
import sys

if len(sys.argv) < 3:
    print "wakeup.py <time> <retries>"
    print "wakeup.py 10:00:00 10"
    exit

wakeup_time = sys.argv[1]
retries = sys.argv[2]
print ""

wakeup = False
while (1):
    io = os.popen('date | sed "s/.* \\([0-9]\\+:[0-9]\\+:[0-9]\\+\\) .*/\\1/g" | tr -d "\\n"')

    result = io.read()
    print "\033[F" + result

    if not wakeup:
        if wakeup_time in result:
            io = os.popen('/www/myapp/src/light A 0 on ' + retries)
            print io.read()
            io = os.popen('/www/myapp/src/light A 1 on ' + retries)
            print io.read()
            wakeup=True
    else:
        if '00:00:00' in result:
            wakeup=False
    time.sleep(1)





