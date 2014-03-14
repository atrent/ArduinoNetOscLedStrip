#!/bin/bash

## Course, you need to use the liblo library
## for making this script working ...
## http://liblo.sourceforge.net/

while
 true
do

 LOAD=$(cut -f 1 -d" " /proc/loadavg |cut -f1 -d.)
 DEC=$(cut -f 1 -d" " /proc/loadavg |cut -f2 -d.)
 echo $LOAD $DEC
 let LOAD=(LOAD+1)*60

 ./liblo/src/tools/oscsend 159.149.159.213 57600 /bargraph iiii $DEC $LOAD 0 0
 ## ^^^^^^^^^^^^^ Check correct path here!
 sleep 1
done
