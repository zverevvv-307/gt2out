#include "filter.h"

#include <memory>
#include <iostream>
#include <fstream>      // fstream
#include <vector>
#include <map>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "vstdstr.h"
#include "helpers.h"
#include "opt.h"


using namespace boost;
//---------------------------------------------------------------------------
bool Filter::open()
{
  filter.clear();
  dbname = Options::instance()->config_file;
  try{
    std::ifstream in(dbname);
    if (!in.is_open()){
      std::cerr<<"bad config file: " << dbname << std::endl;
      return false;
    }

    escaped_list_separator<char> els('\\',';','\"');
    std::vector< std::string > words;
    std::string line;
    while (getline(in,line)) {
      tokenizer< escaped_list_separator<char> > tok(line, els);
      words.assign(tok.begin(),tok.end());
      if (words.size() < 3) continue;


      FilterData rec;
      rec.cfg.dtg_type = to_int(words[0]);
      rec.cfg.dtg_name = trim(right_trim(words[1]));
      rec.cfg.datasize = to_int(words[2]);

      rec.store    = TPackEx2();
      rec.store.Dtgrm.Type = rec.cfg.dtg_type;
      rec.store.Dtgrm.setName(rec.cfg.dtg_name);
      rec.store.Dtgrm.Size = rec.cfg.datasize;

      FilterKey key{rec.cfg.dtg_type, rec.cfg.dtg_name};
      filter.insert_or_assign( key, rec );
    }
    print();
    return true;
  }
  catch(const std::exception& e) {
    std::cerr << "Unexpected exception: " << e.what() << std::endl;
  }
  catch(...) {
    std::cerr << "Unknown error\n";
  }
  return false;
}

void Filter::print()
{
  std::cout << "filter /" << dbname << "/ :  \n----------------------\n";
  for(const auto& i : filter)
    std::cout << i.second.cfg.dtg_type
              << ';' << i.second.cfg.dtg_name
              << ';' << i.second.cfg.datasize
              << std::endl;
  std::cout << "----------------------\n";
}

bool Filter::sift(const TDatagram& dtg)
{
  TDatagram2 dtg2;
  ConvertDtgrmStruct(dtg, dtg2);
  return this->sift2(dtg2);
}

bool Filter::sift2(const TDatagram2& dtg2)
{
  FilterKey key{dtg2.Type, dtg2.Name};
  auto rec = filter.find( key );
  if( rec == filter.end() )
    return false;

  auto& pck2 = rec->second.store;

  pck2.msec = Vx::timestamp();
  pck2.time = Vx::time_t32_now();

  int offset = rec->second.cfg.dtg_type==1 ? 1: 0;
  if( dtg2.Type==0
      || memcmp(
             &pck2.Dtgrm.Data[offset],
             &dtg2.Data[offset],
             rec->second.cfg.datasize-offset
             )!=0
      )
    pck2.tick++;//посчитаем изменение

  pck2.Dtgrm = dtg2;

  return true;
}


