#include "WebWorker.h"
#include <memory>
#include <iostream>


static httplib::Logger debug_logger = [](const auto &req, auto &res) {
  std::clog << "--> " << req.path << " <-- " /*<< res.body */<< std::endl;
};

bool WebWorker::open(const char *intrf, int port, std::optional<httplib::Logger> log_handler) {
  this->server->set_error_handler([](const auto& req, auto &res) {
    std::cerr << "WebWorker: "<<req.method<<" "<<req.path<<" response status: " << res.status << std::endl;
    auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    std::array<char, BUFSIZ> buf { };
    std::snprintf(buf.begin(), buf.size(), fmt, res.status);
    res.set_content(buf.begin(), "text/html");
  });
  this->server->set_logger( log_handler ? *log_handler : debug_logger);

  this->workthread = std::make_unique<std::thread>([this, intrf, port]() {
    this->server->listen(intrf, port);
  });
  std::clog << "WebWorker started on " << intrf << ":" << port << std::endl;
  return true;
}

void WebWorker::close() {
  if (this->workthread) {
    this->server->stop();
    this->workthread->join();
    this->workthread.reset();
    std::clog << "WebWorker closed." << std::endl;
  }
}

