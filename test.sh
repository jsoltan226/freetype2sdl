#!/bin/sh
min=1
max=3
for i in $(seq $min $max); do
    echo $i/$max;
    bin/main $i;
done
