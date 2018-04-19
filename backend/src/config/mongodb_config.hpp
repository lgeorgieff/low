#pragma once

#include <cstdint>
#include <string>

namespace low {
namespace config {

class config_factory;

class mongodb_config {
 public:
  mongodb_config() = default;
  mongodb_config(const mongodb_config &other) = default;
  mongodb_config(mongodb_config &&other) = default;
  ~mongodb_config() = default;

  std::string db_name() const;
  std::string net_docker_name() const;
  uint16_t net_external_port() const;
  uint16_t net_docker_port() const;
  std::string net_docker_nw_name() const;

 private:
  void db_name(const std::string &value);
  void net_docker_name(const std::string &value);
  void net_external_port(const uint16_t &value);
  void net_docker_port(const uint16_t &value);
  void net_docker_nw_name(const std::string &value);
  friend class config_factory;

 private:
  std::string db_name_;
  std::string net_docker_name_;
  uint16_t net_external_port_;
  uint16_t net_docker_port_;
  std::string net_docker_nw_name_;

};  // class mongodb_config
}  // namespace config
}  // namespace low
