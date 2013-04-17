#!/bin/sh
/bin/ls -1 /home/quix/conquer/lib/exec* |& /bin/grep -v 'No match' | /bin/grep -v unowned | awk -F\. '{ print $2 }'
