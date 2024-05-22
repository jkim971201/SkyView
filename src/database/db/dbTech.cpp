#include <iostream>
#include <cassert>
#include <climits>
#include <algorithm>

#include "dbTech.h"

namespace db
{

dbLayer*
dbTech::getLayerByName(const std::string& name)
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
dbTech::getSiteByName(const std::string& name)
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

void
dbTech::addPinToMacro(const lefiPin* pi, dbMacro* topMacro)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  dbPin newPin;

  if(pi->hasDirection() )
  {
    auto dir = types_->getPinDirection( std::string(pi->direction()) );
    newPin.setPinDirection( dir );
  }

  if(pi->hasUse())
  {
    auto use = types_->getPinUsage( std::string(pi->use()) );
    newPin.setPinUsage(use);
  }

  if(pi->hasShape())
  {
    auto shape = types_->getPinShape( std::string(pi->shape()) );
    newPin.setPinShape(shape);
  }

  int numPorts = pi->numPorts();

  for(int i = 0; i < numPorts; i++)
  {
    lefiGeometries*    geo = pi->port(i);
    lefiGeomRect*    lrect = nullptr;
    lefiGeomPolygon* lpoly = nullptr;

    int numItems = geo->numItems();
    for(int j = 0; j < numItems; j++)
    {
      switch(geo->itemType(j))
      {
        case lefiGeomLayerE:
				{
          dbLayer* layer = getLayerByName( geo->getLayer(j) );
          newPin.setLayer( layer );
          break;
				}

        case lefiGeomRectE:
				{
          lrect = geo->getRect(j);

					int rectLx = getDbuLength( lrect->xl );
          int rectLy = getDbuLength( lrect->yl );
          int rectUx = getDbuLength( lrect->xh );
          int rectUy = getDbuLength( lrect->yh );

          newPin.addRect( dbRect(rectLx, rectLy, rectUx, rectUy) );
          break;
				}

        case lefiGeomPolygonE:
				{
          lpoly = geo->getPolygon(j);

          double polyLx = std::numeric_limits<double>::max();
          double polyLy = std::numeric_limits<double>::max();
          double polyUx = std::numeric_limits<double>::min();
          double polyUy = std::numeric_limits<double>::min();

          int numPoints = lpoly->numPoints;
          for(int k = 0; k < numPoints; k++)
          {
            polyLx = std::min(polyLx, lpoly->x[k]);
            polyLy = std::min(polyLy, lpoly->y[k]);
            polyUx = std::max(polyUx, lpoly->x[k]);
            polyUy = std::max(polyUy, lpoly->y[k]);
          }

					int polyLxDbu = getDbuLength( polyLx );
					int polyLyDbu = getDbuLength( polyLy );
					int polyUxDbu = getDbuLength( polyUx );
					int polyUyDbu = getDbuLength( polyUy );

          newPin.addRect( dbRect(polyLxDbu, polyLyDbu, polyUxDbu, polyUyDbu) );
          break;
				}
      }
    }
  }

  topMacro->addPin( newPin );
}

dbMacro*
dbTech::getNewMacro(const char* name)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  macros_.push_back(dbMacro());
  dbMacro* newMacro = &(macros_.back());
  str2dbMacro_[std::string(name)] = newMacro;

  newMacro->setName(name);

  return newMacro;
}

void
dbTech::fillNewMacro(const lefiMacro* ma, dbMacro* newMacro)
{
  if( ma->hasClass() )
  {
    auto macroClass = types_->getMacroClass(std::string(ma->macroClass()));
    newMacro->setMacroClass(macroClass);
  }

  if( ma->hasOrigin() ) 
  {
    newMacro->setOrigX( getDbuLength(ma->originX()) );
    newMacro->setOrigY( getDbuLength(ma->originY()) );
  }

  if( ma->hasSize() )
  {
    newMacro->setSizeX( getDbuLength(ma->sizeX()) );
    newMacro->setSizeY( getDbuLength(ma->sizeY()) );
  }

  if( ma->hasSiteName() ) 
  {
    dbSite* site = getSiteByName( std::string(ma->siteName()) );
    newMacro->setSite( site );
  }

  if( ma->hasXSymmetry()  ) newMacro->setSymmetryX(true);
  if( ma->hasYSymmetry()  ) newMacro->setSymmetryY(true);
  if( ma->has90Symmetry() ) newMacro->setSymmetryR90(true);

  newMacro->print();
}
  
}
