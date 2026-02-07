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
- Makefile: Build script
- run_experiments.sh: Automated experiment script
- measurements.csv: Experimental results (Data Source)
- MT25080_Part_D_Plots.py: Python script to generate plots (Hardcoded values)
- MT25080_Part_E_Report.md: Analysis Report
- verify_installation.sh: Script to verify setup

## Ubuntu Setup Instructions

### 1. Install Dependencies

You will need `gcc`, `make`, `python3`, `matplotlib`, and `perf`.

```bash
sudo apt update
sudo apt install build-essential python3-pip linux-tools-common linux-tools-generic linux-tools-$(uname -r)
pip3 install matplotlib
```

### 2. Configure Perf Permissions

By default, Ubuntu restricts access to performance counters. You can enable them for the session:

```bash
sudo sysctl -w kernel.perf_event_paranoid=-1
```

### 3. Verify Correctness

I have included a script to verify that your environment is set up correctly and the code works:

```bash
./verify_installation.sh
```

This will check dependencies, compile the code, and run a quick "smoke test" on all 3 implementations.

## Building

Run the following command to compile all binaries:

```
make
```

## Running Experiments

To run the automated experiments and generate the CSV:

```
./run_experiments.sh
```

**Note:** The script uses `perf` to collect CPU metrics. If run in a restricted environment (e.g., container without capabilities), `perf` may fail.

## Generating Plots

To generate the plots (PNG) from the hardcoded data:

```
python3 MT25080_Part_D_Plots.py
```

(Requires matplotlib)
