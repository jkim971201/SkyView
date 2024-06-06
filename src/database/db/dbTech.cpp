#include <iostream>
#include <cassert>
#include <climits>
#include <algorithm>

#include "dbTech.h"

namespace db
{

dbTech::dbTech(std::shared_ptr<dbTypes> types)
  : types_               (types),
    dbu_                 (0),
    left_bus_delimiter_  ('['),
    right_bus_delimiter_ (']'),
    divider_             ('/')
{
  str2dbLayer_.clear();
  str2dbSite_.clear();
  str2dbMacro_.clear();
}

dbTech::~dbTech()
{
  str2dbLayer_.clear();
  str2dbSite_.clear();
  str2dbMacro_.clear();

  layers_.clear();
  sites_.clear();
  macros_.clear();
}

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

dbMacro*
dbTech::getMacroByName(const std::string& name)
{
  auto itr = str2dbMacro_.find(name);

  if(itr == str2dbMacro_.end())
  {
    std::cout << "Error - Macro " << name;
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
dbTech::setBusBit(const char* busBit)
{
  left_bus_delimiter_  = busBit[0];
  right_bus_delimiter_ = busBit[1];
}

void
dbTech::setDivider(const char div)
{
  divider_ = div;
}

void
dbTech::createNewLayer(const lefiLayer* la)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  dbLayer* newLayer = new dbLayer;
  layers_.push_back( newLayer );
  newLayer->setName(la->name());

  str2dbLayer_[newLayer->name()] = newLayer;

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
 
  //newLayer->print();
}

void
dbTech::createNewSite(const lefiSite* site)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  dbSite* newSite = new dbSite;
  sites_.push_back(newSite);

  newSite->setName(site->name());
  str2dbSite_[newSite->name()] = newSite;

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

  //newSite->print();
}

void
dbTech::addPinToMacro(const lefiPin* pi, dbMacro* topMacro)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  dbMTerm* newMTerm = new dbMTerm;
  newMTerm->setName( std::string(pi->name()) );
  newMTerm->setMacro( topMacro );

  if(pi->hasDirection() )
  {
    auto dir = types_->getPinDirection( std::string(pi->direction()) );
    newMTerm->setPinDirection( dir );
  }

  if(pi->hasUse())
  {
    auto use = types_->getPinUsage( std::string(pi->use()) );
    newMTerm->setPinUsage(use);
  }

  if(pi->hasShape())
  {
    auto shape = types_->getPinShape( std::string(pi->shape()) );
    newMTerm->setPinShape(shape);
  }

  int numPorts = pi->numPorts();


  dbLayer* curLayer = nullptr;
  for(int i = 0; i < numPorts; i++)
  {
    lefiGeometries* geo = pi->port(i);
    lefiGeomRect*    lrect = nullptr;
    lefiGeomPolygon* lpoly = nullptr;

    int numItems = geo->numItems();
    for(int j = 0; j < numItems; j++)
    {
      switch(geo->itemType(j))
      {
        case lefiGeomLayerE:
        {
          curLayer = getLayerByName( std::string(geo->getLayer(j)) );
          break;
        }

        case lefiGeomRectE:
        {
          lrect = geo->getRect(j);

          int rectLx = getDbuLength( lrect->xl );
          int rectLy = getDbuLength( lrect->yl );
          int rectUx = getDbuLength( lrect->xh );
          int rectUy = getDbuLength( lrect->yh );

          newMTerm->addRect( dbRect(rectLx, rectLy, rectUx, rectUy, curLayer) );
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

          newMTerm->addRect( dbRect(polyLxDbu, polyLyDbu, polyUxDbu, polyUyDbu, curLayer) );
          break;
        }
      }
    }
  }

  topMacro->addMTerm( newMTerm );
}

void
dbTech::addObsToMacro(const lefiObstruction* obs, dbMacro* topMacro)
{
  lefiGeometries* geo = obs->geometries();

  int numItems = geo->numItems();
  dbLayer* curLayer = nullptr;
  for(int i = 0; i < numItems; i++)
  {
    lefiGeomRect* lrect = nullptr;

    switch(geo->itemType(i)) 
    {
      case lefiGeomLayerE:
      {
        curLayer = getLayerByName( std::string(geo->getLayer(i)) );
        break;
      }
      case lefiGeomRectE:
      {
        lrect = geo->getRect(i);

        int rectLx = getDbuLength( lrect->xl );
        int rectLy = getDbuLength( lrect->yl );
        int rectUx = getDbuLength( lrect->xh );
        int rectUy = getDbuLength( lrect->yh );

        topMacro->addRectToObs( dbRect(rectLx, rectLy, rectUx, rectUy, curLayer) );
        break;
      }
      default: 
        break;    
    }
  }
}

dbMacro*
dbTech::getNewMacro(const char* name)
{
  if(dbu_ == 0)
  {
    std::cout << "Database Unit is not defined!" << std::endl;
    exit(1);
  }

  dbMacro* newMacro = new dbMacro;
  macros_.push_back(newMacro);

  newMacro->setName(name);
  str2dbMacro_[newMacro->name()] = newMacro;

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

  //newMacro->print();
}
  
}
