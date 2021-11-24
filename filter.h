#pragma once

#include <map>
#include <memory>
#include <string>
#include "dmpacket.h"
#include "dmpacket2.h"


class Filter
{
  std::string dbname;

public:

  struct FilterData {
    struct {int dtg_type;
      std::string dtg_name;
      int datasize;
      int live_dt;
    } cfg;
    TDatagramPacket2 store;
  };

  Filter(){}
  bool open();
  void print();

  bool sift(const TDatagram& src);
  bool sift2(const TDatagram2& src);

  void for_each( std::function<void(FilterData& )> fun ) {
    for( auto&& i : filter )
      fun(i.second);
  }

private:
  using FilterKey = std::pair<int, std::string>;
  using FilterType = std::map< FilterKey, FilterData >;
  FilterType filter;
};


using FilterPtr = std::shared_ptr<Filter>;
