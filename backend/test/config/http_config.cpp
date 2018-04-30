#include "../../src/config/http_config.hpp"
#include "../../src/config/config_factory.hpp"
#include "../../src/config/mongodb_config.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <cstdint>
#include <set>
#include <string>

using std::set;
using std::string;
using std::chrono::milliseconds;

class http_config_f : public ::testing::Test {
 public:
  class c_factory : public low::config::config_factory {
   public:
    virtual void load(){};
    virtual const low::config::http_config &http_config() const { return http_config_; };
    virtual const low::config::mongodb_config &mongodb_config() const { return mongodb_config_; };
    virtual ~c_factory(){};

    static void set_net_docker_name(low::config::http_config &config, const std::string &value) {
      low::config::config_factory::set_net_docker_name(config, value);
    }

    static void set_net_external_address(low::config::http_config &config,
                                         const std::string &value) {
      low::config::config_factory::set_net_external_address(config, value);
    }

    static void set_net_external_port(low::config::http_config &config, const uint16_t &value) {
      low::config::config_factory::set_net_external_port(config, value);
    }

    static void set_net_docker_port(low::config::http_config &config, const uint16_t &value) {
      low::config::config_factory::set_net_docker_port(config, value);
    }

    static void set_net_docker_nw_name(low::config::http_config &config, const std::string &value) {
      low::config::config_factory::set_net_docker_nw_name(config, value);
    }

    static void set_resources_http_threads(low::config::http_config &config, const uint8_t &value) {
      low::config::config_factory::set_resources_http_threads(config, value);
    }

    static void set_resources_http_idle_timeout_ms(low::config::http_config &config,
                                                   const std::chrono::milliseconds &value) {
      low::config::config_factory::set_resources_http_idle_timeout_ms(config, value);
    }

    static void set_compression_enabled(low::config::http_config &config, const bool &value) {
      low::config::config_factory::set_compression_enabled(config, value);
    }

    static void set_compression_minimum_size_bytes(low::config::http_config &config,
                                                   const uint32_t &value) {
      low::config::config_factory::set_compression_minimum_size_bytes(config, value);
    }

    static void set_compression_level(low::config::http_config &config, const int8_t &value) {
      low::config::config_factory::set_compression_level(config, value);
    }

    static void set_compression_types(low::config::http_config &config,
                                      const std::set<std::string> &value) {
      low::config::config_factory::set_compression_types(config, value);
    }

    static void set_logging_log_to_stderr(low::config::http_config &config, const bool &value) {
      low::config::config_factory::set_logging_log_to_stderr(config, value);
    }

    static void set_logging_log_dir(low::config::http_config &config, const std::string &value) {
      low::config::config_factory::set_logging_log_dir(config, value);
    }

   private:
    low::config::http_config http_config_;
    low::config::mongodb_config mongodb_config_;
  };  // class c_factory

 public:
  http_config_f() : cf_{new c_factory}, http_config_{nullptr} {
    http_config_ = new low::config::http_config{cf_->http_config()};
  }

  ~http_config_f() { TearDown(); }

  void SetUp() {}

  void TearDown() {
    if (cf_) {
      delete cf_;
      cf_ = nullptr;
    }
  }

  low::config::http_config &http_config() { return *http_config_; }

 private:
  low::config::config_factory *cf_;
  low::config::http_config *http_config_;
};  // class http_config_f

TEST_F(http_config_f, compression_constants) {
  using low::config::http_config;
  EXPECT_EQ(string{"application/javascript"}, http_config::COMPRESSION_TYPE_APPLICATION_JAVASCRIPT);
  EXPECT_EQ(string{"application/json"}, http_config::COMPRESSION_TYPE_APPLICATION_JSON);
  EXPECT_EQ(string{"application/x-javascript"},
            http_config::COMPRESSION_TYPE_APPLICATION_X_JAVASCRIPT);
  EXPECT_EQ(string{"application/xhtml+xml"}, http_config::COMPRESSION_TYPE_APPLICATION_XHTML_XML);
  EXPECT_EQ(string{"application/xml"}, http_config::COMPRESSION_TYPE_APPLICATION_XML);
  EXPECT_EQ(string{"application/rss+xml"}, http_config::COMPRESSION_TYPE_APPLICATION_XML_RSS);
  EXPECT_EQ(string{"text/css"}, http_config::COMPRESSION_TYPE_TEXT_CSS);
  EXPECT_EQ(string{"text/html"}, http_config::COMPRESSION_TYPE_TEXT_HTML);
  EXPECT_EQ(string{"text/javascript"}, http_config::COMPRESSION_TYPE_TEXT_JAVASCRIPT);
  EXPECT_EQ(string{"text/plain"}, http_config::COMPRESSION_TYPE_TEXT_PLAIN);
  EXPECT_EQ(string{"text/xml"}, http_config::COMPRESSION_TYPE_TEXT_XML);
}

