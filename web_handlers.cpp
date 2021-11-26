#include "WebUI.h"
#include "gtclient.h"
#include "gtserver.h"
#include "helpers.h"
#include <ctime>



bool hello(const http::Request& , std::string &result) {
  result = "Hi, user of super AI system!";
  return true;
}

bool client(const http::Request& , std::string &result) {
  std::ostringstream oss;
  time_t nnow; time(&nnow);
  oss<< std::ctime( &nnow )<<std::endl;
  GtClient::for_each_packet([&](const TDatagramPacket2& pck){
    time_t last_{pck.time};
    oss<< pck.Dtgrm.Type <<"; "
        << pck.Dtgrm.Name <<"; "
        << pck.Dtgrm.Size <<"; "
        << pck.tick <<"; "
        << std::ctime( &last_ ) <<"; "
        << std::endl;
  });
  result = oss.str();
  return true;
}


bool server(const http::Request& , std::string &result) {
  std::ostringstream oss;
  time_t nnow; time(&nnow);
  oss<< std::ctime( &nnow )<<std::endl;
  auto filter = GtServer::get_filter();
  if(filter)
    filter->for_each([&](auto& rec){
      time_t last_{rec.store.time};
      oss<< rec.store.Dtgrm.Type <<"; "
          << rec.store.Dtgrm.Name <<"; "
          << rec.store.Dtgrm.Size <<"; "
          << rec.store.tick <<"; "
          << std::ctime( &last_ ) <<"; "
          << std::endl;
    });
  result = oss.str();
  return true;
}

static WebUI::RegistratorGET _get[] = {
    {"/", hello},
    {"/client", client},
    {"/server", server}
};
