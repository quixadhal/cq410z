#!/bin/csh
/bin/ls -1 /home/stpa91/99meshki/lib/exec* |& /bin/grep -v 'No match' | /bin/grep -v unowned | awk -F\. '{ print $2 }'
