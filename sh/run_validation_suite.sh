#!/bin/bash

# folders
LOG_PATH=$1

# algorithm configuration
STATE_UPDATE_COUNT=$2
PREDICTION_WINDOW=$3

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <input_log_path> <state_update_count> <prediction_window>"
    exit 1
fi

for dir in $LOG_PATH/*; do
    rcg_csv=$(ls $dir | grep "HELIOS_base" || true)
    if [ ! -s $dir/$rcg_csv ]; then
        echo "File $rcg_csv is empty. Skipping."
        continue
    fi

    mkdir -p $dir/validate
    echo "Running validation for $dir/validate"
    build/validation $dir/$rcg_csv $dir/validate $STATE_UPDATE_COUNT $PREDICTION_WINDOW
done
