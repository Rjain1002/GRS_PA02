# This file is created using chatGPT
#!/bin/bash

echo "=== GRS PA02: Environment & Correctness Check ==="

# 1. check dependencies
echo "[*] Checking Dependencies..."
MISSING=0
for cmd in gcc make python3 perf; do
    if ! command -v $cmd &> /dev/null; then
        echo "    [X] $cmd not found!"
        MISSING=1
    else
        echo "    [OK] $cmd found"
    fi
done

if ! python3 -c "import matplotlib" &> /dev/null; then
    echo "    [ ] matplotlib not found (Plots won't meet requirements, but code will run)"
    # Not critical for correctness of C code
else
    echo "    [OK] matplotlib found"
fi

if [ $MISSING -eq 1 ]; then
    echo "[-] Some dependencies are missing. Run the installation commands in README.txt"
    # Don't exit, try to build anyway
fi

# 2. Build
echo ""
echo "[*] Compiling..."
make clean > /dev/null
if make > /dev/null; then
    echo "    [OK] Compilation Successful"
else
    echo "    [FAIL] Compilation Failed"
    exit 1
fi

# 3. Functional Tests
echo ""
echo "[*] Running Functional Smoke Tests (1 sec each)..."

PORT=9999
SERVER_IP="127.0.0.1"
IMPLS=("A1" "A2" "A3")
BIN_PREFIX="MT25080_Part_"

FAIL=0

for IMPL in "${IMPLS[@]}"; do
    SERVER_BIN="./${BIN_PREFIX}${IMPL}_Server"
    LOG="test_${IMPL}.log"
    
    # Start Server
    $SERVER_BIN $PORT > $LOG 2>&1 &
    SERVER_PID=$!
    sleep 0.5
    
    # Run Client (1 thread, 4096 bytes, 1 second)
    CLIENT_OUT=$( ./MT25080_Part_A_Client $SERVER_IP $PORT 1 4096 1 )
    
    # Kill Server
    kill $SERVER_PID 2>/dev/null
    wait $SERVER_PID 2>/dev/null
    
    # Check output
    BYTES=$(echo "$CLIENT_OUT" | grep "Throughput" | awk '{print $2}')
    
    # We verify if Throughput > 0
    # awk comparison might be tricky with floats, check if string is not empty and not 0
    if [[ "$BYTES" != "0.0000" && ! -z "$BYTES" ]]; then
        echo "    [OK] Implementation $IMPL: Data Transferred ($BYTES Gbps)"
    else
        echo "    [FAIL] Implementation $IMPL: No Data Transfer detected!"
        FAIL=1
    fi
done

echo ""
if [ $FAIL -eq 0 ]; then
    echo "=== [PASSED] All Correctness Checks Passed ==="
    echo "You can now run: ./run_experiments.sh"
else
    echo "=== [FAILED] Some tests failed ==="
fi