TEST_F(http_config_f, default_constructor) {
  const low::config::http_config config{};
  EXPECT_EQ(string{""}, config.net_docker_name());
  EXPECT_EQ(string{""}, config.net_external_address());
  EXPECT_EQ(0, config.net_docker_port());
  EXPECT_EQ(0, config.net_external_port());
  EXPECT_EQ(string{""}, config.net_docker_nw_name());
  EXPECT_EQ(0, config.resources_http_threads());
  EXPECT_EQ(milliseconds{0}, config.resources_http_idle_timeout_ms());
  EXPECT_FALSE(config.compression_enabled());
  EXPECT_EQ(0, config.compression_minimum_size_bytes());
  EXPECT_EQ(0, config.compression_level());
  EXPECT_EQ(set<string>(), config.compression_types());
  EXPECT_FALSE(config.log_to_stderr());
  EXPECT_EQ(string{""}, config.log_dir());
}

TEST_F(http_config_f, net_docker_name) {
  const string STR{"abcdef..."};
  c_factory::set_net_docker_name(http_config(), STR);
  EXPECT_EQ(STR, http_config().net_docker_name());
}

TEST_F(http_config_f, net_external_address) {
  const string STR{"0.0.0.0"};
  c_factory::set_net_external_address(http_config(), STR);
  EXPECT_EQ(STR, http_config().net_external_address());
}

TEST_F(http_config_f, net_docker_port) {
  const int16_t PORT{1234};
  c_factory::set_net_docker_port(http_config(), PORT);
  EXPECT_EQ(PORT, http_config().net_docker_port());
}

TEST_F(http_config_f, net_external_port) {
  const int16_t PORT{1234};
  c_factory::set_net_external_port(http_config(), PORT);
  EXPECT_EQ(PORT, http_config().net_external_port());
}

TEST_F(http_config_f, net_docker_nw_name) {
  const string STR{"...xyz"};
  c_factory::set_net_docker_nw_name(http_config(), STR);
  EXPECT_EQ(STR, http_config().net_docker_nw_name());
}

TEST_F(http_config_f, resources_http_threads) {
  const int8_t THREADS{12};
  c_factory::set_resources_http_threads(http_config(), THREADS);
  EXPECT_EQ(THREADS, http_config().resources_http_threads());
}

TEST_F(http_config_f, resources_http_idle_timeout_ms) {
  const milliseconds MS{60000};
  c_factory::set_resources_http_idle_timeout_ms(http_config(), MS);
  EXPECT_EQ(MS, http_config().resources_http_idle_timeout_ms());
}

TEST_F(http_config_f, compression_enabled) {
  const bool COMPRESSION_ENABLED{true};
  c_factory::set_compression_enabled(http_config(), COMPRESSION_ENABLED);
  EXPECT_EQ(COMPRESSION_ENABLED, http_config().compression_enabled());
}

TEST_F(http_config_f, compression_level) {
  const int8_t COMPRESSION_LEVEL{5};
  c_factory::set_compression_level(http_config(), COMPRESSION_LEVEL);
  EXPECT_EQ(COMPRESSION_LEVEL, http_config().compression_level());
}

TEST_F(http_config_f, compression_types) {
  const set<string> COMPRESSION_TYPES{"a", "b", "c", "..."};
  c_factory::set_compression_types(http_config(), COMPRESSION_TYPES);
  EXPECT_EQ(COMPRESSION_TYPES, http_config().compression_types());
}

TEST_F(http_config_f, log_to_stderr) {
  const bool LOG_TO_STDERR{true};
  c_factory::set_logging_log_to_stderr(http_config(), LOG_TO_STDERR);
  EXPECT_EQ(LOG_TO_STDERR, http_config().log_to_stderr());
}

TEST_F(http_config_f, log_dir) {
  const string STR{"/some/dir"};
  c_factory::set_logging_log_dir(http_config(), STR);
  EXPECT_EQ(STR, http_config().log_dir());
}
