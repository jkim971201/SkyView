#ifndef DB_TECH
#define DB_TECH

#include <memory>
#include <string>
#include <unordered_map>

#include "lef/lefrReader.hpp"
#include "dbTypes.h"
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

    void setUnits        (const lefiUnits* unit);
    void setBusBit       (const char* busBit);
    void setDivider      (const char* divider);
    void createNewLayer  (const lefiLayer* la);
    void createNewSite   (const lefiSite* si);
    void addPinToMacro   (const lefiPin*         pi, dbMacro* topMacro);
    void addObsToMacro   (const lefiObstruction* ob, dbMacro* topMacro);

    dbMacro* getNewMacro (const char* name);
    void fillNewMacro    (const lefiMacro* ma, dbMacro* newMacro); 

    int getDbu() const { return dbu_; }
    int getDbuLength(double micron) const;
    int getDbuArea  (double micron) const;

    int getRightBusBit() const { return right_bus_delimiter_; }
    int getLeftBusBit()  const { return left_bus_delimiter_;  }

    dbLayer* getLayerByName (const std::string& name);
    dbSite*  getSiteByName  (const std::string& name);
    dbMacro* getMacroByName (const std::string& name);

  private:

    // DBU per MICRON
    int dbu_;

    char left_bus_delimiter_;
    char right_bus_delimiter_;
    char divider_;

    std::shared_ptr<dbTypes> types_;

    std::unordered_map<std::string, dbLayer*> str2dbLayer_;
    std::unordered_map<std::string, dbSite*>  str2dbSite_;
    std::unordered_map<std::string, dbMacro*> str2dbMacro_;

    std::vector<dbLayer*> layers_;
    std::vector<dbSite*>  sites_;
    std::vector<dbMacro*> macros_;
};

}

#endif
