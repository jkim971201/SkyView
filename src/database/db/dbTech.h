#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "lef/lefrReader.hpp"
#include "dbLayer.h"
#include "dbSite.h"

namespace db
{

class dbTech
{
  public:

    dbTech(std::shared_ptr<dbTypes> types)
      : types_ (types),
        dbu_   (    0)
    {}

          dbLayer* getLayerByName(std::string& name);
    const dbLayer* getLayerByName(std::string& name) const
    {
      return getLayerByName(name);
    };

          dbSite*  getSiteByName(std::string& name);
    const dbLayer* getSiteByName(std::string& name) const
    {
      return getSiteByName(name);
    };

    void setUnits       (const lefiUnits* unit);
    void createNewLayer (const lefiLayer*   la);
    void createNewSite  (const lefiSite*    si);

    int getDbuLength(double micron) const;
    int getDbuArea  (double micron) const;

  private:

    // DBU per MICRON
    int dbu_;

    std::shared_ptr<dbTypes> types_;

    std::unordered_map<std::string, dbLayer*> str2dbLayer_;
    std::unordered_map<std::string, dbSite*>  str2dbSite_;

    std::vector<dbLayer> layers_;
    std::vector<dbSite>  sites_;
};

}
