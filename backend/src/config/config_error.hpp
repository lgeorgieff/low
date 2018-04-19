#pragma once

#include <string>
#include <exception>

namespace low {
namespace config {

class config_error : public std::exception {
 public:
  config_error(const std::string &mesage);
  virtual const char* what() const noexcept override;

 private:
  std::string message_;
};  // class config_error

}  // namespace config
} // namespace low
