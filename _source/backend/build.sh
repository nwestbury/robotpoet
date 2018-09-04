#!/bin/bash
DIR=$(pwd)
cd /var/www/html/_source/backend/

echo $DIR
echo $(pwd)

g++ -g3  main.cpp getRelatedness.cpp getRhymes.cpp arrayIntersection.cpp -o backend -Wno-unused-result
cp backend /var/www/html/scripts/b

cd $DIR

