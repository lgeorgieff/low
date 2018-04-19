#pragma once

#include "config_factory.hpp"
#include "http_config.hpp"
#include "mongodb_config.hpp"

#include <string>

namespace low {
namespace config {

class json_reader : public config_factory {
 public:
  json_reader(const std::string &json_file_path);
  json_reader(const json_reader &other) = default;
  json_reader(json_reader &&other) = default;
  ~json_reader();

  void load() override;
  const class http_config &http_config() const override;
  const class mongodb_config &mongodb_config() const override;

 private:
  class http_config *http_config_;
  class mongodb_config *mongodb_config_;
  std::string json_file_path_;
};  // class config_reader

}  // namespace config
}  // namespace low
