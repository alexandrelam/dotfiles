#!/bin/bash

command_exists() {
    command -v "$1" >/dev/null 2>&1
}

check_parallel_package() {
    if ! command_exists parallel; then
        echo "Error: GNU Parallel is not installed."
        echo "Please install GNU Parallel using the following command:"
        echo "brew install parallel"
        exit 1
    fi
}

test_path=$1
nb_test=${2:-100}
concurrency_limit=${3:-30}
logfile="test_logs.txt"

global_start_time=$(date +%s)

if [[ -z $test_path ]]; then
    echo "Error: Test path cannot be empty."
    exit 1
fi

if ((concurrency_limit < 0)); then
    echo "Error: Concurrency limit cannot be negative."
    exit 1
fi

check_parallel_package

run_test() {
    local i=$1
    local test_path=$2
    local logfile=$3

    echo "$logfile, $test_path"

    echo "Test $i: running..."
    start_time=$(date +%s)

    port=$((3000 + i))
    HEADLESS=1 PORT=$port rails test "$test_path" >>"$logfile" 2>&1

    end_time=$(date +%s)
    duration=$((end_time - start_time))
    echo "Test $i: ended in $duration seconds"
}

export -f run_test

seq $nb_test | parallel -j $concurrency_limit run_test {} $test_path $logfile

global_end_time=$(date +%s)
global_duration=$((global_end_time - global_start_time))

echo "Ended in $global_duration seconds"
echo "Logs available in $logfile"
