#include "../../src/config/http_config.hpp"
#include "../../src/config/config_factory.hpp"
#include "../../src/config/mongodb_config.hpp"

#include <gtest/gtest.h>
#include <cstdint>
#include <string>

using std::string;

class mongodb_config_f : public ::testing::Test {
 public:
  class c_factory : public low::config::config_factory {
   public:
    virtual void load(){};
    virtual const low::config::http_config &http_config() const { return http_config_; };
    virtual const low::config::mongodb_config &mongodb_config() const { return mongodb_config_; };
    virtual ~c_factory(){};

    static void set_db_name(low::config::mongodb_config &config, const std::string &value) {
      low::config::config_factory::set_db_name(config, value);
    }

    static void set_net_docker_name(low::config::mongodb_config &config, const std::string &value) {
      low::config::config_factory::set_net_docker_name(config, value);
    }

    static void set_net_external_port(low::config::mongodb_config &config, const uint16_t &value) {
      low::config::config_factory::set_net_external_port(config, value);
    }

    static void set_net_docker_port(low::config::mongodb_config &config, const uint16_t &value) {
      low::config::config_factory::set_net_docker_port(config, value);
    }

    static void set_net_docker_nw_name(low::config::mongodb_config &config, const std::string &value) {
      low::config::config_factory::set_net_docker_nw_name(config, value);
    }

   private:
    low::config::http_config http_config_;
    low::config::mongodb_config mongodb_config_;
  };  // class c_factory

 public:
  mongodb_config_f() : cf_{new c_factory}, mongodb_config_{nullptr} {
    mongodb_config_ = new low::config::mongodb_config{cf_->mongodb_config()};
  }

  ~mongodb_config_f() { TearDown(); }

  void SetUp() {}

  void TearDown() {
    if (cf_) {
      delete cf_;
      cf_ = nullptr;
    }
  }

  low::config::mongodb_config &mongodb_config() { return *mongodb_config_; }

 private:
  low::config::config_factory *cf_;
  low::config::mongodb_config *mongodb_config_;
};  // class mongodb_config_f

TEST_F(mongodb_config_f, default_constructor) {
  const low::config::mongodb_config config{};
  EXPECT_EQ(string{""}, config.db_name());
  EXPECT_EQ(string{""}, config.net_docker_name());
  EXPECT_EQ(0, config.net_external_port());
  EXPECT_EQ(0, config.net_docker_port());
  EXPECT_EQ(string{""}, config.net_docker_nw_name());
}

TEST_F(mongodb_config_f, db_name) {
  const string STR{"abcdef..."};
  c_factory::set_db_name(mongodb_config(), STR);
  EXPECT_EQ(STR, mongodb_config().db_name());
}

TEST_F(mongodb_config_f, net_docker_port) {
  const int16_t PORT{1234};
  c_factory::set_net_docker_port(mongodb_config(), PORT);
  EXPECT_EQ(PORT, mongodb_config().net_docker_port());
}

TEST_F(mongodb_config_f, net_docker_name) {
  const string STR{"...xyz"};
  c_factory::set_net_docker_name(mongodb_config(), STR);
  EXPECT_EQ(STR, mongodb_config().net_docker_name());
}

TEST_F(mongodb_config_f, net_docker_nw_name) {
  const string STR{"...xyz"};
  c_factory::set_net_docker_nw_name(mongodb_config(), STR);
  EXPECT_EQ(STR, mongodb_config().net_docker_nw_name());
}

TEST_F(mongodb_config_f, net_external_port) {
  const int16_t PORT{1234};
  c_factory::set_net_external_port(mongodb_config(), PORT);
  EXPECT_EQ(PORT, mongodb_config().net_external_port());
}
