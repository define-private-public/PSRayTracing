#!/usr/bin/env python
#
# Small utlity to read in two of our CSV `results.csv` files, and take the best run from each,
# and then place it into a resulting file

import sys


def get_ns(line: str) -> int:
    # Gets the nanoseconds a test case took
    return int(line.split(',')[8])


def get_best_row(row_a: str, row_b: str) -> str:
    # Compares two (raw string) rows and gets the best runtime
    runtime_a = get_ns(row_a)
    runtime_b = get_ns(row_b)

    if (runtime_a < runtime_b):
        return row_a
    else:
        return row_b


def get_lines(filename: str) -> list[str]:
    # Gets all the lines from a file and shoves it into a list (without trailing whitespace)
    lines = []
    with open(filename, 'r') as file:
        lines = [line.rstrip() for line in file.readlines()]

    return lines


def make_file_with_best_runtimes(filename_a: str, filename_b: str, results_filename: str) -> None:
    # will open two CSV files, which should be formatted the same, then will use the
    # results from `get_best_row()` and write that data to a new file.

    # Get data
    lines_a = get_lines(filename_a)
    lines_b = get_lines(filename_b)
    lines_best = []

    # Get header
    header = lines_a[0]

    # Pair each row of data
    paired = zip(lines_a[1:], lines_b[1:])

    # Get the best
    lines_best = [get_best_row(a, b) for (a, b) in paired]
    best_runtimes = [get_ns(x) for x in lines_best]
    lines_best = [f'{line}\n' for line in lines_best]           # Append newline
    total_runtime_ns = sum(best_runtimes)

    # write result
    with open(results_filename, 'w') as file:
        file.write(f'{header}\n')
        file.writelines(lines_best)

    print(f'Total Runtime (ns): {total_runtime_ns}')


def main():
    if len(sys.argv) < 3:
        print('Error, need three arguments: <results_a.csv> <results_b.csv> <results_best.csv>')
        sys.exit(1)

    # Grab paths
    results_file_a = sys.argv[1]
    results_file_b = sys.argv[2]
    results_file_combined = sys.argv[3]

    make_file_with_best_runtimes(results_file_a, results_file_b, results_file_combined)


if __name__ == '__main__':
    main()
