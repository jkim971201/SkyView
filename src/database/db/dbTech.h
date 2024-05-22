#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "lef/lefrReader.hpp"
#include "dbLayer.h"
#include "dbSite.h"
#include "dbMacro.h"

namespace db
{

class dbTech
{
  public:

    dbTech(std::shared_ptr<dbTypes> types);
    ~dbTech();

          dbLayer* getLayerByName(const std::string& name);
    const dbLayer* getLayerByName(const std::string& name) const
    {
      return getLayerByName(name);
    };

          dbSite* getSiteByName(const std::string& name);
    const dbSite* getSiteByName(const std::string& name) const
    {
      return getSiteByName(name);
    };

    void setUnits        (const lefiUnits* unit);
    void createNewLayer  (const lefiLayer*   la);
    void createNewSite   (const lefiSite*    si);
    void addPinToMacro   (const lefiPin*         pi, dbMacro* topMacro);
    void addObsToMacro   (const lefiObstruction* ob, dbMacro* topMacro);

    dbMacro* getNewMacro (const char* name);
    void fillNewMacro    (const lefiMacro* ma, dbMacro* newMacro); 

    int getDbuLength(double micron) const;
    int getDbuArea  (double micron) const;

  private:

    // DBU per MICRON
    int dbu_;

    std::shared_ptr<dbTypes> types_;

    std::unordered_map<std::string, dbLayer*> str2dbLayer_;
    std::unordered_map<std::string, dbSite*>  str2dbSite_;
    std::unordered_map<std::string, dbMacro*> str2dbMacro_;

    std::vector<dbLayer*> layers_;
    std::vector<dbSite*>  sites_;
    std::vector<dbMacro*> macros_;
};

}
