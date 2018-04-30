#include "config_factory.hpp"

#include <string>
#include <cstdint>
#include <chrono>

using std::string;
using std::set;
using std::chrono::milliseconds;

namespace low {
namespace config {

config_factory::config_factory() {}

config_factory::~config_factory() {}

void config_factory::set_net_docker_name(class http_config &config, const string &value) {
  config.net_docker_name(value);
}

void config_factory::set_net_external_address(class http_config &config, const string &value) {
  config.net_external_address(value);
}

void config_factory::set_net_external_port(class http_config &config, const uint16_t &value) {
  config.net_external_port(value);
}

void config_factory::set_net_docker_port(class http_config &config, const uint16_t &value) {
  config.net_docker_port(value);
}

void config_factory::set_net_docker_nw_name(class http_config &config, const string &value) {
  config.net_docker_nw_name(value);
}

void config_factory::set_resources_http_threads(class http_config &config, const uint8_t &value) {
  config.resources_http_threads(value);
}

void config_factory::set_resources_http_idle_timeout_ms(class http_config &config,
                                                        const milliseconds &value) {
  config.resources_http_idle_timeout_ms(value);
}

void config_factory::set_compression_enabled(class http_config &config, const bool &value) {
  config.compression_enabled(value);
}

void config_factory::set_compression_minimum_size_bytes(class http_config &config,
                                                        const uint32_t &value) {
  config.compression_minimum_size_bytes(value);
}

void config_factory::set_compression_level(class http_config &config, const int8_t &value) {
  config.compression_level(value);
}

void config_factory::set_compression_types(class http_config &config, const set<string> &value) {
  config.compression_types(value);
}

void config_factory::set_db_name(class mongodb_config &config, const string &value) {
  config.db_name(value);
}

void config_factory::set_net_docker_name(class mongodb_config &config, const string &value) {
  config.net_docker_name(value);
}

void config_factory::set_net_external_port(class mongodb_config &config, const uint16_t &value) {
  config.net_external_port(value);
}

void config_factory::set_net_docker_port(class mongodb_config &config, const uint16_t &value) {
  config.net_docker_port(value);
}

void config_factory::set_net_docker_nw_name(class mongodb_config &config, const string &value) {
  config.net_docker_nw_name(value);
}

void config_factory::set_logging_log_to_stderr(class http_config &config, const bool &value) {
  config.log_to_stderr(value);
}

void config_factory::set_logging_log_dir(class http_config &config, const std::string &value) {
  config.log_dir(value);
}

}  // namespace config
}  // namespace low
