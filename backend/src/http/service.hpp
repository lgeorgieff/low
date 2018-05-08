#pragma once

#include "../config/http_config.hpp"

#include <proxygen/httpserver/HTTPServer.h>

#include <thread>

namespace low {
namespace http {

template <typename HANDLER_FACTORY>
class service {
 public:
  explicit service(const low::config::http_config &config);
  service(const service &other);
  service(service &&other);
  ~service();

  void start(bool detached_mainloop = true);
  void stop();
  low::config::http_config config() const;
  bool started() const;
  bool stopped() const;

 private:
  proxygen::HTTPServer *server_;
  std::thread *thread_;
  low::config::http_config config_;
  bool started_;
  bool stopped_;
};  // class service

}  // namespace http
} // namespace low

#include "service.cpp"
