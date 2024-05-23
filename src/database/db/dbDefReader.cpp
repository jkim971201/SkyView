#include <cstdio>
#include <cassert>
#include <cstdio>
#include <string>

#include "dbDefReader.h"

namespace db
{

static void defLogFunction(const char* errMsg) 
{
  printf("ERROR: %s\n", errMsg);
}

static inline std::string removeBackSlashBracket(const std::string& str)
{
	std::string newStr = str;
	//printf("before : %s\n", newStr.c_str());
  if(newStr.find("\\[") != std::string::npos && newStr.find("\\]") != std::string::npos)
  {
    size_t bracket1 = newStr.find("\\[");
    while(bracket1 != std::string::npos)
    {
      newStr.erase(newStr.begin() + bracket1);
      bracket1 = newStr.find("\\[");
    }

    size_t bracket2 = newStr.find("\\]");
    while(bracket2 != std::string::npos)
    {
      newStr.erase(newStr.begin() + bracket2);
      bracket2 = newStr.find("\\]");
    }
  }
	//printf("after : %s\n", newStr.c_str());

	return newStr;
}

void checkType(defrCallbackType_e c)
{
  if(c >= 0 && c <= defrDesignEndCbkType) 
  {
    // OK
    // Do Nothing
  } 
  else 
    printf("ERROR: callback type is out of bounds!\n");
}

dbDefReader::dbDefReader(std::shared_ptr<dbTypes>  types, 
                         std::shared_ptr<dbTech>   tech,
                         std::shared_ptr<dbDesign> design)
  : types_   ( types),
    tech_    (  tech),
    design_  (design)
{
}

void
dbDefReader::init()
{
  defrSetLogFunction(defLogFunction);

  defrInitSession(0);
  
  defrSetUserData(design_.get());

  defrSetDesignCbk(this->defDesignCbk);

  // Unit
  defrSetUnitsCbk(this->defUnitsCbk);
  
  // Die
  defrSetDieAreaCbk((defrBoxCbkFnType)this->defDieAreaCbk);

  // Rows 
  defrSetRowCbk((defrRowCbkFnType)this->defRowCbk);

  // Components
  defrSetComponentStartCbk(this->defComponentStartCbk);
  defrSetComponentCbk(this->defComponentCbk);
  defrSetComponentEndCbk(this->defComponentEndCbk);

  // Pins
  defrSetStartPinsCbk(this->defPinStartCbk);
  defrSetPinCbk((defrPinCbkFnType)this->defPinCbk);
  defrSetPinEndCbk(this->defPinEndCbk);

  // Nets
  defrSetNetStartCbk(this->defNetStartCbk);
  defrSetNetCbk(this->defNetCbk);
  defrSetNetEndCbk(this->defNetEndCbk);

  // Special Nets
  defrSetSNetStartCbk(this->defSNetStartCbk);
  defrSetSNetCbk(this->defSNetCbk);
  defrSetSNetEndCbk(this->defSNetEndCbk);

  // End Design
  defrSetDesignEndCbk(this->defEndCbk);
}

void
dbDefReader::parseDef(const std::string& filename)
{
  init();

  size_t dot = filename.find_last_of('.');
  std::string filetype = filename.substr(dot + 1);

  if(filetype != "def")
  {
    printf("Please give .def file!\n");
    exit(1);
  }

  FILE* file = fopen(filename.c_str(), "r");

  if(file == nullptr)
  {
    printf("Cannot open %s\n", filename.c_str());
    exit(1);
  }

  int res = defrRead(file, filename.c_str(), (void*) design_.get(), 1);
  fclose(file);

  defrClear();

  if(res)
  {
    printf("Fail to parse %s\n", filename.c_str());
    exit(1);
  }
}

// Designs
int 
dbDefReader::defDesignCbk(defrCallbackType_e c, const char* name, defiUserData ud)
{
  checkType(c);
  dbDesign* design = (dbDesign*) ud;
  design->setName(name);
  return 0;
}

// Units
int 
dbDefReader::defUnitsCbk(defrCallbackType_e c, double unit, defiUserData ud)
{
  checkType(c);
  
  int defDbu = static_cast<int>(unit);
  dbDesign* design = (dbDesign*) ud;
  design->setDbu(defDbu);

  return 0;
}

// Die
int 
dbDefReader::defDieAreaCbk(defrCallbackType_e c, defiBox* box, defiUserData ud)
{
  checkType(c);
  dbDesign* design = (dbDesign*) ud;
  design->setDie(box);
  return 0;
}

// Rows
int 
dbDefReader::defRowCbk(defrCallbackType_e c, defiRow* ro, defiUserData ud)
{
  checkType(c);
  dbDesign* design = (dbDesign*) ud;
	design->addNewRow(ro);
  return 0;
}

// Nets
int 
dbDefReader::defNetStartCbk(defrCallbackType_e c, int number, defiUserData ud)
{
  checkType(c);
  return 0;
}

int 
dbDefReader::defNetCbk(defrCallbackType_e c, defiNet* net, defiUserData ud)
{
  checkType(c);
  return 0;
}

int
dbDefReader::defNetEndCbk(defrCallbackType_e c, void* , defiUserData ud)
{
  checkType(c);
  return 0;
}
    
// Special Nets
int 
dbDefReader::defSNetStartCbk(defrCallbackType_e c, int number, defiUserData ud)
{
  checkType(c);
  return 0;
}

int 
dbDefReader::defSNetCbk(defrCallbackType_e c, defiNet* net, defiUserData ud)
{
  checkType(c);
  return 0;
}

int 
dbDefReader::defSNetEndCbk(defrCallbackType_e c, void* , defiUserData ud)
{
  checkType(c);
  return 0;
}

// Pins
int 
dbDefReader::defPinStartCbk(defrCallbackType_e c, int  number, defiUserData ud)
{
  checkType(c);
  return 0;
}

int 
dbDefReader::defPinCbk(defrCallbackType_e c, defiPin* pi, defiUserData ud)
{
  checkType(c);
  return 0;
}

int 
dbDefReader::defPinEndCbk(defrCallbackType_e c, void*  , defiUserData ud)
{
  checkType(c);
  return 0;
}

// Components
int 
dbDefReader::defComponentStartCbk(defrCallbackType_e c, int number, defiUserData ud)
{
  checkType(c);
  return 0;
}

int 
dbDefReader::defComponentCbk(defrCallbackType_e c, defiComponent* co, defiUserData ud)
{
  checkType(c);
  dbDesign* design = (dbDesign*) ud;
 
	std::string nameWithoutBackSlash = removeBackSlashBracket( std::string(co->id()) );

	dbInst* inst = design->getInstByName( nameWithoutBackSlash );

	if(inst == nullptr)
	  design->addNewInst(co, nameWithoutBackSlash);
	else
		design->fillInst(co, inst);

  return 0;
}

int 
dbDefReader::defComponentEndCbk(defrCallbackType_e c, void* , defiUserData ud)
{
  checkType(c);
  return 0;
}

// End Design
int 
dbDefReader::defEndCbk(defrCallbackType_e c, void* , defiUserData ud)
{
  checkType(c);
  return 0;
}

}
