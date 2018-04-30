#include "http_config.hpp"
#include "config_error.hpp"

#include <cstdint>
#include <string>
#include <set>
#include <chrono>

using std::string;
using std::set;
using std::chrono::milliseconds;

namespace low {
namespace config {

string http_config::net_docker_name() const { return net_docker_name_; }

string http_config::net_external_address() const { return net_external_address_; }

uint16_t http_config::net_docker_port() const { return net_docker_port_; }

uint16_t http_config::net_external_port() const { return net_external_port_; }

string http_config::net_docker_nw_name() const { return net_docker_nw_name_; }

uint8_t http_config::resources_http_threads() const { return resources_http_threads_; }

milliseconds http_config::resources_http_idle_timeout_ms() const {
  return resources_http_idle_timeout_ms_;
}

bool http_config::compression_enabled() const { return compression_enabled_; }

uint32_t http_config::compression_minimum_size_bytes() const {
  return compression_minimum_size_bytes_;
}

int8_t http_config::compression_level() const { return compression_level_; }

set<string> http_config::compression_types() const { return compression_types_; }

bool http_config::log_to_stderr() const { return log_to_stderr_; }

string http_config::log_dir() const {
  return log_dir_;
}

void http_config::net_docker_name(const string &value) {
  if (!value.size()) throw new config_error("net_docker_name must not be empty");
  net_docker_name_ = value;
}

void http_config::net_external_address(const string &value) {
  if (!value.size()) throw new config_error("net_external_address must not be empty");
  net_external_address_ = value;
}

void http_config::net_docker_port(const uint16_t &value) {
  if (!value) throw new config_error("net_docker_port must not be 0");
  net_docker_port_ = value;
}

void http_config::net_external_port(const uint16_t &value) {
  if (!value) throw new config_error("net_external_port must not be 0");
  net_external_port_ = value;
}

void http_config::net_docker_nw_name(const string &value) {
  if (!value.size()) throw new config_error("net_docker_nw_name must not be empty");
  net_docker_nw_name_ = value;
}

void http_config::resources_http_threads(const uint8_t &value) {
  if (!value) throw new config_error("resources_http_threads must not be 0");
  resources_http_threads_ = value;
}

void http_config::resources_http_idle_timeout_ms(const milliseconds &value) {
  resources_http_idle_timeout_ms_ = value;
}

void http_config::compression_enabled(const bool &value) { compression_enabled_ = value; }

void http_config::compression_minimum_size_bytes(const uint32_t &value) {
  compression_minimum_size_bytes_ = value;
}

void http_config::compression_level(const int8_t &value) {
  if (value < -1 || value > 9) {
    throw new config_error("compression_level must be in the range between -1 and 9, but is " +
                           std::to_string(value));
  }
  compression_level_ = value;
}

void http_config::compression_types(const set<string> &value) {
  for (const string &type : value)
    if (!value.size()) throw new config_error("compresison type  must not be empty");
  compression_types_ = value;
}

void http_config::log_to_stderr(const bool &value) { log_to_stderr_ = value; }

void http_config::log_dir(const string &value) {
  log_dir_ = value;
}

const string http_config::COMPRESSION_TYPE_APPLICATION_JAVASCRIPT{"application/javascript"};

const string http_config::COMPRESSION_TYPE_APPLICATION_JSON{"application/json"};

const string http_config::COMPRESSION_TYPE_APPLICATION_X_JAVASCRIPT{"application/x-javascript"};

const string http_config::COMPRESSION_TYPE_APPLICATION_XHTML_XML{"application/xhtml+xml"};

const string http_config::COMPRESSION_TYPE_APPLICATION_XML{"application/xml"};

const string http_config::COMPRESSION_TYPE_APPLICATION_XML_RSS{"application/rss+xml"};

const string http_config::COMPRESSION_TYPE_TEXT_CSS{"text/css"};

const string http_config::COMPRESSION_TYPE_TEXT_HTML{"text/html"};

const string http_config::COMPRESSION_TYPE_TEXT_JAVASCRIPT{"text/javascript"};

const string http_config::COMPRESSION_TYPE_TEXT_PLAIN{"text/plain"};

const string http_config::COMPRESSION_TYPE_TEXT_XML{"text/xml"};

}  // namespace config
}  // namespace low
