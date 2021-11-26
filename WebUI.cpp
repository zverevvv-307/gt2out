#include "WebUI.h"
#include <iostream>

static httplib::Logger debug_logger = [](const auto &req, auto &res) {
  std::clog << "--> " << req.path << " <-- " /*<< res.body */<< std::endl;
};

bool WebUI::WebWorker::open(const char *intrf, int port, std::optional<httplib::Logger> log_handler) {
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

void WebUI::WebWorker::close() {
  if (this->workthread) {
    this->server->stop();
    this->workthread->join();
    this->workthread.reset();
    std::clog << "WebWorker closed." << std::endl;
  }
}

void WebUI::run(const std::string &intrf, int port_number) {

  for (auto &&l : routes_get) {
    srv.Get(l.first, l.second);
    std::clog << "WebUI register GET handler: " << l.first << std::endl;
  }
  for (auto &&l : routes_get2) {
    srv.Get(l.first, l.second);
    std::clog << "WebUI register GET handler: " << l.first << std::endl;
  }

  for (auto &&l : routes_post) {
    srv.Post(l.first, l.second);
    std::clog << "WebUI register POST handler: " << l.first << std::endl;
  }

  for (auto &&l : routes_post2) {
    srv.Post(l.first, l.second);
    std::clog << "WebUI register POST handler: " << l.first << std::endl;
  }

  fs::create_directories(storage_dir);
  srv.set_mount_point("/downloads", storage_dir.string());
  std::clog << "WebUI mount point /downloads: " << fs::absolute(storage_dir) << std::endl;

  web_worker = std::make_unique<WebWorker>(&srv);
  web_worker->open(intrf.c_str(), port_number);
  web_worker->run();
}

