#include "Misc.hpp"
#include <stdexcept>
using namespace std;


bool try_parse_size_string(const string &s, uint16_t &x, uint16_t &y)
{
    // First see if we can split it
    size_t pivot = s.find('x');
    if (pivot == string::npos)
    {
        // Check if capital `X` was provided
        pivot = s.find('X');
        if (pivot == string::npos)
            return false;
    }

    // split
    const string w = s.substr(0, pivot);
    const string h = s.substr(pivot + 1, s.length());

    // Try to parse
    try
    {
        x = static_cast<uint16_t>(stoi(w));
        y = static_cast<uint16_t>(stoi(h));
    }
    catch (const invalid_argument &err)
    {
        return false;
    }

    // Make sure we have at least 1x1 (and return success)
    constexpr uint16_t one = 1;
    x = std::max(one, x);
    y = std::max(one, y);
    return true;
}
