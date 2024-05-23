#include "dbDesign.h"

namespace db
{

dbDesign::dbDesign(const std::shared_ptr<dbTypes> types,
                   const std::shared_ptr<dbTech>  tech)
  : tech_  (tech),
    types_ (types),
    name_  ("")
{
}

dbDesign::~dbDesign()
{
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
    printf("Not support for non-rectangular die...\n");
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
  dbRow newRow;

  newRow.setName( std::string( ro->name() ) );

  dbSite* site = tech_->getSiteByName( std::string(ro->macro()) );

  newRow.setSite(site);
  newRow.setOrigX( static_cast<int>(ro->x()) );
  newRow.setOrigY( static_cast<int>(ro->y()) );

  auto ori = types_->getOrient( std::string(ro->orientStr()) );
  newRow.setOrient( ori );

  if( ro->hasDo() )
  {
    newRow.setNumSiteX( ro->xNum() );
    newRow.setNumSiteY( ro->yNum() );
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

    newRow.setStepX( stepX );
    newRow.setStepY( stepY );
  }

  rows_.push_back(newRow);

  newRow.print();
}

}
