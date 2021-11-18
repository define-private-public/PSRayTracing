#pragma once

#include <string>
#include <chrono>
#include <iosfwd>

// A Simple progress bar for command line interfaces
// Helpful to track render progress and give an estimate to the user
//
// Inspired/based off of // Source: https://github.com/prakhar1989/progress-cpp/tree/7bfba0d22d19c41323aa35541618b6ebec9d737c
//
// TODO [features]:
// - allow users to turn off time keeping and configure
// - percent resolution (right now only shows a hard integer)
// - maybe postfixing as well


class ProgressBar {
private:
    // Data
    uint64_t _ticks = 0;
    std::chrono::steady_clock::time_point _start_time{};

    const uint64_t _total_ticks;
    const size_t _bar_width = 0;
    const std::string _prefix = "";
    const char _fill_char = '=';
    const char _empty_char = ' ';
    const char _left_bracket_char = '[';
    const char _right_bracket_char = ']';

    std::string _cache = "";        // Used to test if we need to actually update stdout

public:

    explicit ProgressBar(const uint64_t total_ticks, const uint16_t bar_width, const std::string &prefix="");

    // Start the process (start time keeping too)
    // Will put  everything on a new line first
    inline void start() {
        _start_time = std::chrono::steady_clock::now();
    }

    // Increment by `ticks` ticks
    inline void inc(const uint64_t ticks=1) {
        _ticks += ticks;
    }

    // Set ticks to specific amount
    inline void set_to(const uint64_t ticks) {
        _ticks = ticks;
    }

    // `out` should most likley be standard output
    void display(std::ostream &out);

    // Used to signify what we're done with using the progress bar
    void done(std::ostream &out);

private:
    // Get the total width (in characters) that the line to show needs to be
    // It factors in things such as padding/room for timing & percentage
    //   seconds -- how many will need to be displayed
    size_t _get_line_width(const std::string &time_str) const;
};
