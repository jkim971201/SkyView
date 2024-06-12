#include "dbBookShelfReader.h"
#include "dbMacro.h"
#include "dbLayer.h"
#include "dbRect.h"

#include <map>

namespace db
{

dbBookShelfReader::dbBookShelfReader(std::shared_ptr<dbTypes>  types,
                                     std::shared_ptr<dbDesign> design)
  : types_  ( types),
    design_ (design)
{
  bsParser_ = std::make_unique<BookShelfParser>();
}

void
dbBookShelfReader::readFile(const std::string& filename)
{
  bsParser_->parse(filename);
  convert2db();
}

void
dbBookShelfReader::convert2db()
{
  // Note that BookShelf format does not have any
  // technology information.
  // Though some tech-related objects (e.g. dbMacro)
  // will be used in this function, but they are
  // just dummy objects to smoothly
  // convert bookshelf format to LEF/DEF-based database.
  
  const auto bookshelfDB = bsParser_->getDB();

  // Bookshelf contains micron unit
  // so we need a scaling factor to
  // convert to integer data of dbInst, dbDie, ...
  // 10 is just a magic number.
  constexpr int dbuBookShelf = 10;

  auto convert2dbDie = [&] (BsDie* bsDie)
  {
    auto dbDiePtr = design_->getDie();
    dbDiePtr->setLx(dbuBookShelf * bsDie->lx());
    dbDiePtr->setLy(dbuBookShelf * bsDie->ly());
    dbDiePtr->setUx(dbuBookShelf * bsDie->ux());
    dbDiePtr->setUy(dbuBookShelf * bsDie->uy());

    // Asuume BookShelf Format always have same core size with die
    design_->setCoreLx(dbuBookShelf * bsDie->lx());
    design_->setCoreLy(dbuBookShelf * bsDie->ly());
    design_->setCoreUx(dbuBookShelf * bsDie->ux());
    design_->setCoreUy(dbuBookShelf * bsDie->uy());
  };

  // This will be used to determine Macro
  const int rawRowHeight = bookshelfDB->rowHeight(); 
  
  int numRow = 0;
  auto convert2dbRow = [&] (BsRow* bsRow)
  {
    dbRow* newRow = new dbRow;
    const std::string row_name = "BookShelfRow" + std::to_string(numRow++);
    newRow->setName(row_name);
    newRow->setOrigX(dbuBookShelf * bsRow->lx());
    newRow->setOrigY(dbuBookShelf * bsRow->ly());
    newRow->setSiteSize(dbuBookShelf * bsRow->siteWidth(),  
                        dbuBookShelf * bsRow->dy());
    newRow->setStepX(dbuBookShelf * bsRow->siteSpacing());
    newRow->setStepY(dbuBookShelf * bsRow->dy());
    newRow->setNumSiteX(bsRow->numSites());
    newRow->setNumSiteY(1);
    return newRow;
  };

  // These dbMacros will not be registed to dbTech
  // These are only to avoid dbInst makes segmentation fault
  // due to dbInst methods that use dbMacro pointer.
  std::map<std::pair<int, int>, dbMacro*> size2macro;

  auto convert2dbInst = [&] (BsCell* bsCell)
  {
    dbInst* newInst = new dbInst;
    newInst->setName(bsCell->name());
    int macroSizeX = dbuBookShelf * bsCell->dx();
    int macroSizeY = dbuBookShelf * bsCell->dy();
    
    std::pair<int, int> macroSize = {macroSizeX, macroSizeY};

    dbMacro* macroPtr;
    auto itr = size2macro.find(macroSize);
    if(itr == size2macro.end())
    {
      macroPtr = new dbMacro;
      const std::string macro_name 
        = "BookShelfMacro" + std::to_string(size2macro.size());
      macroPtr->setName(macro_name.c_str());
      macroPtr->setSizeX(macroSizeX);
      macroPtr->setSizeY(macroSizeY);
      size2macro[macroSize] = macroPtr;
    }
    else
      macroPtr = itr->second;
  
    // Use raw value
    if(bsCell->dy() > rawRowHeight)
      macroPtr->setMacroClass(MacroClass::BLOCK);

    newInst->setMacro(macroPtr);

    newInst->setLocation(dbuBookShelf * bsCell->lx(), 
                         dbuBookShelf * bsCell->ly());

    if(bsCell->isFixed())
      newInst->setStatus(Status::FIXED);

    return newInst;
  };

  // dummy layer to make dbRect
  dbLayer* dummyLayer = new dbLayer; 

  auto convert2dbBTerm = [&] (BsCell* bsCell)
  {
    dbBTerm* newIO = new dbBTerm;
    newIO->setOrigX(0);
    newIO->setOrigY(0);  
    // Default orient is N
    newIO->addRect(dbRect(dbuBookShelf * bsCell->lx(),
                          dbuBookShelf * bsCell->ly(),
                          dbuBookShelf * bsCell->dx(),
                          dbuBookShelf * bsCell->dy(),
                          dummyLayer));
		newIO->setLocation();
		return newIO;
  };

  auto convert2dbNet = [&] (BsNet* bsNet)
  {
    
  };

  // Die
  convert2dbDie(bookshelfDB->getDie());

  // Row
  auto& dbRowVector = design_->getRows();
  auto& bsRowVector = bookshelfDB->rowVector();
  for(auto bsRowPtr : bsRowVector)
    dbRowVector.push_back(convert2dbRow(bsRowPtr));

  // Inst && BTerm (include ITerm)
  auto& dbInstVector = design_->getInsts();
  auto& bsCellVector = bookshelfDB->cellVector();
  for(auto bsCellPtr : bsCellVector)
	{
		// Bookshelf format does not have IOs explicitly.
		// we have detect them by context.
		if(bsParser_->isOutsideDie(bsCellPtr) || bsCellPtr->dx() == 0 || bsCellPtr->dy() == 0)
			convert2dbBTerm(bsCellPtr);
		else
			convert2dbInst(bsCellPtr);
	}

  // BTerm (IO)
  
  // Net
  
}

}
