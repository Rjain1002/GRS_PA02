#This file is created using chatGPT
import matplotlib.pyplot as plt

# ==========================================
# HARDCODED VALUES FROM EXPERIMENT (PA02)
# Generated automatically from measurements.csv
# ==========================================

data = {
    'A1': [
        {'Size': 1024, 'Threads': 1, 'Tpt': 7.351, 'Lat': 1.09, 'Cycles': 18083700574, 'L1': 213389861 },
        {'Size': 1024, 'Threads': 2, 'Tpt': 13.992, 'Lat': 1.13, 'Cycles': 34694549562, 'L1': 380964317 },
        {'Size': 1024, 'Threads': 4, 'Tpt': 14.8363, 'Lat': 2.16, 'Cycles': 36786220933, 'L1': 429507170 },
        {'Size': 1024, 'Threads': 8, 'Tpt': 14.7561, 'Lat': 4.34, 'Cycles': 36539261112, 'L1': 413985833 },
        {'Size': 4096, 'Threads': 1, 'Tpt': 20.2203, 'Lat': 1.58, 'Cycles': 18140285363, 'L1': 630741915 },
        {'Size': 4096, 'Threads': 2, 'Tpt': 38.4473, 'Lat': 1.67, 'Cycles': 34676704974, 'L1': 1205460989 },
        {'Size': 4096, 'Threads': 4, 'Tpt': 39.4902, 'Lat': 3.27, 'Cycles': 36428459579, 'L1': 1201133247 },
        {'Size': 4096, 'Threads': 8, 'Tpt': 38.6762, 'Lat': 6.69, 'Cycles': 37308622232, 'L1': 1154734518 },
        {'Size': 65536, 'Threads': 1, 'Tpt': 36.487, 'Lat': 14.22, 'Cycles': 18118772230, 'L1': 2093498186 },
        {'Size': 65536, 'Threads': 2, 'Tpt': 65.7144, 'Lat': 15.79, 'Cycles': 34406236826, 'L1': 3743602621 },
        {'Size': 65536, 'Threads': 4, 'Tpt': 49.5895, 'Lat': 41.91, 'Cycles': 34959742959, 'L1': 2758378761 },
        {'Size': 65536, 'Threads': 8, 'Tpt': 45.5317, 'Lat': 91.31, 'Cycles': 35994176041, 'L1': 2540235902 },
        {'Size': 1048576, 'Threads': 1, 'Tpt': 42.7131, 'Lat': 194.62, 'Cycles': 18043656731, 'L1': 2353556916 },
        {'Size': 1048576, 'Threads': 2, 'Tpt': 28.8031, 'Lat': 577.45, 'Cycles': 33105195351, 'L1': 1584740031 },
        {'Size': 1048576, 'Threads': 4, 'Tpt': 18.2788, 'Lat': 1818.6, 'Cycles': 42324623007, 'L1': 1016932306 },
        {'Size': 1048576, 'Threads': 8, 'Tpt': 16.9668, 'Lat': 3919.12, 'Cycles': 37678285962, 'L1': 937095631 },
    ],
    'A2': [
        {'Size': 1024, 'Threads': 1, 'Tpt': 5.7079, 'Lat': 1.4, 'Cycles': 17606070085, 'L1': 158968942 },
        {'Size': 1024, 'Threads': 2, 'Tpt': 11.096, 'Lat': 1.44, 'Cycles': 33996584700, 'L1': 326282194 },
        {'Size': 1024, 'Threads': 4, 'Tpt': 12.8595, 'Lat': 2.5, 'Cycles': 39378552342, 'L1': 375790677 },
        {'Size': 1024, 'Threads': 8, 'Tpt': 13.23, 'Lat': 4.87, 'Cycles': 39508370525, 'L1': 343099455 },
        {'Size': 4096, 'Threads': 1, 'Tpt': 17.4305, 'Lat': 1.84, 'Cycles': 17771914985, 'L1': 525718562 },
        {'Size': 4096, 'Threads': 2, 'Tpt': 31.428, 'Lat': 2.04, 'Cycles': 32183233179, 'L1': 934954299 },
        {'Size': 4096, 'Threads': 4, 'Tpt': 33.8931, 'Lat': 3.81, 'Cycles': 35651890009, 'L1': 1011560938 },
        {'Size': 4096, 'Threads': 8, 'Tpt': 34.2098, 'Lat': 7.57, 'Cycles': 36399136035, 'L1': 961576636 },
        {'Size': 65536, 'Threads': 1, 'Tpt': 41.5983, 'Lat': 12.46, 'Cycles': 17463162633, 'L1': 1533493596 },
        {'Size': 65536, 'Threads': 2, 'Tpt': 65.4193, 'Lat': 15.87, 'Cycles': 31626447697, 'L1': 2374315177 },
        {'Size': 65536, 'Threads': 4, 'Tpt': 46.5879, 'Lat': 44.57, 'Cycles': 31232889975, 'L1': 1625648718 },
        {'Size': 65536, 'Threads': 8, 'Tpt': 46.375, 'Lat': 89.58, 'Cycles': 33528715536, 'L1': 1637508187 },
        {'Size': 1048576, 'Threads': 1, 'Tpt': 49.1505, 'Lat': 169.15, 'Cycles': 17236355459, 'L1': 1745254269 },
        {'Size': 1048576, 'Threads': 2, 'Tpt': 53.1787, 'Lat': 312.71, 'Cycles': 32734375111, 'L1': 1886903244 },
        {'Size': 1048576, 'Threads': 4, 'Tpt': 28.7142, 'Lat': 1158.79, 'Cycles': 39252878007, 'L1': 1020566844 },
        {'Size': 1048576, 'Threads': 8, 'Tpt': 26.5684, 'Lat': 2504.1, 'Cycles': 36552812193, 'L1': 930550851 },
    ],
    'A3': [
        {'Size': 1024, 'Threads': 1, 'Tpt': 1.7577, 'Lat': 4.59, 'Cycles': 16851992966, 'L1': 143665960 },
        {'Size': 1024, 'Threads': 2, 'Tpt': 2.9858, 'Lat': 5.32, 'Cycles': 29076798791, 'L1': 264706730 },
        {'Size': 1024, 'Threads': 4, 'Tpt': 4.1272, 'Lat': 7.85, 'Cycles': 43233694060, 'L1': 515423267 },
        {'Size': 1024, 'Threads': 8, 'Tpt': 5.0758, 'Lat': 12.78, 'Cycles': 48470928103, 'L1': 512648045 },
        {'Size': 4096, 'Threads': 1, 'Tpt': 5.8493, 'Lat': 5.53, 'Cycles': 17918751299, 'L1': 268202236 },
        {'Size': 4096, 'Threads': 2, 'Tpt': 10.8902, 'Lat': 5.94, 'Cycles': 33325864774, 'L1': 510411698 },
        {'Size': 4096, 'Threads': 4, 'Tpt': 14.4344, 'Lat': 8.98, 'Cycles': 42481289948, 'L1': 716516474 },
        {'Size': 4096, 'Threads': 8, 'Tpt': 17.1351, 'Lat': 15.14, 'Cycles': 46401104719, 'L1': 738067640 },
        {'Size': 65536, 'Threads': 1, 'Tpt': 29.5257, 'Lat': 17.58, 'Cycles': 17924729548, 'L1': 1044672668 },
        {'Size': 65536, 'Threads': 2, 'Tpt': 55.0233, 'Lat': 18.86, 'Cycles': 33064353633, 'L1': 1914423686 },
        {'Size': 65536, 'Threads': 4, 'Tpt': 45.7253, 'Lat': 45.45, 'Cycles': 30403499192, 'L1': 1457418937 },
        {'Size': 65536, 'Threads': 8, 'Tpt': 41.1642, 'Lat': 100.91, 'Cycles': 33395807962, 'L1': 1346376326 },
        {'Size': 1048576, 'Threads': 1, 'Tpt': 39.5473, 'Lat': 210.25, 'Cycles': 17943234758, 'L1': 1340087258 },
        {'Size': 1048576, 'Threads': 2, 'Tpt': 37.6145, 'Lat': 442.28, 'Cycles': 19886202995, 'L1': 994080220 },
        {'Size': 1048576, 'Threads': 4, 'Tpt': 26.2782, 'Lat': 1265.87, 'Cycles': 26478970320, 'L1': 567348811 },
        {'Size': 1048576, 'Threads': 8, 'Tpt': 25.1172, 'Lat': 2647.04, 'Cycles': 33823341836, 'L1': 682936534 },
    ],
}

