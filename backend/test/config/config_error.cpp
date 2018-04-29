#include "../../src/config/config_error.hpp"

#include <gtest/gtest.h>
#include <string>

using low::config::config_error;
using std::string;

TEST(config_error, constructor) {
  config_error{""};
  SUCCEED();
}

TEST(config_error, what_returns_error_message) {
  const string MESSAGE{"error message"};
  config_error err{MESSAGE};
  EXPECT_STREQ(MESSAGE.c_str(), err.what());
}
