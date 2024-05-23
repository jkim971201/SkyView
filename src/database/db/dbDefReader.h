#pragma once

#include <string>
#include <memory>

#include "def/defrReader.hpp"
#include "def/defiAlias.hpp"

#include "dbTech.h"

namespace db
{

class dbDefReader
{
  public:

    dbDefReader(std::shared_ptr<dbTypes> types, 
                std::shared_ptr<dbTech> tech);

    void init();

    void parseDef(const std::string& filename);

    // DEF CallBacks
    
    // Designs
    static int defDesignCbk(defrCallbackType_e c, const char* name, defiUserData ud);

    // Units
    static int defUnitsCbk(defrCallbackType_e c, double unit, defiUserData ud);

    // Die
    static int defDieAreaCbk(defrCallbackType_e c, defiBox* box, defiUserData ud);

    // Rows
    static int defRowCbk(defrCallbackType_e c, defiRow* ro, defiUserData ud);

    // Nets
    static int defNetStartCbk (defrCallbackType_e c, int   number, defiUserData ud);
    static int defNetCbk      (defrCallbackType_e c, defiNet* net, defiUserData ud);
    static int defNetEndCbk   (defrCallbackType_e c, void*       , defiUserData ud);
    
    // Special Nets
    static int defSNetStartCbk (defrCallbackType_e c, int   number, defiUserData ud);
    static int defSNetCbk      (defrCallbackType_e c, defiNet* net, defiUserData ud);
    static int defSNetEndCbk   (defrCallbackType_e c, void*       , defiUserData ud);

    // Pins
    static int defPinStartCbk (defrCallbackType_e c, int  number, defiUserData ud);
    static int defPinCbk      (defrCallbackType_e c, defiPin* pi, defiUserData ud);
    static int defPinEndCbk   (defrCallbackType_e c, void*      , defiUserData ud);

    // Components
    static int defComponentStartCbk (defrCallbackType_e c, int        number, defiUserData ud);
    static int defComponentCbk      (defrCallbackType_e c, defiComponent* co, defiUserData ud);
    static int defComponentEndCbk   (defrCallbackType_e c, void*            , defiUserData ud);

    // End Design
    static int defEndCbk(defrCallbackType_e c, void* , defiUserData ud);
  
  private:

    std::shared_ptr<dbTypes> types_;
    std::shared_ptr<dbTech>  tech_;
};

}
