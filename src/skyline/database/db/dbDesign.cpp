#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono> // to write date to output file

#include "dbDesign.h"
#include "dbUtil.h"

inline std::string getCalenderDate()
{
  auto now = std::chrono::system_clock::now();
  std::time_t now_ctime = std::chrono::system_clock::to_time_t(now);

  auto parts = std::localtime(&now_ctime);

  int year = parts->tm_year + 1900;
  int mon  = parts->tm_mon + 1;
  int day  = parts->tm_mday;
  
  std::string calenderDate =
      std::to_string(mon) + "/" 
    + std::to_string(day) + "/" 
    + std::to_string(year);

  return calenderDate;
}

namespace db
{

dbDesign::dbDesign(const std::shared_ptr<dbTypes> types,
                   const std::shared_ptr<dbTech>  tech)
  : tech_    (tech),
    types_   (types),
    name_    (""),
    divider_ ('/'),
    coreLx_  (std::numeric_limits<int>::max()),
    coreLy_  (std::numeric_limits<int>::max()),
    coreUx_  (std::numeric_limits<int>::min()),
    coreUy_  (std::numeric_limits<int>::min())
{
  str2dbInst_.clear();
  str2dbBTerm_.clear();
  str2dbNet_.clear();
}

dbDesign::~dbDesign()
{
  rows_.clear();
  insts_.clear();
  nets_.clear();
  bterms_.clear();
  iterms_.clear();

  str2dbInst_.clear();
  str2dbBTerm_.clear();
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

dbBTerm*
dbDesign::getBTermByName(const std::string& name)
{  
  auto itr = str2dbBTerm_.find(name);
  
  if(itr == str2dbBTerm_.end())  
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
dbDesign::setDivider(const char div)
{
  const char lefDiv = tech_->getDivider();
  const char defDiv = div;

  if(lefDiv != defDiv)
  {
    printf("DEF Div (%c) is different from LEF Div (%c)\n", 
            defDiv, lefDiv);
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

  //die_.print();
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
  //newRow->print();
  
  if(coreLx_ > newRow->lx()) coreLx_ = newRow->lx();
  if(coreLy_ > newRow->ly()) coreLy_ = newRow->ly();
  if(coreUx_ < newRow->ux()) coreUx_ = newRow->ux();
  if(coreUy_ < newRow->uy()) coreUy_ = newRow->uy();
}

void
dbDesign::addNewInst(const defiComponent* comp, const std::string& name)
{
  dbInst* newInst = new dbInst;
  insts_.push_back( newInst );

  // This is really weird part of LEF/DEF C++ API.
  // id() of defiComponent returns the name of the instance,
  // and name() returns the name of the LEF MACRO.
  // macroName() even returns null pointer...
  newInst->setName( name );

  if(duplicateCheck(str2dbInst_, name))
  {
    printf("Inst %s already exists!\n", name.c_str());
    exit(1);
  }

  str2dbInst_[name] = newInst;
  
  dbMacro* macro = tech_->getMacroByName( std::string(comp->name()) );

  if(macro == nullptr)
  {
    printf("Error - Macro %s", comp->name());
    printf(" does not exist in the technology database...\n");
    exit(1);
  }

  newInst->setMacro( macro );

  fillInst(comp, newInst);

  // dbITerms are created at the same time with dbInst
  for(auto mterm : macro->getMTerms())
  {
    const std::string itermName = makeITermName(name, mterm->name());
    dbITerm* newITerm = new dbITerm(itermName, newInst, mterm);
    iterms_.push_back(newITerm);
    newInst->addITerm(newITerm);
  }
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
    // haloEdges is non-const method originally in LEF/DEF C++ APIs,
    // so we have to change haloEdges to const method.
    comp->haloEdges(&left, &bottom, &right, &top);
    inst->setHalo(top, bottom, left, right);
  }

  if(comp->hasRouteHalo() > 0) 
  {
    printf("ROUTEHALO on component is unsupported\n");
    exit(1);
  }

  //inst->print();
}

void 
dbDesign::addNewIO(const defiPin* pin, const std::string& name)
{
  dbBTerm* newBTerm = new dbBTerm;
  bterms_.push_back( newBTerm );
  newBTerm->setName( name );

  if(duplicateCheck(str2dbBTerm_, name))
  {
    printf("BTerm %s already exists!\n", name.c_str());
    exit(1);
  }

  str2dbBTerm_[name] = newBTerm;
  
  const std::string netNameStr = pin->netName();
  dbNet* net = getNetByName( netNameStr );

  if(net == nullptr)
    net = getNewNet( netNameStr );

  newBTerm->setNet(net);

  if(pin->hasDirection())
  {
    auto dir = types_->getPinDirection( std::string(pin->direction()) );
    newBTerm->setDirection( dir );
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
      newBTerm->setOrigX( pin->placementX() );
      newBTerm->setOrigY( pin->placementY() );
      auto orient = types_->getOrient( pin->orientStr() );
      newBTerm->setOrient( orient );
    }
  
    if(pin->hasLayer())
    {
      for(int i = 0; i < pin->numLayer(); i++)
      {
        int xl, yl, xh, yh;
        pin->bounds(i, &xl, &yl, &xh, &yh);
        dbLayer* layer = tech_->getLayerByName( std::string(pin->layer(i)) );

        if(layer == nullptr)
        {
          printf("Error - Layer %s", pin->layer(i));
          printf(" does not exist in the technology database...\n");
          exit(0);
        }

        newBTerm->addRect( dbRect(xl, yl, xh, yh, layer) );
        // printf("(%d %d) (%d %d)\n", xl, yl, xh, yh);
      }
    }

    newBTerm->setLocation();
  }

  //newBTerm->print();
}

dbNet*
dbDesign::getNewNet(const std::string& name)
{
  dbNet* newNet = new dbNet;
  nets_.push_back( newNet );
  newNet->setName(name);

  if(duplicateCheck(str2dbNet_, name))
  {
    printf("Net %s already exists!\n", name.c_str());
    exit(1);
  }

  str2dbNet_[name] = newNet;

  return newNet;
}

void
dbDesign::fillNet(const defiNet* defNet, dbNet* net)
{
  if(defNet->hasUse())    
  {
    auto use = types_->getNetUse(std::string(defNet->use()));
    net->setUse(use);
  }

  if(defNet->hasSource()) 
  {
    auto src = types_->getSource(std::string(defNet->source()));
    net->setSource(src);
  }

  for(int i = 0; i < defNet->numConnections(); ++i) 
  {
    if(defNet->pinIsSynthesized(i)) 
    {
      printf("SYNTHESIZED is not unsupported syntax.");
      exit(1);
    }

    if(defNet->pinIsMustJoin(i)) 
    {
      printf("MUSTJOIN is not unsupported syntax.");
      exit(1);
    } 
    else 
    {
      const std::string& instNameStr 
        = removeBackSlashBracket( std::string(defNet->instance(i)) );
      const std::string& termNameStr = std::string(defNet->pin(i));

      if(instNameStr == "PIN" || instNameStr == "Pin" || instNameStr == "pin")
      {
        dbBTerm* bterm = getBTermByName(termNameStr);
        assert(bterm != nullptr);
        net->addBTerm(bterm);
        bterm->setNet(net);
      }
      else
      {
        dbInst* inst = getInstByName(instNameStr);
        assert(inst != nullptr);

        dbITerm* iterm = inst->getITermByMTermName(termNameStr);
        assert(iterm != nullptr);
        
        net->addITerm(iterm);
        iterm->setNet(net);
      }
    }
  }
  // net->print();
}

const std::string
dbDesign::makeITermName(const std::string& instName, const std::string& mTermName) const
{
  const std::string divStr = std::string(1, divider_); 
  // convert a single char to std::string
  return instName + divStr + mTermName;
}

void
dbDesign::writeBookShelf(const char* path) const
{
  const int dbu = tech_->getDbu();

  std::string auxFileName = std::string(path); 
  std::string auxFileNameWithOutSuffix = "";
  std::string auxFileNameWithOutSlash  = "";

  if(auxFileName == "")
  {
    auxFileName = name_ + ".aux";
    std::cout << "File name is not given... ";
    std::cout << "design name will be used by default..." << std::endl;
  }
  else
  {
    size_t dot   = auxFileName.find_last_of('.');
    size_t slash = auxFileName.find_last_of('/');

    std::string suffix = auxFileName.substr(dot + 1);
    auxFileNameWithOutSuffix = auxFileName.substr(0, dot);
    auxFileNameWithOutSlash  = auxFileName.substr(slash + 1, dot - slash - 1);

    if(suffix != "aux")
    {
      printf("BookShelf output should be .aux file!\n");
      exit(1);
    }
  }

  std::string plFileName = auxFileNameWithOutSuffix + ".pl";
  
  // Step #1. Write .aux file 
  std::ofstream aux_output;
  aux_output.open(auxFileName);

  aux_output << "RowBasedPlacement :" << " ";
  aux_output << name_ + ".nodes" << " ";
  aux_output << name_ + ".nets" << " ";
  aux_output << name_ + ".wts" << " ";
  // If wts filename is not written in aux, 
  // ntuplace3 binaray makes segmentation fault.
  aux_output << auxFileNameWithOutSlash + ".pl" << " ";
  aux_output << name_ + ".scl";

  aux_output.close();

  // Step #2. Write .pl file 
  // Print Headline
  std::ofstream pl_output;
  pl_output.open(plFileName);

  pl_output << "UCLA pl 1.0" << std::endl;
  pl_output << "# Created: " << getCalenderDate() << std::endl;
  pl_output << "# User: Jaekyung Im (jkim97@postech.ac.kr)" << std::endl;
  pl_output << std::endl;

  for(auto& inst : insts_)
  {
    pl_output << inst->name() << " ";
    pl_output << static_cast<float>(inst->lx()) 
			         / static_cast<float>(dbu) << " ";
    pl_output << static_cast<float>(inst->ly())
			         / static_cast<float>(dbu) << " : N";
    if(inst->isFixed())
      pl_output << " /FIXED";
    pl_output << std::endl;
  }

  pl_output.close();
  printf("Write results to .aux (%s) and .pl (%s).\n", 
          auxFileName.c_str(), plFileName.c_str());
}

void
dbDesign::writeDef(const char* path) const
{

}

}
