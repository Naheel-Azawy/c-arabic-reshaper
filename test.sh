#!/bin/bash
T="test-data$1"
cat "$T.txt" | ./arabic-reshape > "$T-reshaped.txt"
cat "$T-reshaped.txt" | env ARRESHOPT="u" ./arabic-reshape > "$T-unshaped.txt"
RES=$(diff "$T.txt" "$T-unshaped.txt")
if [ "$RES" = "" ]; then
    echo "Passed test!!!"
else
    echo "Failed :("
fi

