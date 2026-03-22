#!/bin/bash

nms=(20000 40000)
threads=(1 2 4 7 8 16 20 25 30 35 40 45)
NUM_ITER=100

T1_20=0
T1_40=0
result="result.csv"
for_copy="for_copy"
echo "NM,threads,av_time,speedup,effect" > $result
> $for_copy

for nm in ${nms[@]}; do
    best_thread_effect=1
    for thread in ${threads[@]}; do
        
        # определим переданное число потоков
        export OMP_NUM_THREADS=$thread
        file="${nm}_${thread}.txt"
        
        # запускаем код NUM_ITER раз
        for ((iter=0; iter<NUM_ITER; iter++)); do
            stat=$(./build/main ${nm} ${nm} 2>/dev/null)
            echo $stat >> $file
        done
        echo "${nm} [${thread}] - complete"

        # удалим выбросы мехквантильным размахом
        avg=$(sort -n "$file" | awk '
        {
            if ($1 > 0) arr[NR]=$1
        }
        END {
            num=NR
            if (num == 0) { print "0.00001"; exit; }
            
            q1=arr[int(num*0.25)]; q3=arr[int(num*0.75)];
            iqr=q3-q1;
            low=q1-iqr*1.5; high=q3+iqr*1.5;

            sum=0; cnt=0;
            for (i=1; i<=num; i++) {
                if (i in arr && low <= arr[i] && arr[i] <= high) {
                    sum += arr[i];
                    cnt += 1;
                }
            }
            if (cnt == 0) cnt = 1;
            avg = sum / cnt;
            printf "%.4f", avg 
        }')

        speedup="1.00000"
        if [ "$nm" -eq 20000 ]; then
            if [ "$thread" -eq 1 ]; then
                T1_20=$avg
            else
                speedup=$(awk "BEGIN { printf \"%.4f\", $T1_20 / $avg }")
            fi
        elif [ "$nm" -eq 40000 ]; then
            if [ "$thread" -eq 1 ]; then
                T1_40=$avg
            else
                speedup=$(awk "BEGIN { printf \"%.4f\", $T1_40 / $avg }")
            fi
        fi

        rm -f $file
        effect=$(awk "BEGIN { printf \"%.2f\", $speedup / $thread }")
        is_good=$(awk "BEGIN { printf \"%d\", ($effect >= 0.5) ? 1 : 0 }")
        if [ $is_good -eq 1 ]; then
            best_thread_effect=$thread
        fi

        echo "${nm},${thread},${avg},${speedup},${effect}" >> $result
        if [ $thread -ne 1 ]; then
            echo "${speedup}" >> $for_copy
        fi
    done
    echo "" >> $result
    echo "" >> $for_copy
    echo "Best num threads for ${nm}: ${best_thread_effect}" >> $result
    echo "" >> $result
done
echo "Bash completed: result.csv"
