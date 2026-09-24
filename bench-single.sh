#!/bin/bash


echo $(date)
echo "starting matrix multiply"

mkdir -p data/bench-single

for i in $(seq 100 100 1000)
do
    /usr/bin/time -f "CPU: %P\n" -o data/bench-single/mm-${i}-cpu.out ./bench ${i} ${i} ${i} 0 > data/bench-single/mm-${i}.out 
    pids[${i}]=$!
    echo $(date)
done

echo "waiting for multiplies"
# wait for all pids
for pid in ${pids[*]}; do
    wait $pid
done
echo $(date)

echo $(ps)

#kill $monitor
