#include "json_reader.hpp"
#include "config_error.hpp"
#include "http_config.hpp"
#include "mongodb_config.hpp"

#include <sys/stat.h>

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <set>
#include <chrono>

using std::string;
using std::ifstream;
using std::cerr;
using std::endl;
using std::set;
using Json::Value;
using std::chrono::milliseconds;

namespace {

bool path_exists(const string &path) {
  struct stat buffer;
  return stat(path.c_str(), &buffer) == 0;
}

bool path_is_file(const string & path) {
  struct stat buffer;
  stat(path.c_str(), &buffer);
  return S_ISREG(buffer.st_mode);
}

Value load_json_file(const string &file_path) {
  if(!path_exists(file_path)) {
    throw low::config::config_error("File \"" + file_path + "\" not found");
  } else if (!path_is_file(file_path)) {
    throw low::config::config_error("Path \"" + file_path + "\" must be refer to a file");
  }

  Json::CharReaderBuilder builder;
  Value root;
  string errors;
  ifstream json_file_stream(file_path, std::ifstream::binary);
  if (!Json::parseFromStream(builder, json_file_stream, &root, &errors))
    throw low::config::config_error("Could not load configuration from \"" + file_path + "\"\n" +
                                    errors);
  return root;
}

bool get_object(const Value &owner, const string &field_name, Value &result) {
  if(owner.isMember(field_name) && owner[field_name].isObject()) {
    result = owner[field_name];
    return true;
  }
  return false;
}

bool get_string(const Value &owner, const string &field_name, string &result) {
  if(owner.isMember(field_name) && owner[field_name].isString()) {
    result = owner[field_name].asString();
    return true;
  } else if(owner.isMember(field_name) && owner[field_name].isNull()) {
    result = "";
    return true;
  }
  return false;
}

bool get_uint8_t(const Value &owner, const string &field_name, uint8_t &result) {
  if (owner.isMember(field_name) && owner[field_name].isUInt()) {
    result = owner[field_name].asUInt();
    return true;
  }
  return false;
}

bool get_int8_t(const Value &owner, const string &field_name, int8_t &result) {
  if (owner.isMember(field_name) && owner[field_name].isInt()) {
    result = owner[field_name].asInt();
    return true;
  }
  return false;
}

bool get_uint16_t(const Value &owner, const string &field_name, uint16_t &result) {
  if (owner.isMember(field_name) && owner[field_name].isUInt()) {
    result = owner[field_name].asUInt();
    return true;
  }
  return false;
}

bool get_uint32_t(const Value &owner, const string &field_name, uint32_t &result) {
  if (owner.isMember(field_name) && owner[field_name].isUInt()) {
    result = owner[field_name].asUInt();
    return true;
  }
  return false;
}

bool get_uint64_t(const Value &owner, const string &field_name, uint64_t &result) {
  if (owner.isMember(field_name) && owner[field_name].isUInt()) {
    result = owner[field_name].asUInt64();
    return true;
  }
  return false;
}

bool get_bool(const Value &owner, const string &field_name, bool &result) {
  if(owner.isMember(field_name) && owner[field_name].isBool()) {
    result = owner[field_name].asBool();
    return true;
  }
  return false;
}

bool get_strset(const Value &owner, const string &field_name, set<string> &result) {
  if (owner.isMember(field_name) && owner[field_name].isArray()) {
    set<string> tmp;
    for(int i{0}; i < owner[field_name].size(); ++i) {
      if(!owner[field_name][i].isString()) return false;
      tmp.insert(owner[field_name][i].asString());
    }

    result = std::move(tmp);
    return true;
  } else if (owner.isMember(field_name) && owner[field_name].isNull()) {
    result = set<string>();
  }
  return false;
}

}  // anonymous namespace