# Helper to filter data from the dictionary
def get_series(impl, fixed_key, fixed_val, x_key, y_key):
    series_x = []
    series_y = []
    if impl not in data: return [], []
    
    for d in data[impl]:
        if d[fixed_key] == fixed_val:
            series_x.append(d[x_key])
            series_y.append(d[y_key])
    return series_x, series_y

print("Generating plots from Hardcoded Values...")

# 1. Throughput vs Message Size (Threads=4)
plt.figure(figsize=(10, 6))
fixed_threads = 4
for impl in ['A1', 'A2', 'A3']:
    x, y = get_series(impl, 'Threads', fixed_threads, 'Size', 'Tpt')
    plt.plot(x, y, marker='o', label=f'Impl {impl}')

plt.xscale('log')
plt.xlabel('Message Size (Bytes)')
plt.ylabel('Throughput (Gbps)')
plt.title(f'Throughput vs Message Size (Threads={fixed_threads})')
plt.legend()
plt.grid(True)
plt.savefig('throughput_vs_size.png')
print("Saved throughput_vs_size.png")

# 2. Latency vs Thread Count (Size=65536)
plt.figure(figsize=(10, 6))
fixed_size = 65536
for impl in ['A1', 'A2', 'A3']:
    x, y = get_series(impl, 'Size', fixed_size, 'Threads', 'Lat')
    plt.plot(x, y, marker='s', label=f'Impl {impl}')

