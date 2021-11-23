#include "gttsfilter.h"

#include<iostream>
#include<map>
#include "sqlite_modern_cpp.h"

#include "vstdstr.h"
#include "opt.h"
#include "md5.h"
#include "sqlitetools.h"

#include "gttsperecoder.h"

using namespace sqlite;
using namespace std;
//---------------------------------------------------------------------------
class TsFilterDef
{
public:
    struct TsDef {
        int         Pack;
        int         Sz;
        TsDef() = default;
    };

    typedef std::map<std::string, TsDef> TDefs;
    TDefs Defs;

    std::string dbname;
    std::string signature;

    TsFilterDef() = default;
};
static TsFilterDef Filter;
//---------------------------------------------------------------------------
bool GtTsFilter::open(const char* fn)
{
    //printf("opening db: %s... \n", fn);
    try{
        database db(fn);
        int count = 0;
        db << "select count(*) from sqlite_master where name='datdef';" >> tie(count);
        if (count == 0) {
            printf("bad table: datdef.");
            return false;
        }
        Filter.dbname = fn;
        MD5 md5;
        db << "select dat, pack, sz from datdef ;"
           >> [&](string dat, int pack, int sz) {
            TsFilterDef::TsDef def;
            def.Pack = pack;
            if (!def.Pack) def.Pack = 8;
            def.Sz = sz;
            if (!def.Sz) def.Sz = 256;

            Filter.Defs.insert( make_pair(trim(dat), def) );
            md5.update( dat.c_str(), dat.size() );
        };

        md5.update( "strings", 8 );
        md5.finalize();
        string dig = md5.hexdigest();

        if (Options::instance()->vm.count(MKOK))
            WR_DBCONFIG_string(db, "tskey", dig);

        string tskey = RD_DBCONFIG_string(db, "tskey", "tskey");
        if(tskey==dig)
            cout<<"\nTS config - OK."<<endl;
        else{
            cout<<"\nTS config - Bad!"<<endl;
            Filter.Defs.clear();
        }

        if(Options::instance()->verbose)
            print();

        return !Filter.Defs.empty();
    }
    catch(const sqlite_exception& e) {
        cerr <<endl<<__FUNCTION__<< " sqlite: " << e.what() << endl;
    }
    return false;
}

void GtTsFilter::clear()
{
    Filter.Defs.clear();
}

void GtTsFilter::print()
{
    for(const auto& i : Filter.Defs)
        cout << i.first << ';' << i.second.Pack << ';' << i.second.Sz << endl;
}

bool GtTsFilter::sift(const TDatagram& src, TDatagram2& dst)
{
    //printf("datagramma %d:%s ...\n", src.Type, src.Name);

    if (src.Type != 1) return false;
    auto i = Filter.Defs.find(string(src.Name));
    if (i != Filter.Defs.end()) {
        int src_pck = i->second.Pack;
        size_t icount = i->second.Sz * src_pck;
        GtTsPereCoder::Perepack(src_pck, icount, src, dst);
        return true;
    }
    return false;
}

bool GtTsFilter::sift2(const TDatagram2& src, TDatagram2& dst)
{
    if (src.Type != 1) return false;
    auto i = Filter.Defs.find(string(src.Name));
    if (i != Filter.Defs.end()) {
        int src_pck = i->second.Pack;
        size_t icount = i->second.Sz * src_pck;
        GtTsPereCoder::Perepack2(src_pck, icount, src, dst);
        return true;
    }
    return false;
}


