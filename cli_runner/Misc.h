// Misculanous things for the runner

#include <cstdint>
#include <string>

/**
 * Try to parse a size string like "960x540".  Upon success, returns `true`
 * `false` if not. parsed size will be put into `x` and `y`.  Size is at minimum (if
 * sussfull parsing) is guarentteed to be 1x1
 */
bool try_parse_size_string(const std::string &s, uint16_t &x, uint16_t &y);
