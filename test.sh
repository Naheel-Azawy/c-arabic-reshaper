#!/bin/bash
cat test-data.txt | ./arabic-reshape > test-data-reshaped.txt
cat test-data-reshaped.txt | env ARRESHOPT="u" ./arabic-reshape > test-data-unshaped.txt
RES=$(diff test-data.txt test-data-unshaped.txt)
if [ "$RES" = "" ]; then
    echo "Passed test!!!"
else
    echo "Failed :("
fi

