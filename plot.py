#!/usr/bin/env python3

import re
from pathlib import Path
import matplotlib.pyplot as plt
import sys

# Regex patterns
TIME_RE = re.compile(r"(\d+)x(\d+)\s+matrices:\s+([\d.]+)\s+seconds")
CPU_RE = re.compile(r"CPU:\s+(\d+)%")
RUN_RE = re.compile(r"mm-(\d+)\.out")


def parse_time_file(filepath):
    """
    Extract execution time from a .out file.
    """
    text = Path(filepath).read_text()

    match = TIME_RE.search(text)
    if not match:
        return None

    return float(match.group(3))


def parse_cpu_file(filepath):
    """
    Extract CPU percentage from a -cpu.out file.
    """
    text = Path(filepath).read_text()

    match = CPU_RE.search(text)
    if not match:
        return None

    return int(match.group(1))


def extract_run_id(filename):
    """
    Extract numeric run id from filename like mm-82.out
    """
    match = RUN_RE.match(filename)
    if not match:
        return None

    return int(match.group(1))


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <results_folder> <output image path>")
        sys.exit(1)

    out_files = sorted(Path(sys.argv[1]).glob("mm-*.out"))

    runs = []

    for out_file in out_files:
        # Skip CPU files
        if out_file.name.endswith("-cpu.out"):
            continue

        run_id = extract_run_id(out_file.name)
        if run_id is None:
            continue

        cpu_file = out_file.with_name(f"mm-{run_id}-cpu.out")

        if not cpu_file.exists():
            print(f"Missing CPU file for run {run_id}")
            continue

        exec_time = parse_time_file(out_file)
        cpu_usage = parse_cpu_file(cpu_file)

        if exec_time is None or cpu_usage is None:
            print(f"Could not parse run {run_id}")
            continue

        runs.append({
            "run_id": run_id,
            "time": exec_time,
            "cpu": cpu_usage
        })

    # Sort by run id
    runs.sort(key=lambda r: r["run_id"])

    run_ids = [r["run_id"] for r in runs]
    times = [r["time"] for r in runs]
    cpus = [r["cpu"] for r in runs]

    # Plot
    fig, ax1 = plt.subplots(figsize=(9, 5))

    ax1.plot(run_ids, times, marker='o')
    ax1.set_xlabel("Run ID")
    ax1.set_ylabel("Execution Time (seconds)")
    ax1.tick_params(axis='y')

    ax2 = ax1.twinx()
    ax2.plot(run_ids, cpus, marker='s', linestyle='--')
    ax2.set_ylabel("CPU Usage (%)")
    ax2.tick_params(axis='y')

    plt.title("Execution Time and CPU Usage per Run")
    plt.tight_layout()
    plt.savefig(sys.argv[2])


if __name__ == "__main__":
    main()
