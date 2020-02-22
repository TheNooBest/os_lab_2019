#!/bin/sh

count=0

for i in "$@"
do
  let "count = count + i"
done

args_count="$#"

let "result = count / args_count"

echo "$#"
echo "$result"