plt.xlabel('Thread Count')
plt.ylabel('Latency (us)')
plt.title(f'Latency vs Thread Count (Size={fixed_size})')
plt.legend()
plt.grid(True)
plt.savefig('latency_vs_threads.png')
print("Saved latency_vs_threads.png")

# 3. Cache Misses (L1) vs Message Size (Threads=4)
plt.figure(figsize=(10, 6))
for impl in ['A1', 'A2', 'A3']:
    x, y = get_series(impl, 'Threads', fixed_threads, 'Size', 'L1')
    plt.plot(x, y, marker='^', label=f'Impl {impl}')

plt.xscale('log')
plt.xlabel('Message Size (Bytes)')
plt.ylabel('L1 Cache Misses (Total)')
plt.title(f'L1 Cache Misses vs Message Size (Threads={fixed_threads})')
plt.legend()
plt.grid(True)
plt.savefig('l1_miss_vs_size.png')
print("Saved l1_miss_vs_size.png")

# 4. CPU Cycles per Byte vs Message Size (Threads=4)
plt.figure(figsize=(10, 6))
for impl in ['A1', 'A2', 'A3']:
    x = []
    y = []
    duration = 5.0 # hardcoded duration
    for d in data[impl]:
        if d['Threads'] == fixed_threads:
            x.append(d['Size'])
            # Derived metric calculation using hardcoded values
            total_bytes = d['Tpt'] * 1e9 / 8.0 * duration
            if total_bytes > 0:
                val = d['Cycles'] / total_bytes
            else:
                val = 0
            y.append(val)
            
    plt.plot(x, y, marker='d', label=f'Impl {impl}')

plt.xscale('log')
plt.xlabel('Message Size (Bytes)')
plt.ylabel('CPU Cycles per Byte')
plt.title(f'CPU Efficiency vs Message Size (Threads={fixed_threads})')
plt.legend()
plt.grid(True)
plt.savefig('cycles_per_byte_vs_size.png')
print("Saved cycles_per_byte_vs_size.png")

print("All plots generated.")
