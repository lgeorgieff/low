#include "../config/json_reader.hpp"
#include "http/service.hpp"

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <proxygen/httpserver/HTTPServerOptions.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

using low::http::service;
using low::config::json_reader;

const string CONFIG_FILE_PATH_DEFAULT("../config/main_config.json");
DEFINE_string(config, CONFIG_FILE_PATH_DEFAULT, "Path to configuration file");
const string USAGE_TEXT("low service implementing ...");
const string APPLICATION_VERSION("0.0.1");

mongocxx::instance inst{};

class HelloWorldHandler : public proxygen::RequestHandler {
 public:
  explicit HelloWorldHandler() {}

  void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {}

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept {}

  void onEOM() noexcept {
    mongocxx::client conn{mongocxx::uri{"mongodb://low_mongodb:27017"}};

    using stream_document = bsoncxx::builder::stream::document;
    using document_value = bsoncxx::stdx::optional<bsoncxx::document::value>;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;
    using bsoncxx::builder::stream::open_document;

    stream_document document{};
    auto collection = conn["testdb"]["testcollection"];

    document_value maybe_result = collection.find_one({});
    int64_t counter{1};
    if (maybe_result) {
      counter = maybe_result->view()["counter"].get_int64();
      document << "counter" << counter;
      collection.update_one(
          stream_document{} << "counter" << counter << finalize,
          stream_document{} << "$set" << open_document << "counter" << ++counter
                            << close_document << finalize);
    } else {
      document << "counter" << counter;
      collection.insert_one(document.view());
    }

    proxygen::ResponseBuilder(downstream_)
        .status(200, "OK")
        .body("Hello World! #" + std::to_string(counter))
        .sendWithEOM();
  }

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept {}

  void requestComplete() noexcept {
    delete this;
  }

  void onError(proxygen::ProxygenError err) noexcept { delete this; }
};

class HelloWorldHandlerFactory : public proxygen::RequestHandlerFactory {
public:
  void onServerStart(folly::EventBase *) noexcept override {}

  void onServerStop() noexcept override {}

  proxygen::RequestHandler* onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept override {
    return new HelloWorldHandler();
  }
};

int main(int argc, char **argv) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  // Initialize Google flag library.
  gflags::SetUsageMessage(USAGE_TEXT);
  gflags::SetVersionString(APPLICATION_VERSION);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // Load json configuration for this application.
  json_reader config_reader(FLAGS_config);
  config_reader.load();

  if (config_reader.http_config().log_to_stderr() &&
      !config_reader.http_config().log_dir().empty()) {
    FLAGS_alsologtostderr = true;
    FLAGS_log_dir = config_reader.http_config().log_dir();
  } else {
    if (config_reader.http_config().log_to_stderr()) FLAGS_logtostderr = true;
    if (!config_reader.http_config().log_dir().empty())
      FLAGS_log_dir = config_reader.http_config().log_dir();
  }

  service<HelloWorldHandlerFactory> http_service{config_reader.http_config()};
  http_service.start();
  LOG(INFO) << "server started on " << config_reader.http_config().net_external_address() << ":"
            << config_reader.http_config().net_docker_port();
  http_service.stop();
  LOG(INFO) << "server stopped";

  gflags::ShutDownCommandLineFlags();
  return 0;
}
