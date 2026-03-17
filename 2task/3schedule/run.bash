#!/bin/bash

echo "Исследование оптимального schedule"

export OMP_NUM_THREADS=8
N=5000

chunks=(1 4 8 16 32 64 128 256 512)
schedules=(static dynamic guided)

for schedule in ${schedules[@]}; do
    echo ""
    echo "Testing ${schedule} type"

    for chunk in ${chunks[@]}; do
        export OMP_SCHEDULE="${schedule},${chunk}"
        time_per_chunk=$(build/./main ${N})
        echo "${time_per_chunk}"
    done
done