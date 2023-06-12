#!/bin/bash

test_path=$1
nb_test=${2:-100}
concurrency_limit=${3:-30}
logfile="test_logs.txt"
counter=0

global_start_time=$(date +%s)

if ((${#test_path} < 1)); then
    echo "Error: Test path cannot be empty."
    exit 1
fi

if ((concurrency_limit < 0)); then
    echo "Error: Concurrency limit cannot be negative."
    exit 1
fi

for ((i = 1; i <= $nb_test; i++)); do
    {
        echo "Test $i: running..."
        start_time=$(date +%s)
        HEADLESS=1 rails test $test_path >>"$logfile" 2>&1
        end_time=$(date +%s)
        duration=$((end_time - start_time))
        echo "Test $i: ended in $duration seconds"
    } &

    ((counter++))

    if ((counter >= concurrency_limit)); then
        wait
        counter=0
    fi
done

wait

global_end_time=$(date +%s)
global_duration=$((global_end_time - global_start_time))

echo "Ended in $global_duration seconds"
echo "Logs available in $logfile"
