#include "ProgressBar.hpp"
#include <ostream>

using namespace std;


ProgressBar::ProgressBar(const uint64_t total_ticks, const uint16_t bar_width, const string &prefix) :
    _total_ticks(total_ticks),
    _bar_width(static_cast<size_t>(bar_width)),
    _prefix(prefix)
{ }


// NOTE: will not always print, only if there is something to update
//       e.g. results are stored in a cache before printing out.  If
//            nothing is different, then it doesn't do an output
void ProgressBar::display(ostream &out)
{
    const auto time_now = chrono::steady_clock::now();
    const float progress = static_cast<float>(_ticks) / static_cast<float>(_total_ticks);
    const uint16_t pos = static_cast<uint16_t>(progress * static_cast<float>(_bar_width));

    const auto time_elapsed = chrono::duration_cast<chrono::seconds>(time_now - _start_time).count();
    const auto seconds_elapsed = static_cast<uint32_t>(time_elapsed);
    const string time_str = to_string(seconds_elapsed) + "s";

    string buffer(_get_line_width(time_str), _empty_char);

    // Prefix
    size_t cursor = 0;
    if (!_prefix.empty())
    {
        buffer.replace(cursor, _prefix.length(), _prefix);
        cursor += _prefix.length() + 1;
    }

    // Left bar cap
    buffer[cursor] = _left_bracket_char;
    cursor += 1;

    // Fill with the progress
    buffer.replace(cursor, pos, pos, _fill_char);

    // Show `>` for current location
    buffer.replace(cursor + pos, 1, 1, '>');
    cursor += _bar_width;

    // Right bar cap
    buffer[cursor] = _right_bracket_char;
    cursor += 2;

    // Make the percentage
    const string percentage = to_string(static_cast<int>(progress * 100)) + "%";
    const string left_pad = string(4 - percentage.length(), ' ');

    // (Left pad first)
    buffer.replace(cursor, left_pad.length(), left_pad);
    cursor += left_pad.length();

    // Add in the percentage
    buffer.replace(cursor, percentage.length(), percentage);
    cursor += percentage.length();
    cursor += 1;

    // Put in the time
    buffer.replace(cursor, time_str.length(), time_str);

    // First test that the output is different from the cache
    if (_cache.compare(buffer) == 0)
        return;     // It's the same, nothing new to print

    // Different! cache  it and write out
    _cache = buffer;

    // write it to the ostream
    out << "\r" << buffer;      // Carriage return sends the cursor back at the end
    out.flush();
}


void ProgressBar::done(ostream &out)
{
    display(out);
    out << endl;
}


size_t ProgressBar::_get_line_width(const string &time_str) const
{
    size_t prefix_width = 0;
    if (!_prefix.empty())
        prefix_width = _prefix.length() + 1;        // +1 for padding

    const size_t middle = static_cast<size_t>(
        1 +                 // left bracket
        _bar_width +        // bar
        1 +                 // Right bracket
        1 +                 // Padding
        4 +                 // Percentage (left padding with spaces) (eg.g 100%)
        1                   // padding
    );
    const size_t time_width = time_str.length();

    return prefix_width + middle + time_width;
}
