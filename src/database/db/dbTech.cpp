#include <iostream>
#include <algorithm>

#include "dbTech.h"

namespace db
{

dbLayer*
dbTech::getLayerByName(std::string& name)
{
  auto itr = str2dbLayer_.find(name);

  if(itr == str2dbLayer_.end())
  {
    std::cout << "Error - Layer " << name;
    std::cout << " does not exist in the database..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

dbSite*
dbTech::getSiteByName(std::string& name)
{
  auto itr = str2dbSite_.find(name);

  if(itr == str2dbSite_.end())
  {
    std::cout << "Error - Site " << name;
    std::cout << " does not exist in the database..." << std::endl;
    exit(0);
  }
  else
    return itr->second;
}

int
dbTech::getDbuLength(double micron) const
{
  double dbuLength = micron * static_cast<double>(dbu_);
  return static_cast<int>(dbuLength);
}

int
dbTech::getDbuArea(double micron) const
{
  double dbuArea = micron * static_cast<double>(dbu_) * static_cast<double>(dbu_);
  return static_cast<int>(dbuArea);
}

void
dbTech::setUnits(const lefiUnits* unit)
{
  if(unit->hasDatabase())
    dbu_ = static_cast<int>(unit->databaseNumber());
}

void
dbTech::createNewLayer(const lefiLayer* la)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  layers_.push_back(dbLayer());
  dbLayer* newLayer = &(layers_.back());
  str2dbLayer_[std::string(la->name())] = newLayer;

  newLayer->setName(la->name());

  if(la->hasType())      
  {
    auto type = types_->getRoutingType(std::string(la->type()));
    newLayer->setType(type);
  }

  if(la->hasDirection()) 
  {
    auto dir = types_->getLayerDirection(std::string(la->direction()));
    newLayer->setDirection(dir);
  }

  if(la->hasPitch()) 
  {
    newLayer->setXPitch(getDbuLength(la->pitch()));
    newLayer->setYPitch(getDbuLength(la->pitch()));
  }
  else if(la->hasXYPitch()) 
  { 
    newLayer->setXPitch(getDbuLength(la->pitchX()));
    newLayer->setYPitch(getDbuLength(la->pitchY()));
  }

  if(la->hasOffset()) 
  {
    newLayer->setXOffset(getDbuLength(la->offset()));
    newLayer->setYOffset(getDbuLength(la->offset()));
  }
  else if(la->hasXYOffset()) 
  {
    newLayer->setXOffset(getDbuLength(la->offsetX()));
    newLayer->setYOffset(getDbuLength(la->offsetY()));
  }

  if(la->hasWidth()) 
    newLayer->setWidth(getDbuLength(la->width()));

  if(la->hasArea())
    newLayer->setArea(getDbuArea(la->area()));

  //if(la->hasSpacing()) 
  //  newLayer->setSpacing(la->spacing());
 
  newLayer->print();
}

void
dbTech::createNewSite(const lefiSite* site)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  sites_.push_back(dbSite());
  dbSite* newSite = &(sites_.back());
  str2dbSite_[std::string(site->name())] = newSite;

  newSite->setName(site->name());

  if(site->hasSize())
  {
    newSite->setSizeX( getDbuLength(site->sizeX()) );
    newSite->setSizeY( getDbuLength(site->sizeY()) );
  }

  if(site->hasClass())
  {
    auto siteClass = types_->getSiteClass(std::string(site->siteClass()));
    newSite->setSiteClass( siteClass );
  }

  if(site->hasXSymmetry())
    newSite->setSymmetryX(true);
  if(site->hasYSymmetry())
    newSite->setSymmetryY(true);
  if(site->has90Symmetry())
    newSite->setSymmetryR90(true);

  newSite->print();
}
  
}
