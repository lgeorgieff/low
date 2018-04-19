#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <chrono>

namespace low {
namespace config {

class config_factory;

class http_config {
 public:
  http_config() = default;
  http_config(const http_config &) = default;
  http_config(http_config &&) = default;
  ~http_config() = default;

  std::string net_docker_name() const;
  std::string net_external_address() const;
  uint16_t net_docker_port() const;
  uint16_t net_external_port() const;
  std::string net_docker_nw_name() const;
  uint8_t resources_http_threads() const;
  std::chrono::milliseconds resources_http_idle_timeout_ms() const;
  bool compression_enabled() const;
  uint32_t compression_minimum_size_bytes() const;
  int8_t compression_level() const;
  std::set<std::string> compression_types() const;

 public:
  const static std::string COMPRESSION_TYPE_APPLICATION_JAVASCRIPT;
  const static std::string COMPRESSION_TYPE_APPLICATION_JSON;
  const static std::string COMPRESSION_TYPE_APPLICATION_X_JAVASCRIPT;
  const static std::string COMPRESSION_TYPE_APPLICATION_XHTML_XML;
  const static std::string COMPRESSION_TYPE_APPLICATION_XML;
  const static std::string COMPRESSION_TYPE_APPLICATION_XML_RSS;
  const static std::string COMPRESSION_TYPE_TEXT_CSS;
  const static std::string COMPRESSION_TYPE_TEXT_HTML;
  const static std::string COMPRESSION_TYPE_TEXT_JAVASCRIPT;
  const static std::string COMPRESSION_TYPE_TEXT_PLAIN;
  const static std::string COMPRESSION_TYPE_TEXT_XML;

 private:
  void net_docker_name(const std::string &value);
  void net_external_address(const std::string &value);
  void net_external_port(const uint16_t &value);
  void net_docker_port(const uint16_t &value);
  void net_docker_nw_name(const std::string &value);
  void resources_http_threads(const uint8_t &value);
  void resources_http_idle_timeout_ms(const std::chrono::milliseconds &value);
  void compression_enabled(const bool &value);
  void compression_minimum_size_bytes(const uint32_t &value);
  void compression_level(const int8_t &value);
  void compression_types(const std::set<std::string> &value);
  friend class config_factory;

 private:
  std::string net_docker_name_;
  std::string net_external_address_;
  uint16_t net_docker_port_;
  uint16_t net_external_port_;
  std::string net_docker_nw_name_;
  uint8_t resources_http_threads_;
  std::chrono::milliseconds resources_http_idle_timeout_ms_;
  bool compression_enabled_;
  uint32_t compression_minimum_size_bytes_;
  int8_t compression_level_;
  std::set<std::string> compression_types_;
};  // class http_config

}  // namespace config
}  // namespace low
