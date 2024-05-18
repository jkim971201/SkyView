#pragma once

#include <string>
#include <memory>

#include "lef/lefrReader.hpp"
#include "lef/lefwWriter.hpp"
#include "lef/lefiDebug.hpp"
#include "lef/lefiEncryptInt.hpp"
#include "lef/lefiUtil.hpp"

#include "dbTech.h"

namespace db
{

class dbLefReader
{
  public:

    dbLefReader(std::shared_ptr<dbTypes> types, 
                std::shared_ptr<dbTech> tech);

    void init();

    void parseLef(const std::string& filename);

    // LEF CallBacks.
		static int lefUnitsCbk    (lefrCallbackType_e c, lefiUnits*      unit, lefiUserData ud);
    static int lefLayerCbk    (lefrCallbackType_e c, lefiLayer*        la, lefiUserData ud);
    static int lefSiteCbk     (lefrCallbackType_e c, lefiSite*         si, lefiUserData ud);
  
    static int lefStartCbk    (lefrCallbackType_e c, const char*     name, lefiUserData ud);
    static int lefMacroCbk    (lefrCallbackType_e c, lefiMacro*        ma, lefiUserData ud); 
    static int lefMacroPinCbk (lefrCallbackType_e c, lefiPin *         ma, lefiUserData ud);
    static int lefMacroObsCbk (lefrCallbackType_e c, lefiObstruction* obs, lefiUserData ud);
    static int lefEndCbk      (lefrCallbackType_e c, const char*     name, lefiUserData ud);

  private:

    std::shared_ptr<dbTypes> types_;
    std::shared_ptr<dbTech>  tech_;

    int parse65nm_;
    int parseLef58Type_;
};

}
