#!/bin/bash

cd build

num_runs=5000
prediction_window=("10" "20" "30" "40" "50")
state_update_count=("10" "15" "20" "25" "30")

for window in "${prediction_window[@]}"; do
    for divisor in "${state_update_count[@]}"; do
        echo "Running execution_time $num_runs times for prediction window $window and angle divisor $divisor"
        ./execution_time $num_runs $window $divisor
    done
done
