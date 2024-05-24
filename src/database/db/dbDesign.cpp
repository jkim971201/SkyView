#include "dbDesign.h"

namespace db
{

dbDesign::dbDesign(const std::shared_ptr<dbTypes> types,
                   const std::shared_ptr<dbTech>  tech)
  : tech_  (tech),
    types_ (types),
    name_  ("")
{
  str2dbInst_.clear();
}

dbDesign::~dbDesign()
{
  rows_.clear();
  insts_.clear();
  ios_.clear();
  nets_.clear();

  str2dbInst_.clear();
  str2dbIO_.clear();
  str2dbNet_.clear();
}

dbInst*
dbDesign::getInstByName(const std::string& name)
{
  auto itr = str2dbInst_.find(name);
  
  if(itr == str2dbInst_.end())  
    return nullptr;
  else
    return itr->second;
}

dbIO*
dbDesign::getIOByName(const std::string& name)
{  
  auto itr = str2dbIO_.find(name);
  
  if(itr == str2dbIO_.end())  
    return nullptr;
  else
    return itr->second;
}

dbNet*
dbDesign::getNetByName(const std::string& name)
{ 
  auto itr = str2dbNet_.find(name);
  
  if(itr == str2dbNet_.end())  
    return nullptr;
  else
    return itr->second;
}

void
dbDesign::setDbu(int dbu)
{
  int lefDbu = tech_->getDbu();
  int defDbu = dbu;

  if(lefDbu != defDbu)
  {
    printf("DEF Dbu (%d) is different from LEF Dbu (%d)\n", 
            defDbu, lefDbu);
    exit(1);
  }
}

void
dbDesign::setDie(const defiBox* box)
{
  defiPoints pts = box->getPoint();
  int numPoints = pts.numPoints;

  if(numPoints != 2)
  {
    printf("Does not support non-rectangular die...\n");
    exit(1);
  }

  int lx = box->xl();
  int ly = box->yl();
  int ux = box->xh();
  int uy = box->yh();

  die_.setLx(lx);
  die_.setLy(ly);
  die_.setUx(ux);
  die_.setUy(uy);

  die_.print();
}

void
dbDesign::addNewRow(const defiRow* ro)
{
  dbRow* newRow = new dbRow;

  newRow->setName( std::string( ro->name() ) );

  dbSite* site = tech_->getSiteByName( std::string(ro->macro()) );

  newRow->setSite(site);
  newRow->setOrigX( static_cast<int>(ro->x()) );
  newRow->setOrigY( static_cast<int>(ro->y()) );

  auto ori = types_->getOrient( std::string(ro->orientStr()) );
  newRow->setOrient( ori );

  if( ro->hasDo() )
  {
    newRow->setNumSiteX( ro->xNum() );
    newRow->setNumSiteY( ro->yNum() );
  }

  if( ro->hasDoStep() ) 
  {
    int stepX = ro->xStep();
    int stepY = ro->yStep();

    if(stepX > 1 && stepX < site->sizeX())
    {
      printf("Row StepX %d is smaller than site width %d\n", stepX, site->sizeX());
      exit(1);
    }

    if(stepY > 1 && stepY < site->sizeY())
    {
      printf("Row StepY %d is smaller than site height %d\n", stepY, site->sizeY());
      exit(1);
    }

    newRow->setStepX( stepX );
    newRow->setStepY( stepY );
  }

  rows_.push_back(newRow);

  newRow->print();
}

void
dbDesign::addNewInst(const defiComponent* comp, const std::string& name)
{
  dbInst* newInst = new dbInst;
  insts_.push_back( newInst );

  // This is really weird...
  // id() returns the name of the instance,
  // and name() returns the name of the LEF MACRO.
  // macroName() even returns null pointer...
  newInst->setName( name );
  str2dbInst_[newInst->name()] = newInst;
  
  dbMacro* macro = tech_->getMacroByName( std::string(comp->name()) );
  newInst->setMacro( macro );

  fillInst(comp, newInst);
}

void
dbDesign::fillInst(const defiComponent* comp, dbInst* inst)
{ 
  auto orient = types_->getOrient( std::string( comp->placementOrientStr() ) );
  inst->setOrient( orient );

  auto status = types_->getStatus( comp->placementStatus() );
  inst->setStatus( status );

  inst->setLocation(comp->placementX(), comp->placementY());

  if(comp->hasSource())
  {
    auto source = types_->getSource( std::string( comp->source() ) );
    inst->setSource( source );
  }

  if(comp->hasHalo() > 0) 
  {
    int left, bottom, right, top;
    // haloEdges is non-const method originally,
    // so we have to change haloEdges to const method.
    comp->haloEdges(&left, &bottom, &right, &top);
    inst->setHalo(top, bottom, left, right);
  }

  if(comp->hasRouteHalo() > 0) 
  {
    printf("ROUTEHALO on component is unsupported\n");
    exit(1);
  }

  inst->print();
}

void 
dbDesign::addNewIO(const defiPin* pin, const std::string& name)
{
  dbIO* newIO = new dbIO;
  ios_.push_back( newIO );
  newIO->setName( name );
  str2dbIO_[ newIO->name() ] = newIO;
  
	const std::string netNameStr = pin->netName();
  dbNet* net = getNetByName( netNameStr );

  if(net == nullptr)
		net = getNewNet( netNameStr );

  newIO->setNet(net);

  if(pin->hasDirection())
  {
    auto dir = types_->getPinDirection( std::string(pin->direction()) );
    newIO->setDirection( dir );
  }

  if(pin->hasPort())
  {
    printf("PORT Syntax for DEF PIN is not supported yet...\n");
    exit(1);
  }
  else
  {
    if(pin->hasPlacement())
    {
      newIO->setOrigX( pin->placementX() );
      newIO->setOrigY( pin->placementY() );
      auto orient = types_->getOrient( pin->orientStr() );
      newIO->setOrient( orient );
    }
  
    if(pin->hasLayer())
    {
      for(int i = 0; i < pin->numLayer(); i++)
      {
        int xl, yl, xh, yh;
        pin->bounds(i, &xl, &yl, &xh, &yh);
        dbLayer* layer = tech_->getLayerByName( std::string(pin->layer(i)) );
        newIO->addRect( dbRect(xl, yl, xh, yh, layer) );
        // printf("(%d %d) (%d %d)\n", xl, yl, xh, yh);
      }
    }

    newIO->setLocation();
  }

  newIO->print();
}

dbNet*
dbDesign::getNewNet(const std::string& name)
{
	dbNet* newNet = new dbNet;
	nets_.push_back( newNet );
  newNet->setName(name);
  str2dbNet_[ newNet->name() ] = newNet;

	return newNet;
}

}
