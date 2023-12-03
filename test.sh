#!/bin/sh
min=1
max=3
for i in $(seq $min $max); do
    stream=/dev/null
    [ $i -eq 1 ] && stream=/dev/stdout
    bin/main $i >$stream
    echo $i/$max;
done
