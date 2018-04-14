#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/HTTPServerOptions.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <cstddef>
#include <chrono>
#include <vector>
#include <set>
#include <string>
#include <iostream>

mongocxx::instance inst{};

static const std::string HTTP_IP_ADDRESS("0.0.0.0");
static const uint16_t HTTP_PORT(80);
static const size_t HTTP_THREADS(8);
static const std::chrono::milliseconds HTTP_IDLE_TIMEOUT(60000);
static const bool HTTP_CONTENT_COMPRESSION(true);
static const uint64_t HTTP_CONTENT_COMPRESSION_MINIMUM_SIZE(1024);
static const int HTTP_CONTENT_COMPRESSION_LEVEL(5);
static const std::set<std::string> HTTP_CONTENT_COMPRESSION_TYPES{
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
    "text/xml"};

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

int main(const int argc, const char **argv) {
  proxygen::HTTPServerOptions options;
  options.threads = HTTP_THREADS;
  options.idleTimeout = HTTP_IDLE_TIMEOUT;
  options.shutdownOn = {SIGINT, SIGTERM};
  options.enableContentCompression = HTTP_CONTENT_COMPRESSION;
  options.contentCompressionMinimumSize = HTTP_CONTENT_COMPRESSION_MINIMUM_SIZE;
  options.contentCompressionLevel = HTTP_CONTENT_COMPRESSION_LEVEL;
  options.handlerFactories =
      proxygen::RequestHandlerChain().addThen<HelloWorldHandlerFactory>().build();

  proxygen::HTTPServer server(std::move(options));
  std::vector<proxygen::HTTPServer::IPConfig> IPs = {
    {folly::SocketAddress(HTTP_IP_ADDRESS, HTTP_PORT, true), proxygen::HTTPServer::Protocol::HTTP}};
  server.bind(IPs);
  std::cout << ">> server.bind(IPs);" << std::endl;

  // Start HTTPServer mainloop in a separate thread
  std::thread t([&] () {
    server.start();
  });
  std::cout << ">> server.start();" << std::endl;

  t.join();

  return 0;
}
