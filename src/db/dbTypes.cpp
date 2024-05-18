#include <iostream>
#include <algorithm>

#include "dbTypes.h"

namespace db
{

dbTypes::dbTypes()
{
  // Initialization
  strToRoutingType_["CUT"        ] = RoutingType::CUT;
  strToRoutingType_["ROUTING"    ] = RoutingType::ROUTING;
  strToRoutingType_["MASTERSLICE"] = RoutingType::MASTERSLICE;
  strToRoutingType_["OVERLAP"    ] = RoutingType::OVERLAP;

  strToLayerDirection_["VERTICAL"  ] = LayerDirection::VERTICAL;
  strToLayerDirection_["HORIZONTAL"] = LayerDirection::HORIZONTAL;

  strToMacroClass_["CORE"       ] = MacroClass::CORE;
  strToMacroClass_["CORE_SPACER"] = MacroClass::CORE_SPACER;
  strToMacroClass_["PAD"        ] = MacroClass::PAD;
  strToMacroClass_["BLOCK"      ] = MacroClass::BLOCK;
  strToMacroClass_["ENDCAP"     ] = MacroClass::ENDCAP;
  
  strToSiteClass_["CORE"] = SiteClass::CORE_SITE;
  strToSiteClass_["core"] = SiteClass::CORE_SITE;

  strToPinDirection_["INPUT" ] = PinDirection::INPUT;
  strToPinDirection_["OUTPUT"] = PinDirection::OUTPUT;
  strToPinDirection_["INOUT" ] = PinDirection::INOUT;

  strToPinUsage_["SIGNAL" ] = PinUsage::SIGNAL;
  strToPinUsage_["POWER"  ] = PinUsage::POWER;
  strToPinUsage_["GROUND" ] = PinUsage::GROUND;
  strToPinUsage_["CLOCK"  ] = PinUsage::CLOCK;

  strToOrient_["N" ] = Orient::N;
  strToOrient_["S" ] = Orient::S;
  strToOrient_["FN"] = Orient::FN;
  strToOrient_["FS"] = Orient::FS;

  strToSource_["DIST"   ] = Source::DIST;
  strToSource_["NETLIST"] = Source::NETLIST;
  strToSource_["TIMING" ] = Source::TIMING;
  strToSource_["USER"   ] = Source::USER;
}

RoutingType
dbTypes::getRoutingType(const std::string& str) const
{
  auto itr = strToRoutingType_.find(str);
  
  if(itr == strToRoutingType_.end())
  {
    std::cout << "Error - ROUTING TYPE " << str;
    std::cout << " is unknown (or not supported yet)..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

LayerDirection
dbTypes::getLayerDirection(const std::string& str) const
{
  auto itr = strToLayerDirection_.find(str);
  
  if(itr == strToLayerDirection_.end())
  {
    std::cout << "Error - DIRECTION " << str;
    std::cout << " is unknown (or not supported yet)..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

MacroClass
dbTypes::getMacroClass(const std::string& str) const
{
  auto itr = strToMacroClass_.find(str);
  
  if(itr == strToMacroClass_.end())
  {
    std::cout << "Error - MACRO CLASS " << str;
    std::cout << " is unknown (or not supported yet)..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

SiteClass
dbTypes::getSiteClass(const std::string& str) const
{
  auto itr = strToSiteClass_.find(str);
  
  if(itr == strToSiteClass_.end())
  {
    std::cout << "Error - SITE CLASS " << str;
    std::cout << " is unknown (or not supported yet)..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

PinDirection
dbTypes::getPinDrection(const std::string& str) const
{
  auto itr = strToPinDirection_.find(str);
  
  if(itr == strToPinDirection_.end())
  {
    std::cout << "Error - PIN DIRECTION " << str;
    std::cout << " is unknown (or not supported yet)..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

PinUsage
dbTypes::getPinUsage(const std::string& str) const
{
  auto itr = strToPinUsage_.find(str);
  
  if(itr == strToPinUsage_.end())
  {
    std::cout << "Error - PIN  USAGE " << str;
    std::cout << " is unknown (or not supported yet)..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

Source
dbTypes::getSource(const std::string& str) const
{
  auto itr = strToSource_.find(str);
  
  if(itr == strToSource_.end())
  {
    std::cout << "Error - SOURCE " << str;
    std::cout << " is unknown (or not supported yet)..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

}
