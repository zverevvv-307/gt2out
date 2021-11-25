#pragma once

#include <memory>
#include <thread>
#include <optional>
#include "http.h"

class WebWorker {
  std::unique_ptr<std::thread> workthread;
  httplib::Server *server;

public:
  WebWorker(httplib::Server *server) : server(server) {};
  ~WebWorker() { close(); }

  bool open(const char *intrf = "0.0.0.0", int port = 8088,
            std::optional<httplib::Logger> log_handler = std::nullopt);
  void close();
  void run() {
    this->workthread->join();
  }

};

