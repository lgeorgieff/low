#include "../../src/config/config_error.hpp"
#include "../../src/config/http_config.hpp"
#include "../../src/config/json_reader.hpp"
#include "../../src/config/mongodb_config.hpp"

#include <sys/stat.h>
#include <gtest/gtest.h>
#include <list>
#include <cstdio>
#include <fstream>
#include <chrono>
#include <set>

using std::string;
using std::set;
using std::chrono::milliseconds;
using low::config::json_reader;
using low::config::config_error;
using low::config::http_config;
using low::config::mongodb_config;

const string VALID_CONFIG{R"str(
{
    "mongodb": {
        "db_name": "low",
        "net": {
            "docker_name": "low_mongodb",
            "docker_port": 27017,
            "external_port": 27017,
            "docker_nw_name": "low_mongodb_net"
        }
    },
    "http": {
        "net": {
            "docker_name": "low_http",
            "external_address": "0.0.0.0",
            "external_port": 8080,
            "docker_port": 80,
            "docker_nw_name": "low_mongodb_net"
        },
        "resources": {
            "http_threads": 8,
            "http_idle_timeout_ms": 60000
        },
        "http_content_compression": {
            "enabled": true,
            "minimum_size_bytes": 1024,
            "level": 5,
            "types": [
                "application/javascript",
                "application/json",
                "application/x-javascript",
                "application/xhtml+xml",
                "application/xml",
                "application/xml+rss",
                "text/css",
                "text/html",
                "text/javascript",
                "text/plain",
                "text/xml"
            ]
        },
        "logging": {
            "log_to_stderr": true,
            "log_dir": null
        }
    }
}
)str"};

const set<string> COMPRESSION_TYPES{"application/javascript",
                                    "application/json",
                                    "application/x-javascript",
                                    "application/xhtml+xml",
                                    "application/xml",
                                    "application/xml+rss",
                                    "text/css",
                                    "text/html",
                                    "text/javascript",
                                    "text/plain",
                                    "text/xml"};

class json_reader_f : public ::testing::Test {
public:
 json_reader_f() : CONFIG_PATH{"tmp.conf"} {}

 ~json_reader_f() { TearDown(); }

 void SetUp() {}

 void TearDown() { delete_file(); }

 void create_file(const string &content) {
   if (file_exists_()) delete_file();

   std::ofstream file{CONFIG_PATH.c_str()};
   file << content << std::endl;
 }

 void delete_file() {
   if(!file_exists_()) return;
   //   std::remove(CONFIG_PATH.c_str());
 }

 const string CONFIG_PATH;

private:
 bool file_exists_() {
   struct stat buffer;
   return stat(CONFIG_PATH.c_str(), &buffer) == 0;
 }
}; // class json_reader_f

TEST_F(json_reader_f, constructor_does_not_throw) {
  EXPECT_NO_THROW(json_reader{"does not exist"});
}

TEST_F(json_reader_f, load_throws_if_file_is_missing) {
  json_reader reader{"does not exits"};
  EXPECT_THROW(reader.load(), low::config::config_error);
}

TEST_F(json_reader_f, load_throws_if_json_is_invalid) {
  create_file("invalid json");
  json_reader reader{CONFIG_PATH};
  EXPECT_THROW(reader.load(), low::config::config_error);
}

TEST_F(json_reader_f, config_is_loaded) {
  create_file(VALID_CONFIG);
  json_reader reader{CONFIG_PATH};
  EXPECT_NO_THROW(reader.load());
}

TEST_F(json_reader_f, http_config_is_loaded) {
  create_file(VALID_CONFIG);
  json_reader reader{CONFIG_PATH};
  reader.load();
  http_config config{reader.http_config()};
  EXPECT_EQ(string{"low_http"}, config.net_docker_name());
  EXPECT_EQ(string{"0.0.0.0"}, config.net_external_address());
  EXPECT_EQ(80, config.net_docker_port());
  EXPECT_EQ(8080, config.net_external_port());
  EXPECT_EQ(string{"low_mongodb_net"}, config.net_docker_nw_name());
  EXPECT_EQ(8, config.resources_http_threads());
  EXPECT_EQ(milliseconds{60000}, config.resources_http_idle_timeout_ms());
  EXPECT_TRUE(config.compression_enabled());
  EXPECT_EQ(1024, config.compression_minimum_size_bytes());
  EXPECT_EQ(5, config.compression_level());
  EXPECT_EQ(COMPRESSION_TYPES, config.compression_types());
  EXPECT_TRUE(config.log_to_stderr());
  EXPECT_EQ(string{""}, config.log_dir());
}

TEST_F(json_reader_f, mongodb_config_is_loaded) {
  create_file(VALID_CONFIG);
  json_reader reader{CONFIG_PATH};
  reader.load();
  mongodb_config config{reader.mongodb_config()};
  EXPECT_EQ(string{"low"}, config.db_name());
  EXPECT_EQ(string{"low_mongodb"}, config.net_docker_name());
  EXPECT_EQ(27017, config.net_external_port());
  EXPECT_EQ(27017, config.net_docker_port());
  EXPECT_EQ(string{"low_mongodb_net"}, config.net_docker_nw_name());
}
