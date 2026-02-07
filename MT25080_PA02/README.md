# Roll No: MT25080

# MT25080 - GRS PA02

## Overview

This package contains the implementation for the Network I/O Primitives Analysis assignment.

## Files

- MT25080_Part_A1_Server.c: Two-Copy Server Implementation
- MT25080_Part_A2_Server.c: One-Copy Server Implementation (Sendmsg + Iovec)
- MT25080_Part_A3_Server.c: Zero-Copy Server Implementation (MSG_ZEROCOPY)
- MT25080_Part_A_Client.c: Generic Client Load Generator
- MT25080_Part_A_Common.h: Shared definitions

Brief: concise experiments comparing Three I/O strategies — A1 (Two-Copy), A2 (One-Copy / scatter-gather), A3 (Zero-Copy). The repo contains implementations, a client driver, hardcoded plotting script, recorded measurements, and the analysis report.

Key files
- `MT25080_Part_A_Client.c` — client/load generator
- `MT25080_Part_A1_Server.c`, `MT25080_Part_A2_Server.c`, `MT25080_Part_A3_Server.c` — server implementations
- `MT25080_Part_A_Common.h` — shared defs
- `MT25080_run_experiments.sh` — automates experiments and writes `MT25080_measurements.csv`
- `MT25080_Part_D_Plots.py` — matplotlib script with hardcoded arrays (used to regenerate report figures)

Quick setup (Ubuntu / WSL)
1. Install tools:
```bash
sudo apt update
sudo apt install build-essential python3-pip linux-tools-common linux-tools-generic linux-tools-$(uname -r)
pip3 install matplotlib
```
2. (Optional) allow perf access for metrics:
```bash
sudo sysctl -w kernel.perf_event_paranoid=-1
```

Build
```bash
make
```

Run experiments (automated)
1. Make executable: `chmod +x MT25080_run_experiments.sh`
2. Edit variables at the top of the script to change `DURATION`, `MSG_SIZES`, `THREADS`, or `IMPLS` if needed.
3. Run:
```bash
./MT25080_run_experiments.sh
```
Output: `MT25080_measurements.csv` (and PNGs if you run the plotting script).

Plotting (assignment rule)
- The plotting script `MT25080_Part_D_Plots.py` contains hardcoded arrays for `A1/A2/A3` and saves the PNGs used in the report. Regenerate with:
```bash
python3 MT25080_Part_D_Plots.py
```

Notes & tips
- Run experiments on an isolated system (use `taskset`) to reduce noise.
- If `perf` is not installed the automation still runs but hardware-counter fields will be zero.
- Scripts/binaries target Linux; use WSL or a VM on Windows.

This produces the `client` and `server` binaries.

## Verify installation

Run the verification script which compiles and runs a short test of each implementation:

```bash
./MT25080_verify_installation.sh
```

This script checks for required tools, builds the code, and runs a quick client/server exchange.

## Interpreting results (summary)

- Small messages (<= few KB): Two-Copy (`A1`) often performs best due to very low per-message overhead.
- Medium messages (~4–64 KB): One-Copy (`A2`) performs well by avoiding the user-space assembly copy while keeping kernel bookkeeping low.
- Very large messages (>= tens of KB to MB): Zero-Copy (`A3`) can provide the best throughput if the VM/NIC overheads are amortized; results depend on kernel/NIC support.

See `MT25080_Part_E_Report.md` for detailed plots, numeric tables, analysis, and recommendations.

## Running the automated experiment script (`MT25080_run_experiments.sh`)

The repository includes `MT25080_run_experiments.sh`, which compiles the code and runs a matrix of experiments (implementations × message sizes × thread counts). Results are written to `MT25080_measurements.csv`.

Steps to run:

1. Make the script executable (if needed):

```bash
chmod +x MT25080_run_experiments.sh
```

2. (Optional) Edit the top of the script to change parameters:

- `DURATION` — seconds per run (default 5).
- `MSG_SIZES` — array of message sizes in bytes.
- `THREADS` — array of thread counts.
- `IMPLS` — implementations to test (`A1`, `A2`, `A3`).

Open `MT25080_run_experiments.sh` and edit those variables directly if you want different values.

3. Run the script:

```bash
./MT25080_run_experiments.sh
```

Notes:

- The script attempts to use `perf` to gather hardware counters; if `perf` is not present the script still runs but `perf` fields will be empty/zero.
- On Windows run this inside WSL (Ubuntu) or a Linux VM — the binaries and scripts target Linux.
- To change port, CPU isolation, or duration for a single run, edit the corresponding variables at the top of the script.

Example: set longer runs (edit `DURATION=30`), then run the script.
