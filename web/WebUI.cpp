#include "WebUI.h"
#include <iostream>


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