namespace low {
namespace config {

json_reader::json_reader(const string &json_file_path)
    : http_config_{nullptr}, mongodb_config_{nullptr}, json_file_path_{json_file_path} {}

const http_config &json_reader::http_config() const {
  if (!http_config_) {
    throw config_error(
        "json_reader::load() must be called before calling json_reader::http_config()");
  }
  return *http_config_;
}

const mongodb_config &json_reader::mongodb_config() const {
  if (!http_config_) {
    throw config_error(
        "json_reader::load() must be called before calling json_reader::mongodb_config()");
  }
  return *mongodb_config_;
}

void json_reader::load() {
  Value root(load_json_file(json_file_path_));

  if (!root.isObject()) throw config_error("Config root must be a json object");

  Value mongodb;
  if (!get_object(root, "mongodb", mongodb))
    throw config_error("Config root must include an object called \"mongodb\"");
  string mongodb_db_name;
  if (!get_string(mongodb, "db_name", mongodb_db_name))
    throw config_error("mongodb must include a string called \"db_name\"");
  Value mongodb_net;
  if (!get_object(mongodb, "net", mongodb_net))
    throw config_error("mongodb must include an object called \"net\"");
  string mongodb_net_docker_name;
  if (!get_string(mongodb_net, "docker_name", mongodb_net_docker_name))
    throw config_error("mongodb/net must include a string called \"docker_name\"");
  uint16_t mongodb_net_docker_port;
  if (!get_uint16_t(mongodb_net, "docker_port", mongodb_net_docker_port))
    throw config_error("mongodb/net must include a number called \"docker_port\"");
  uint16_t mongodb_net_external_port;
  if (!get_uint16_t(mongodb_net, "external_port", mongodb_net_external_port))
    throw config_error("mongodb/net must include a number called \"external_port\"");
  string mongodb_net_docker_nw_name;
  if (!get_string(mongodb_net, "docker_nw_name", mongodb_net_docker_nw_name))
    throw config_error("mongodb/net must include a string called \"docker_nw_name\"");

  Value http;
  if (!get_object(root, "http", http))
    throw config_error("Config root must include an object called \"http\"");
  Value http_net;
  if (!get_object(http, "net", http_net))
    throw config_error("http must include an object called \"net\"");
  string http_net_docker_name;
  if (!get_string(http_net, "docker_name", http_net_docker_name))
    throw config_error("http/net must include a string called \"docker_name\"");
  uint16_t http_net_docker_port;
  if (!get_uint16_t(http_net, "docker_port", http_net_docker_port))
    throw config_error("http/net must include a number called \"docker_port\"");
  uint16_t http_net_external_port;
  if (!get_uint16_t(http_net, "external_port", http_net_external_port))
    throw config_error("http/net must include a number called \"external_port\"");
  string http_net_external_address;
  if (!get_string(http_net, "external_address", http_net_external_address))
    throw config_error("http/net must include a string called \"external_address\"");
  string http_net_docker_nw_name;
  if (!get_string(http_net, "docker_nw_name", http_net_docker_nw_name))
    throw config_error("http/net must include a string called \"docker_nw_name\"");
  Value http_resources;
  if (!get_object(http, "resources", http_resources))
    throw config_error("http must include an object called \"resources\"");
  uint8_t http_resources_http_threads;
  if (!get_uint8_t(http_resources, "http_threads", http_resources_http_threads))
    throw config_error("http/resources must include a number called \"http_threads\"");
  uint64_t http_resources_http_idle_timeout_ms;
  if (!get_uint64_t(http_resources, "http_idle_timeout_ms", http_resources_http_idle_timeout_ms))
    throw config_error("http/resources must include a number called \"http_idle_timeout_ms\"");
  Value http_ccompression;
  if (!get_object(http, "http_content_compression", http_ccompression))
    throw config_error("http must include an object called \"http_content_compression\"");
  bool http_ccompression_enabled;
  if (!get_bool(http_ccompression, "enabled", http_ccompression_enabled))
    throw config_error("http/resources must include a boolean called \"enabled\"");
  uint32_t http_ccompression_minimum_size_bytes;
  if (!get_uint32_t(http_ccompression, "minimum_size_bytes", http_ccompression_minimum_size_bytes))
    throw config_error("http/resources must include a number called \"minimum_size_bytes\"");
  int8_t http_ccompression_level;
  if (!get_int8_t(http_ccompression, "level", http_ccompression_level))
    throw config_error("http/resources must include a number called \"level\"");
  set<string> http_ccompression_types;
  if (!get_strset(http_ccompression, "types", http_ccompression_types))
    throw config_error("http/resources must include an array of numbers called \"types\"");
  Value http_logging;
  if (!get_object(http, "logging", http_logging))
    throw config_error("http must include an object called \"logging\"");
  bool http_logging_log_to_stderr;
  if (!get_bool(http_logging, "log_to_stderr", http_logging_log_to_stderr))
    throw config_error("http/logging must include a boolean called \"log_to_stderr\"");
  string http_logging_log_dir;
  if (!get_string(http_logging, "log_dir", http_logging_log_dir))
    throw config_error("http/logging must include a string called \"log_dir\"");

  http_config_ = new class http_config;
  set_net_docker_name(*http_config_, http_net_docker_name);
  set_net_external_address(*http_config_, http_net_external_address);
  set_net_external_port(*http_config_, http_net_external_port);
  set_net_docker_port(*http_config_, http_net_docker_port);
  set_net_docker_nw_name(*http_config_, http_net_docker_nw_name);
  set_resources_http_threads(*http_config_, http_resources_http_threads);
  set_resources_http_idle_timeout_ms(*http_config_,
                                     milliseconds(http_resources_http_idle_timeout_ms));
  set_compression_enabled(*http_config_, http_ccompression_enabled);
  set_compression_minimum_size_bytes(*http_config_, http_ccompression_minimum_size_bytes);
  set_compression_level(*http_config_, http_ccompression_level);
  set_compression_types(*http_config_, http_ccompression_types);
  set_logging_log_to_stderr(*http_config_, http_logging_log_to_stderr);
  set_logging_log_dir(*http_config_, http_logging_log_dir);

  mongodb_config_ = new class mongodb_config;
  set_db_name(*mongodb_config_, mongodb_db_name);
  set_net_docker_name(*mongodb_config_, mongodb_net_docker_name);
  set_net_docker_nw_name(*mongodb_config_, mongodb_net_docker_nw_name);
  set_net_external_port(*mongodb_config_, mongodb_net_external_port);
  set_net_docker_port(*mongodb_config_, mongodb_net_docker_port);
}

json_reader::~json_reader() {
  if(http_config_) delete http_config_;
  if(mongodb_config_) delete mongodb_config_;
}

}  // namespace config
}  // namespace low
