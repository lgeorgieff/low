#pragma once

#include "http_config.hpp"
#include "mongodb_config.hpp"

#include <string>
#include <cstdint>
#include <chrono>

namespace low {
namespace config {

class config_factory {
public:
  virtual void load() = 0;
  virtual const http_config &http_config() const = 0;
  virtual const mongodb_config &mongodb_config() const = 0;

 protected:
  static void set_net_docker_name(class http_config &config, const std::string &value);
  static void set_net_external_address(class http_config &config, const std::string &value);
  static void set_net_external_port(class http_config &config, const uint16_t &value);
  static void set_net_docker_port(class http_config &config, const uint16_t &value);
  static void set_net_docker_nw_name(class http_config &config, const std::string &value);
  static void set_resources_http_threads(class http_config &config, const uint8_t &value);
  static void set_resources_http_idle_timeout_ms(class http_config &config,
                                                 const std::chrono::milliseconds &value);
  static void set_compression_enabled(class http_config &config, const bool &value);
  static void set_compression_minimum_size_bytes(class http_config &config, const uint32_t &value);
  static void set_compression_level(class http_config &config, const int8_t &value);
  static void set_compression_types(class http_config &config, const std::set<std::string> &value);

  static void set_db_name(class mongodb_config config, const std::string &value);
  static void set_net_docker_name(class mongodb_config config, const std::string &value);
  static void set_net_external_port(class mongodb_config config, const uint16_t &value);
  static void set_net_docker_port(class mongodb_config config, const uint16_t &value);
  static void set_net_docker_nw_name(class mongodb_config config, const std::string &value);
}; // class config_factory

} // namespace config
} // namespace low
