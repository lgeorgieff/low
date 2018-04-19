#include "mongodb_config.hpp"
#include "config_error.hpp"

#include <cstdint>
#include <string>

using std::string;

namespace low {
namespace config {

string mongodb_config::db_name() const { return db_name_; }

string mongodb_config::net_docker_name() const { return net_docker_name_; }

uint16_t mongodb_config::net_external_port() const { return net_external_port_; }

uint16_t mongodb_config::net_docker_port() const { return net_docker_port_; }

string mongodb_config::net_docker_nw_name() const { return net_docker_nw_name_; }

void mongodb_config::db_name(const std::string &value) {
  if (!value.size()) throw new config_error("db_name must not be empty");
  db_name_ = value;
}

void mongodb_config::net_docker_name(const std::string &value) {
  if (!value.size()) throw new config_error("net_docker_name must not be empty");
  net_docker_name_ = value;
}

void mongodb_config::net_external_port(const uint16_t &value) {
  if (!value) throw new config_error("net_external_port must not be 0");
  net_external_port_ = value;
}

void mongodb_config::net_docker_port(const uint16_t &value) {
  if (!value) throw new config_error("net_docker_port must not be 0");
  net_docker_port_ = value;
}

void mongodb_config::net_docker_nw_name(const std::string &value) {
  if (!value.size()) throw new config_error("net_docker_nw_name must not be empty");
  net_docker_nw_name_ = value;
}

}  // namespace config
}  // namespace low
