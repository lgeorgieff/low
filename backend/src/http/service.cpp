#pragma once

#include "service.hpp"
#include "../config/http_config.hpp"

#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/HTTPServerOptions.h>

#include <stdexcept>
#include <thread>
#include <vector>

using std::runtime_error;
using std::vector;
using std::thread;
using low::config::http_config;

namespace {

  template<typename HANDLER_FACTORY>
  proxygen::HTTPServerOptions options_from_config(http_config config) {
    proxygen::HTTPServerOptions options;
    options.threads = config.resources_http_threads();
    options.idleTimeout = config.resources_http_idle_timeout_ms();
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = config.compression_enabled();
    options.contentCompressionMinimumSize = config.compression_minimum_size_bytes();
    options.contentCompressionLevel = config.compression_level();
    options.handlerFactories = proxygen::RequestHandlerChain().addThen<HANDLER_FACTORY>().build();
    return options;
  }
  
} // anonymous namespace

namespace low {
  namespace http {
  template <typename HANDLER_FACTORY>
  service<HANDLER_FACTORY>::service(const http_config &config)
      : server_{nullptr}, thread_{nullptr}, config_{config}, started_{false}, stopped_{true} {}

  template <typename HANDLER_FACTORY>
  service<HANDLER_FACTORY>::service(const service &other)
      : server_{other.server_},
        thread_{other.thread_},
        config_{other.config_},
        started_{other.started_},
        stopped_{other.stopped_} {}

  template <typename HANDLER_FACTORY>
  service<HANDLER_FACTORY>::service(service &&other)
      : server_{other.server_},
        thread_{other.thread_},
        config_{std::move(other.config_)},
        started_{std::move(other.started_)},
        stopped_{std::move(other.stopped_)} {
    other.server_ = nullptr;
    other.thread_ = nullptr;
  }

  template <typename HANDLER_FACTORY>
  service<HANDLER_FACTORY>::~service() {
    if (started_) stop();
    if (server_) delete server_;
  }

  template <typename HANDLER_FACTORY>
  bool service<HANDLER_FACTORY>::started() const {
    return started_;
  }

  template <typename HANDLER_FACTORY>
  bool service<HANDLER_FACTORY>::stopped() const {
    return stopped_;
  }

  template <typename HANDLER_FACTORY>
  void service<HANDLER_FACTORY>::stop() {
    if (stopped_) throw runtime_error("the service is already stopped");

    server_->stop();
    if (thread_) {
      thread_->join();
      delete thread_;
      thread_ = nullptr;
    }

    stopped_ = true;
    started_ = false;
  }

  template <typename HANDLER_FACTORY>
  void service<HANDLER_FACTORY>::start(bool detached_mainloop) {
    if (started_) throw runtime_error("the service is already running");

    if (!server_) server_ = new proxygen::HTTPServer{options_from_config<HANDLER_FACTORY>(config_)};
    vector<proxygen::HTTPServer::IPConfig> ips = {
        {folly::SocketAddress(config_.net_external_address(), config_.net_docker_port(), true),
         proxygen::HTTPServer::Protocol::HTTP}};
    server_->bind(ips);

    if (detached_mainloop) {
      thread_ = new thread{[&]() { server_->start(); }};
    } else {
      server_->start();
    }

    started_ = true;
    stopped_ = false;
  }

  } // namespace http
} // namespace low
