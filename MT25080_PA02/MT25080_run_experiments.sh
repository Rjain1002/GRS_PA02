# This file is created using chatGPT
#!/bin/bash

# Configuration
PORT=8080
DURATION=5 # Seconds per run
CSV_FILE="MT25080_measurements.csv"
MSG_SIZES=(1024 4096 65536 1048576) # 1KB, 4KB, 64KB, 1MB
THREADS=(1 2 4 8)
IMPLS=("A1" "A2" "A3")
BIN_PREFIX="MT25080_Part_"

# Ensure cleanup on exit
cleanup() {
    pkill -f "${BIN_PREFIX}"
    pkill -f "perf"
}
trap cleanup EXIT

# Compile
echo "Compiling..."
make clean > /dev/null
make > /dev/null

# Initialize CSV
echo "Implementation,MessageSize,Threads,Throughput_Gbps,Latency_us,CPU_Cycles,Context_Switches,L1_Misses,LLC_Misses" > $CSV_FILE

for IMPL in "${IMPLS[@]}"; do
    SERVER_BIN="./${BIN_PREFIX}${IMPL}_Server"
    
    for SIZE in "${MSG_SIZES[@]}"; do
        for THREAD in "${THREADS[@]}"; do
            echo "Running: $IMPL, Size=$SIZE, Threads=$THREAD"
            
            PERF_OUT="perf_${IMPL}_${SIZE}_${THREAD}.txt"
            SERVER_LOG="server_${IMPL}_${SIZE}_${THREAD}.log"
            
            # Start Server with perf
            # Use setsid to start in new session to avoid signal issues
            # We explicitly check if perf works, otherwise run without
            if command -v perf >/dev/null; then
                 perf stat -e cycles,context-switches,L1-dcache-load-misses,LLC-load-misses \
                    -o "$PERF_OUT" \
                    $SERVER_BIN $PORT > "$SERVER_LOG" 2>&1 &
            else
                 $SERVER_BIN $PORT > "$SERVER_LOG" 2>&1 &
            fi
            SERVER_PID=$!
            
            sleep 1
            
            # Run Client with Timeout (Safety net: Duration + 5s)
            MAX_TIME=$((DURATION + 5))
            CLIENT_OUT=$( timeout ${MAX_TIME}s ./MT25080_Part_A_Client 127.0.0.1 $PORT $THREAD $SIZE $DURATION )
            RET=$?
            
            # Check for timeout
            if [ $RET -eq 124 ]; then
                echo "    [TIMEOUT] Client hung! Killing..."
                THROUGHPUT="0"
                LATENCY="0"
            else
                # Parse Client Output
                THROUGHPUT=$(echo "$CLIENT_OUT" | grep "Throughput" | awk '{print $2}')
                LATENCY=$(echo "$CLIENT_OUT" | grep "Latency" | awk '{print $2}')
                if [ -z "$THROUGHPUT" ]; then THROUGHPUT="0"; fi
                if [ -z "$LATENCY" ]; then LATENCY="0"; fi
            fi
            
            # Kill Server forcefullly to ensure we move on
            # We used setsid/perf, tracking PID is tricky if double forked
            # So we just kill by name (since we run one at a time)
            pkill -SIGINT -f "${BIN_PREFIX}${IMPL}_Server"
            sleep 1
            pkill -SIGKILL -f "${BIN_PREFIX}${IMPL}_Server"
            
            # Allow perf to write
            wait $SERVER_PID 2>/dev/null
            
            # Parse Perf (Handle missing file if perf failed)
            CYCLES="0"
            SWITCHES="0"
            L1_MISS="0"
            LLC_MISS="0"
            
            if [ -f "$PERF_OUT" ]; then
                CYCLES=$(grep "cycles" "$PERF_OUT" | awk '{print $1}' | tr -d ',')
                SWITCHES=$(grep "context-switches" "$PERF_OUT" | awk '{print $1}' | tr -d ',')
                L1_MISS=$(grep "L1-dcache-load-misses" "$PERF_OUT" | awk '{print $1}' | tr -d ',')
                LLC_MISS=$(grep "LLC-load-misses" "$PERF_OUT" | awk '{print $1}' | tr -d ',')
            fi
            
            # Use 0 if empty
            [ -z "$CYCLES" ] && CYCLES="0"
            [ -z "$SWITCHES" ] && SWITCHES="0"
            [ -z "$L1_MISS" ] && L1_MISS="0"
            [ -z "$LLC_MISS" ] && LLC_MISS="0"
            
            echo "    Result: $THROUGHPUT Gbps, $CYCLES Cycles"
            echo "$IMPL,$SIZE,$THREAD,$THROUGHPUT,$LATENCY,$CYCLES,$SWITCHES,$L1_MISS,$LLC_MISS" >> $CSV_FILE
            
            # Cleanup logs
            rm -f "$PERF_OUT" "$SERVER_LOG"
            
        done
    done
done

echo "Experiments Completed. Results in $CSV_FILE"

