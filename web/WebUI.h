#pragma once

#include <map>
#include <chrono>
#include <filesystem>
#include "WebWorker.h"
#include "http.h"


namespace http = httplib;
namespace fs = std::filesystem;


enum ResponceStatus
{
  //https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
  //https://ru.wikipedia.org/wiki/%D0%A1%D0%BF%D0%B8%D1%81%D0%BE%D0%BA_%D0%BA%D0%BE%D0%B4%D0%BE%D0%B2_%D1%81%D0%BE%D1%81%D1%82%D0%BE%D1%8F%D0%BD%D0%B8%D1%8F_HTTP
  Ok = 200,
  NotFound = 404,
  Im_a_teapot = 418,
  Insufficient_Storage =507,
  Unknown_Error = 520,
  Site_is_frozen = 530,
};


class WebUI {
  static inline http::Server srv;
  static inline std::unique_ptr<WebWorker> web_worker;
  static inline fs::path storage_dir = "storage";

public:
  using GetRawHandler = http::Server::Handler;
  using PostRawHandler = http::Server::HandlerWithContentReader;

  static http::Server& server() {
    return srv;
  }

  static const fs::path& storage_path() { return storage_dir; }

  //main
  static void run(const std::string& intrf, int port_number);
  static void close() { web_worker.release(); }

  using GetHandlerType = std::function<bool(const http::Request&, std::string&)>;
  using PostHandlerType = std::function<bool(const http::Request&, const std::string&)>;

  struct GET_Handler {
    GetHandlerType handler;
    GET_Handler(const GetHandlerType &handler) :
                                                 handler(handler) {
    }

    void operator()(const http::Request &req, http::Response &res) {
      try {
        std::string result;
        if ( handler(req, result) ) {
          res.set_content(result, "application/json");
          res.status = ResponceStatus::Ok;
        } else res.status = ResponceStatus::Im_a_teapot;
      }
      catch(const std::exception &e){
        res.status = ResponceStatus::Unknown_Error;
        std::cerr << "std::exception: " << e.what() << __FUNCTION__ << std::endl;
      }
      catch (...) {
        res.status = ResponceStatus::Unknown_Error;
        std::cerr << "unhandled exception ..." << __FILE__ << std::endl;
      }
    }
  };

  struct POST_Handler {
    PostHandlerType handler;
    POST_Handler(const PostHandlerType &handler) :
                                                   handler(handler) {
    }

    void operator()(const http::Request &req, http::Response &res, const http::ContentReader &content_reader) {
      try{
        std::string content;
        content_reader([&](const char *data, size_t data_length) {
          content.append(data, data_length);
          return true;
        });
        res.status = handler(req, content) ? ResponceStatus::Ok : ResponceStatus::Im_a_teapot;
      }
      catch(const std::exception &e){
        res.status = ResponceStatus::Unknown_Error;
        std::cerr << "std::exception: " << e.what() << __FUNCTION__ << std::endl;
      }
      catch (...) {
        res.status = ResponceStatus::Unknown_Error;
        std::cerr << "unhandled exception ..." << __FILE__ << std::endl;
      }
    }
  };

  struct RegistratorGET {
    RegistratorGET(const std::string &pattern, const GetRawHandler &handler) {
      routes_get.insert_or_assign(pattern, handler);
    }

    RegistratorGET(const std::string &pattern, const GetHandlerType &handler) {
      routes_get2.insert_or_assign(pattern, handler);
    }
  };

  struct RegistratorPOST {
    RegistratorPOST(const std::string &pattern, const PostRawHandler &handler) {
      routes_post.insert_or_assign(pattern, handler);
    }
    RegistratorPOST(const std::string &pattern, const PostHandlerType &handler) {
      routes_post2.insert_or_assign(pattern, handler);
    }
  };

private:
  static inline std::map<const std::string, GetRawHandler> routes_get;
  static inline std::map<const std::string, PostRawHandler> routes_post;
  static inline std::map<const std::string, GET_Handler> routes_get2;
  static inline std::map<const std::string, POST_Handler> routes_post2;
};

