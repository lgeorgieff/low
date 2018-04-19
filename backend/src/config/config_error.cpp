#include "config_error.hpp"

#include <string>

using std::string;

namespace low {
namespace config {

config_error::config_error(const string &message) : message_{message} {}

const char* config_error::what() const noexcept { return message_.c_str(); }

} // namespace config
} // namespace low
