#include <cmath>
#include <cassert>
#include <map>
#include <random>    // For mt19937
#include <algorithm> // For sort
#include <stdio.h>
#include <iostream>
#include <climits>   // For INT_MAX, INT_MIN
#include <cfloat>    // For FLT_MAX
#include <fstream>

#include "SkyPlaceDB.h"

namespace SkyPlace
{

using namespace LefDefDB;
using namespace BookShelf;

//////////////////////////////////////////////////////////

static float getOverlapArea(const Bin* bin, const Cell* cell)
{
  float rectLx = std::max(bin->lx(), cell->lx());
  float rectLy = std::max(bin->ly(), cell->ly());
  float rectUx = std::min(bin->ux(), cell->ux());
  float rectUy = std::min(bin->uy(), cell->uy());

  if(rectLx >= rectUx || rectLy >= rectUy)
    return 0;
  else
    return (rectUx - rectLx) * (rectUy - rectLy);
}

// getOverlapArea should use int64_t ideally,
// but runtime will be doubled (OpenROAD comment)
static float getOverlapAreaWithDensitySize(const Bin* bin, const Cell* cell)
{
  float rectLx = std::max(bin->lx(), cell->dLx());
  float rectLy = std::max(bin->ly(), cell->dLy());
  float rectUx = std::min(bin->ux(), cell->dUx());
  float rectUy = std::min(bin->uy(), cell->dUy());

  if(rectLx >= rectUx || rectLy >= rectUy)
    return 0;
  else
    return ((rectUx - rectLx) * (rectUy - rectLy));
}

//////////////////////////////////////////////////////////

// Cell // 
Cell::Cell()
  : id_            (      0),
    cluster_id_    (      0),
    cx_            (      0), 
    cy_            (      0),
    dx_            (      0), 
    dy_            (      0),
    dDx_           (      0), 
    dDy_           (      0),
    densityScale_  (      1),
    isIO_          (  false),
    isMacro_       (  false), 
    isFixed_       (  false),
    isFiller_      (  false),
    bsCell_        (nullptr),
    dbCell_        (nullptr)
{}

Cell::Cell(dbIO* io) : Cell()
{
  dbIO_ = io;

  dx_ = static_cast<float>(io->dx());
  dy_ = static_cast<float>(io->dy());

  cx_ = static_cast<float>(io->lx() + dx_ / 2);
  cy_ = static_cast<float>(io->ly() + dy_ / 2);

  isIO_    = true;
  isFixed_ = true;
}

Cell::Cell(dbCell* cell) : Cell()
{
  dbCell_ = cell;

  isFixed_ = cell->isFixed();
  isMacro_ = cell->isMacro();

  if( !isMacro_ )
  {
    dx_ = static_cast<float>(cell->dx());
    dy_ = static_cast<float>(cell->dy());

    cx_ = static_cast<float>(cell->lx() + dx_ / 2);
    cy_ = static_cast<float>(cell->ly() + dy_ / 2);
  }
  else
  {
    dx_ = static_cast<float>(cell->dx() 
                           + cell->haloR() 
                           + cell->haloL() );

    dy_ = static_cast<float>(cell->dy() 
                           + cell->haloT() 
                           + cell->haloB() );

    cx_ = static_cast<float>(cell->lx() - cell->haloL() + dx_ / 2);
    cy_ = static_cast<float>(cell->ly() - cell->haloB() + dy_ / 2);
  }
}

// height := criterion of macro
Cell::Cell(BsCell* bsCell, float height) : Cell()
{
  bsCell_ = bsCell;

  float bsCellDx = bsCell->dx();
  float bsCellDy = bsCell->dy();

  dx_ = static_cast<float>(bsCellDx);
  dy_ = static_cast<float>(bsCellDy);

  cx_ = static_cast<float>(bsCell->lx() + dx_ / 2);
  cy_ = static_cast<float>(bsCell->ly() + dy_ / 2);

  isFixed_ = bsCell->isFixed();

  if(bsCellDy > height) 
    isMacro_ = true;
  else if(bsCellDy < 1.0)  
    isIO_ = true; 
  // In the MMS benchmark, IOs have zero size
}

// Constructor for Filler Cell
Cell::Cell(float cx, float cy, float dx, float dy) : Cell()
{
  cx_ = cx;
  cy_ = cy;

  dx_ = dx;
  dy_ = dy;

  isFixed_  = false;
  isMacro_  = false;
  isFiller_ = true;
}

void 
Cell::setCenterLocation(float newCx, float newCy)
{
  cx_ = newCx;
  cy_ = newCy;

  for(auto &pin : pins_)
    pin->updatePinLocation(this);
}

void 
Cell::setDensitySize(float dWidth, float dHeight, float dScale)
{
  dDx_ = dWidth;
  dDy_ = dHeight;
  densityScale_ = dScale;
}

// Pin // 
Pin::Pin()
  : io_           (    'I'), 
    id_           (      0), 
    cx_           (      0), 
    cy_           (      0),
    isIO_         (  false),
    offsetX_      (      0), 
    offsetY_      (      0),
    isMinPinX_    (  false), 
    isMinPinY_    (  false),
    isMaxPinX_    (  false), 
    isMaxPinY_    (  false),
    net_          (nullptr), 
    cell_         (nullptr),
    dbPin_        (nullptr),
    bsPin_        (nullptr)
{}

Pin::Pin(dbPin* pin, int id) : Pin()
{
  id_ = id;
  // io_ is not necessary right now...
  dbPin_   = pin;
  offsetX_ = static_cast<float>(pin->offsetX());
  offsetY_ = static_cast<float>(pin->offsetY());
}

Pin::Pin(BsPin* bsPin, int id) : Pin()
{
  id_ = id;
  io_ = bsPin->IO();
  bsPin_ = bsPin;
  offsetX_ = static_cast<float>(bsPin->offsetX());
  offsetY_ = static_cast<float>(bsPin->offsetY());
}

void
Pin::updatePinLocation(Cell* cell) 
{
  // if we use cell_ instead of given Cell*,
  // we have to check whether cell_ is not nullptr 
  // everytime we call this function
  cx_ = cell->cx() + offsetX_;
  cy_ = cell->cy() + offsetY_;
}

// Net //
Net::Net()
  : id_      (0      ),
    lx_      (INT_MAX), 
    ly_      (INT_MAX), 
    ux_      (INT_MIN), 
    uy_      (INT_MIN),
    weight_  (1.0    ),
    dbNet_   (nullptr),
    bsNet_   (nullptr)
{}

Net::Net(dbNet* net, int id) : Net()
{
  id_     =  id;
  weight_ = 1.0;
  dbNet_  = net;
}

Net::Net(BsNet* bsNet, int id) : Net()
{
  id_     =  id;
  weight_ = 1.0;
  bsNet_  = bsNet;
}

void
Net::updateBBox()
{
  if( pins_.size() == 0 )
  {
    std::cout << "Warning - " << dbNetPtr()->name() << " has no pins.\n";
    lx_ = ly_ = ux_ = uy_ = 0;
    return;
  }

  // To detect an error,
  // We initilize them as INT_MAX
  // so that an un-initilized net will
  // make an invalid HPWL
  lx_ = ly_ = FLT_MAX;
  ux_ = uy_ = 0;

  for(auto& p : pins_)
  {
    lx_ = std::min(p->cx(), lx_);
    ly_ = std::min(p->cy(), ly_);
    ux_ = std::max(p->cx(), ux_);
    uy_ = std::max(p->cy(), uy_);
  }
}

void
Die::init(const BsDie* bsDie)
{
  lx_ = static_cast<float>(bsDie->lx());
  ly_ = static_cast<float>(bsDie->ly());
  ux_ = static_cast<float>(bsDie->ux());
  uy_ = static_cast<float>(bsDie->uy());
}

void
Die::init(const dbDie* die)
{
  lx_ = static_cast<float>(die->coreLx());
  ly_ = static_cast<float>(die->coreLy());
  ux_ = static_cast<float>(die->coreUx());
  uy_ = static_cast<float>(die->coreUy());
}

// Bin //
Bin::Bin()
  : row_    (0), 
    col_    (0),
    lx_     (0), 
    ly_     (0),
    ux_     (0), 
    uy_     (0),
    lambda_           (1.0),
    fixedArea_        (0),
    movableArea_      (0),
    density_          (0),
    targetDensity_    (0),
    electroForceX_    (0),
    electroForceY_    (0),
    electroPotential_ (0)
{}

Bin::Bin(int row , int col , 
         float lx, float ly, 
         float ux, float uy,
         float targetDensity) : Bin()
{
  row_ = row;
  col_ = col;
  lx_ = lx;
  ly_ = ly;
  ux_ = ux;
  uy_ = uy;
  targetDensity_ = targetDensity;
}

// Row //
Row::Row(dbRow* row)
{
  numSiteX_ = row->numSiteX();
  numSiteY_ = row->numSiteY();

  lx_ = row->origX();
  ly_ = row->origY();

  ux_ = lx_ + row->sizeX();
  uy_ = ly_ + row->sizeY();

  stepX_ = row->stepX();
  stepY_ = row->stepY();

  siteWidth_ = row->siteWidth();
}

Row::Row(BsRow* row)
{
  numSiteX_ = row->numSites();
  numSiteY_ = 1; 
  // NumSitesY does not exist in Bookshelf .scl

  lx_ = row->lx();
  ly_ = row->ly();

  ux_ = lx_ + row->ux();
  uy_ = ly_ + row->uy();

  stepX_ = row->siteWidth();
  stepY_ = row->dy();
}

// SkyPlaceDB // 
SkyPlaceDB::SkyPlaceDB()
  : dbLefDef_             (nullptr),
    dbBS_                 (nullptr),
    dbUnit_               (      1),
    designName_           (     ""),
    targetDensity_        (    1.0),
    maxRowHeight_         (      0),
    numStdCells_          (      0), 
    numMacro_             (      0),
    numFixed_             (      0), 
    numMovable_           (      0), 
    numMovableMacro_      (      0),
    numFiller_            (      0),
    numBinX_              (      0), 
    numBinY_              (      0), 
    numIO_                (      0),
    numCluster_           (      0),
    sumMovableArea_       (      0), 
    sumMovableStdArea_    (      0), 
    sumMovableMacroArea_  (      0), 
    sumScaledMovableArea_ (      0), 
    sumFixedArea_         (      0),
    hpwl_                 (      0), 
    binX_                 (      0), 
    binY_                 (      0),
    lefDefFlag_           (  false),
    bookShelfFlag_        (  false), 
    diePtr_               (nullptr),
    fillerWidth_          (      0), 
    fillerHeight_         (      0),
    numInitStep_          (      5)
{}

void
SkyPlaceDB::bookShelfDBtoSkyPlaceDB()
{
  // Step #1: Build Cell Vector
  int numCells = dbBS_->numCells();
  cellInsts_.resize(numCells);
  cellPtrs_.reserve(numCells);
  fixedPtrs_.reserve(numFixed_);
  movablePtrs_.reserve(numMovable_);

  int cIdx = 0;
  int fixedIdx   = 0;
  int movableIdx = 0;
  for(auto& cellInst : cellInsts_)
  {
    BsCell* bsCell = dbBS_->cellVector()[cIdx];
    cellInst = Cell(bsCell, maxRowHeight_);
    cellPtrs_.push_back(&cellInst);
    cellMap_[bsCell->id()] = &cellInst;
    cIdx++;

    // For Information
    if(cellInst.isFixed()) 
    {
      cellInst.setID(fixedIdx++);
      fixedPtrs_.push_back(&cellInst);
      if(!isOutsideDie(&cellInst))
        sumFixedArea_ += cellInst.area();
      else
      {
        // Assign as an IO pin
        // ( ISPD 2005 adaptec benchmarks )
        numIO_++;
        cellInst.setIO(true);
      }

      if( cellInst.isMacro() )
        numMacro_++;

    }
    else                    
    {
      cellInst.setID(movableIdx++);
      movablePtrs_.push_back( &cellInst );
      auto cellArea = cellInst.area();
      sumMovableArea_ += cellArea;

      if( cellInst.isMacro() )
      {
        sumMovableMacroArea_  += cellArea;
        sumScaledMovableArea_ += cellArea * targetDensity_;
        numMacro_++;
        numMovableMacro_++;
      }
      else 
      {
        sumMovableStdArea_    += cellArea;
        sumScaledMovableArea_ += cellArea;
        numStdCells_++;
      }
    }

    if( cellInst.isIO() )
      numIO_++;
  }

  //printf("fixedIdx: %d numFixed_: %d numMacro_: %d\n", fixedIdx, numFixed_, numMacro_);

  assert(fixedIdx == numFixed_);
  assert(movableIdx == numMovable_);

  // Step #2: Build Net Vector
  int numNets = dbBS_->netVector().size();
  netInsts_.resize(numNets);
  netPtrs_.reserve(numNets);

  int nIdx = 0;
  for(auto& netInst : netInsts_)
  {
    BsNet* bsNet = dbBS_->netVector()[nIdx];
    netInst = Net(bsNet, nIdx);
    netPtrs_.push_back(&netInst);
    netMap_[nIdx] = &netInst;
    nIdx++;
  }

  // Step #3: Build Pin Vector
  int numPins = dbBS_->pinVector().size();
  pinInsts_.resize(numPins);
  pinPtrs_.reserve(numPins);

  int pIdx = 0;

  auto netMapEnd  = netMap_.end();
  auto cellMapEnd = cellMap_.end();

  for(auto& pinInst : pinInsts_)
  {
    BsPin* bsPin = dbBS_->pinVector()[pIdx];
    pinInst = Pin(bsPin, pIdx++);
    pinPtrs_.push_back(&pinInst);

    int bsNetID  = bsPin->net()->id();
    int bsCellID = bsPin->cell()->id();

    auto findNet  = netMap_.find(bsNetID);
    auto findCell = cellMap_.find(bsCellID);

    if( findNet != netMapEnd )
    {
      auto netPtr = findNet->second;
      netPtr->addNewPin(&pinInst);
      pinInst.setNet(netPtr);
    }
    else
    {
      printf("Net DB Construction Error.\n");
      exit(0);
    }

    if( findCell != cellMapEnd )
    {
      auto cellPtr = findCell->second;
      cellPtr->addNewPin(&pinInst);
      pinInst.setCell(cellPtr);
    }
    else
    {
      printf("Cell DB Construction Error.\n");
      exit(0);
    }
  }

  // Step #4: Initialize Pin Location and NetBBox
  updateHpwl();

  // Step #5: Make Rows
  int numRow = dbBS_->numRows();
  rowInsts_.resize(numRow);
  rowPtrs_.reserve(numRow);

  int rowIdx = 0;
  for(auto& row : rowInsts_)
  {
    row = Row(dbBS_->rowVector()[rowIdx++]);
    rowPtrs_.push_back(&row);
  }
}

void
SkyPlaceDB::lefDefDBtoSkyPlaceDB()
{
  // Step #1: Build Cell Vector
  int numCell    = dbLefDef_->numInst();
  int numIO      = dbLefDef_->ios().size();

  int numInst    = numCell + numIO;
  int numFixed   = dbLefDef_->numFixed() + numIO + dbLefDef_->numDummy();
  int numMovable = numInst - numFixed;

  cellInsts_.resize(numInst);
  cellPtrs_.reserve(numInst);
  fixedPtrs_.reserve(numFixed);
  movablePtrs_.reserve(numMovable);

  int cIdx = 0;
  int fixedIdx   = 0;
  int movableIdx = 0;

  int cellVectorSize = dbLefDef_->cells().size();

  for( auto& cellInst : cellInsts_ )
  {
    if( cIdx < numCell )
    {
      dbCell* cell = dbLefDef_->cells()[cIdx];
      cellInst = Cell(cell);
      cellMap_[cell->id()] = &cellInst;
    }
    else
    {
      dbIO* io = dbLefDef_->ios()[cIdx - numCell];
      cellInst = Cell(io);
      cellMap_[io->id() + numCell] = &cellInst;
    }

    cellPtrs_.push_back(&cellInst);
    cIdx++;

    // For Information
    if(cellInst.isFixed())
    {
      cellInst.setID(fixedIdx++);
      fixedPtrs_.push_back(&cellInst);
      if(!isOutsideDie(&cellInst))
        sumFixedArea_ += cellInst.area();

      if( cellInst.isMacro() )
        numMacro_++;
    }
    else                    
    {
      cellInst.setID(movableIdx++);
      movablePtrs_.push_back( &cellInst );
      auto cellArea = cellInst.area();
      sumMovableArea_ += cellArea;

      if( cellInst.isMacro() )
      {
        sumMovableMacroArea_  += cellArea;
        sumScaledMovableArea_ += cellArea * targetDensity_;
        numMacro_++;
        numMovableMacro_++;
      }
      else 
      {
        sumMovableStdArea_    += cellArea;
        sumScaledMovableArea_ += cellArea;
        numStdCells_++;
      }
    }

    if( cellInst.isIO() )
      numIO_++;
  }

  //printf("fixedIdx: %d numFixed_: %d numMacro_: %d\n", fixedIdx, numFixed_, numMacro_);
  //printf("movableIdx: %d numMovable_: %d\n", movableIdx, numMovable_);

  assert(fixedIdx == numFixed_);
  assert(movableIdx == numMovable_);

  // Step #2: Build Net Vector
  int numNets = dbLefDef_->nets().size();
  netInsts_.resize(numNets);
  netPtrs_.reserve(numNets);

  int nIdx = 0;
  for(auto& netInst : netInsts_)
  {
    dbNet* net = dbLefDef_->nets()[nIdx];
    netInst = Net(net, nIdx);
    netPtrs_.push_back( &netInst );
    netMap_[net->id()] = &netInst;
    nIdx++;
  }

  // Step #3: Build Pin Vector
  int numPins = dbLefDef_->pins().size();
  pinInsts_.resize(numPins);
  pinPtrs_.reserve(numPins);

  int dbIOID   = 0;
  int dbNetID  = 0;
  int dbCellID = 0;

  int  netIDForThisPin = 0;
  int cellIDForThisPin = 0;

  int pIdx = 0;

  for( auto& dbNetPtr : dbLefDef_->nets() )
  {
    int dbNetID = dbNetPtr->id();

    for( auto& dbPinPtr : dbNetPtr->pins() )
    {
      if( dbPinPtr->isExternal() )
      {
        dbIOID = dbPinPtr->ioid();
        cellIDForThisPin = dbIOID + numCell;
      }
      else
      {
        dbCellID = dbPinPtr->cid();
        cellIDForThisPin = dbCellID;
      }

      auto findNet  = netMap_.find(dbNetID);
      auto findCell = cellMap_.find(cellIDForThisPin);

      if( findNet == netMap_.end() || findCell == cellMap_.end() )
      {
        if(findNet  ==  netMap_.end() )
          std::cout << "Net " << dbNetPtr->name() << " does not exist\n";
        if(findCell == cellMap_.end() ) 
        {
          if( dbPinPtr->isExternal() )
            std::cout << "IO " << dbLefDef_->ios()[dbIOID] << " does not exist\n";
          else
            std::cout << "Cell " << dbLefDef_->cells()[dbCellID] << " does not exist\n";
        }
        exit(0);
      }

      Pin* pinPtr = &(pinInsts_[pIdx]);
      int dbPinID = dbPinPtr->id();

      auto netPtr  = findNet->second;
      auto cellPtr = findCell->second;

      *pinPtr = Pin( dbLefDef_->pins()[dbPinID] , pIdx );

      netPtr->addNewPin(pinPtr);
      cellPtr->addNewPin(pinPtr);

      pinPtr->setNet(netPtr);
      pinPtr->setCell(cellPtr);

      pinPtrs_.push_back( pinPtr );
      pIdx++;
    }
  }

  // Step #4: Initialize Pin Location and NetBBox
  updateHpwl();

  // Step #5: Make Rows
  int numRow = dbLefDef_->numRow();
  rowInsts_.resize(numRow);
  rowPtrs_.reserve(numRow);

  int rowIdx = 0;
  for(auto& row : rowInsts_)
  {
    row = Row(dbLefDef_->rows()[rowIdx++]);
    rowPtrs_.push_back(&row);
  }
}

void
SkyPlaceDB::setDB(std::shared_ptr<BookShelfParser> bsParser)
{
  printf("Start SkyPlaceDB Initialization.\n");

  dbBS_        = bsParser;
  dbUnit_      = 1;
  designName_  = std::string(bsParser->getBenchName());
  designDir_   = std::string(bsParser->getDir() );

  maxRowHeight_ = dbBS_->rowHeight();
  if(maxRowHeight_ <= 0)
  {
    printf("Error - Row Height must be larger than 0!\n");
    exit(0);
  }

  bookShelfFlag_ = true;
  lefDefFlag_    = false;

  die_.init(dbBS_->getDie());
  diePtr_ = &die_;

  numFixed_   = dbBS_->numFixed() + dbBS_->numFixedNI();
  numMovable_ = dbBS_->numMovable();

  bookShelfDBtoSkyPlaceDB();
  printf("BookShelf to SkyPlaceDB   ---> Finished. (Step 1 / %d)\n", numInitStep_);

  init();
  printf("SkyPlaceDB is initilazed successfully!\n");

  printInfo();
}

void
SkyPlaceDB::setDB(std::shared_ptr<LefDefParser> parser)
{
  printf("Start SkyPlaceDB Initialization.\n");

  dbLefDef_    = parser;
  dbUnit_      = parser->dbUnit();
  designName_  = std::string( parser->designName() );
  designDir_   = std::string( parser->getDir() );

  // Assume that every row has same height
  maxRowHeight_ = dbLefDef_->rows()[0]->sizeY();
  if(maxRowHeight_ <= 0)
  {
    printf("Error - Row Height must be larger than 0!\n");
    exit(0);
  }

  bookShelfFlag_ = false;
  lefDefFlag_    = true;

  die_.init( dbLefDef_->die() );
  diePtr_ = &die_;

  numFixed_   = dbLefDef_->numFixed() + dbLefDef_->ios().size() + dbLefDef_->numDummy();
  numMovable_ = dbLefDef_->numInst() - dbLefDef_->numFixed() - dbLefDef_->numDummy();

  lefDefDBtoSkyPlaceDB();
  printf("LefDefDB to SkyPlaceDB    ---> Finished. (Step 1 / %d)\n", numInitStep_);

  init();
  printf("SkyPlaceDB is initilazed successfully!\n");

  printInfo();
}

void
SkyPlaceDB::updateHpwl()
{
  // Initialize Pin Location
  for(auto& c : cellPtrs_)
    for(auto& p : c->pins())
      p->updatePinLocation(c);

  hpwl_ = 0;

  // Initilize Net BBox
  for(auto& n : netPtrs_)
  {
    n->updateBBox();
    hpwl_ += n->hpwl();
  }
}

float
SkyPlaceDB::updateBBoxAndGetHpwl()
{
  hpwl_ = 0;
  // Initilize Net BBox
  for(auto& n : netPtrs_)
  {
    n->updateBBox();
    hpwl_ += n->hpwl();
  }
  return hpwl_;
}

bool
SkyPlaceDB::isOutsideDie(const Cell* cell)
{
  bool isOutside = false;

  if(cell->ux() <= die_.lx()) isOutside = true;
  if(cell->lx() >= die_.ux()) isOutside = true;
  if(cell->uy() <= die_.ly()) isOutside = true;
  if(cell->ly() >= die_.uy()) isOutside = true;

  return isOutside;
}

void
SkyPlaceDB::updatePinBound()
{
  for(auto& net : netPtrs_)
  {
    Pin* minPinX = nullptr;
    Pin* minPinY = nullptr;
    Pin* maxPinX = nullptr;
    Pin* maxPinY = nullptr;

    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float maxX = 0;
    float maxY = 0;

    for(auto& pin : net->pins())
    {
      float cx = pin->cx();
      float cy = pin->cy();

      if(cx <= minX)
      {
        if(minPinX != nullptr)
          minPinX->unsetMinPinX();
        minX = cx;
        pin->setMinPinX();
        minPinX = pin;
      }

      if(cy <= minY)
      {
        if(minPinY != nullptr)
          minPinY->unsetMinPinY();
        minY = cy;
        pin->setMinPinY();
        minPinY = pin;
      }

      if(cx > maxX)
      {
        if(maxPinX != nullptr)
          maxPinX->unsetMaxPinX();
        maxX = cx;
        pin->setMaxPinX();
        maxPinX = pin;
      }

      if(cy > maxY)
      {
        if(maxPinY != nullptr)
          maxPinY->unsetMaxPinY();
        maxY = cy;
        pin->setMaxPinY();
        maxPinY = pin;
      }
    }
  }
}

void
SkyPlaceDB::init()
{
  // Step#1: Bin Initialization
  createBins();
  printf("Bin Grid Initialization ---> Finished. (Step 2 / %d)\n", numInitStep_); 
  
  // Step#2: Filler Insertion
  createFillers();
  printf("Filler Cell Insertion   ---> Finished. (Step 3 / %d)\n", numInitStep_); 

  // Step#3: Update Density Size
  updateDensitySize();
  printf("Density Size Update     ---> Finished. (Step 4 / %d)\n", numInitStep_); 

  // Step#4: Update Fixed Overlap Area
  updateFixedOverlapArea();
  printf("FixedOverlapArea Update ---> Finished. (Step 5 / %d)\n", numInitStep_); 
}

void
SkyPlaceDB::createBins()
{
  sumTotalInstArea_ = sumFixedArea_ + sumMovableArea_;
  dieArea_ = die_.area();
  density_ = sumTotalInstArea_ / dieArea_;
  util_    = sumMovableArea_   / (dieArea_ - sumFixedArea_);

  avgCellArea_    =  (sumMovableArea_ + sumFixedArea_) / (numMovable_  + numFixed_);
  avgStdCellArea_ =  (sumMovableArea_) / static_cast<float>(numMovable_);

  // Do not use density_, rather use TargetDensity 
  idealBinAreaForAvgCellArea_    = avgCellArea_    / targetDensity_;
  idealBinAreaForAvgStdCellArea_ = avgStdCellArea_ / targetDensity_;

  idealBinCountForAvgCellArea_    = dieArea_ / idealBinAreaForAvgCellArea_;
  idealBinCountForAvgStdCellArea_ = dieArea_ / idealBinAreaForAvgStdCellArea_;

  idealNumBinForAvgCellArea_      = std::sqrt(idealBinCountForAvgCellArea_);
  idealNumBinForAvgStdCellArea_   = std::sqrt(idealBinCountForAvgStdCellArea_);

  //idealNumBin_ = std::ceil(idealNumBinForAvgCellArea_);
  idealNumBin_ = std::ceil(idealNumBinForAvgStdCellArea_);

  float dieX = die_.dx();
  float dieY = die_.dy();

  float aspect_ratio = std::max(dieX / dieY, dieY / dieX);
  int ratio = 1;

  if(aspect_ratio >= 4.0)
    ratio = 4;
  else if(aspect_ratio >= 2.0)
    ratio = 2;
  else 
    ratio = 1;

  int numBin = 4; 
  while(4 * numBin * (numBin * ratio) < idealBinCountForAvgStdCellArea_)
  {
    numBin *= 2;
    if(numBin >= 8192)
      break;
  }
	numBin /= 2;

  if(dieX > dieY)
  {
    numBinX_ = numBin * ratio;
    numBinY_ = numBin;
  }
  else
  {
    numBinX_ = numBin;
    numBinY_ = numBin * ratio;
  }

  binInsts_.resize(numBinX_ * numBinY_);
  binPtrs_.reserve(numBinX_ * numBinY_);

  float binSizeX = die_.dx() / static_cast<float>(numBinX_);
  float binSizeY = die_.dy() / static_cast<float>(numBinY_);

  float dieUx = die_.ux();
  float dieLx = die_.lx();
  float dieUy = die_.uy();
  float dieLy = die_.ly();

  float lx = dieLx, ly = dieLy;
  int numCreated = 0;

  for(auto& bin : binInsts_)
  {  
    float binWidth  = binSizeX;
    float binHeight = binSizeY;

    int row = numCreated / numBinX_;
    int col = numCreated % numBinX_;

    if(col == (numBinX_ - 1) || (lx + binSizeX > dieUx))
      binWidth  = dieUx - lx;

    if(row == (numBinY_ - 1) || (ly + binSizeY > dieUy))
      binHeight = dieUy - ly;

    bin = Bin(row, col, lx, ly, 
              lx + binWidth, ly + binHeight, 
              targetDensity_);

    lx += binWidth;
    
    if(lx >= dieUx)
    {
      ly += binHeight;
      lx = dieLx;
    }

    binPtrs_.push_back(&bin);
    numCreated++;
  }

  binX_  = binSizeX;
  binY_  = binSizeY;
}

void
SkyPlaceDB::createFillers()
{
  float dxSum = 0;
  float dySum = 0;

  std::vector<float> dxList;
  std::vector<float> dyList;

  dxList.reserve(numMovable_);
  dyList.reserve(numMovable_);

  for(auto& cell : movablePtrs_)
  {
    dxList.push_back(cell->dx());
    dyList.push_back(cell->dy());
  }

  std::sort(dxList.begin(), dxList.end());

  int minIdx = static_cast<int>(static_cast<float>(dxList.size()) * 0.10);
  int maxIdx = static_cast<int>(static_cast<float>(dxList.size()) * 0.90);

  // if numMovable is too small
  if(minIdx == maxIdx) 
  {
    minIdx = 0;
    maxIdx = numMovable_;
  }

  for(int i = minIdx; i < maxIdx; i++)
  {
    dxSum += dxList[i];
    dySum += dyList[i];
  }

  fillerWidth_   = dxSum / static_cast<float>(maxIdx - minIdx);
  fillerHeight_  = dySum / static_cast<float>(maxIdx - minIdx);

  avgFillerArea_  = fillerWidth_ * fillerHeight_;
  whiteSpaceArea_ = (die_.area() - sumFixedArea_);
  fillerArea_     = whiteSpaceArea_ * targetDensity_ 
                  - sumScaledMovableArea_;

  if(fillerArea_ < 0)
  {
    printf("Error - FillerArea is smaller than 0!\n");
    printf("Use higher placement density...\n");
    exit(0);
  }

  numFiller_ = static_cast<int>(fillerArea_ / avgFillerArea_);

  std::mt19937 randVal(0);

  int dieLx = static_cast<int>(die_.lx());
  int dieLy = static_cast<int>(die_.ly());
  int dieWidth  = static_cast<int>(die_.dx());
  int dieHeight = static_cast<int>(die_.dy());

  // Rest of this function is incredibly stupid...
  // TODO: Fix these stupid parts
  for(int i = 0; i < numFiller_; i++)
  {
    auto randX = randVal();
    auto randY = randVal();
    
    // Random distribution over the entire layout
    Cell filler(randX % dieWidth  + dieLx, 
                randY % dieHeight + dieLy,
                fillerWidth_, fillerHeight_);

    cellInsts_.push_back(filler);
  }

  cellPtrs_.clear();
  cellPtrs_.reserve(cellInsts_.size());

  numMovable_ += numFiller_;
  
  movableMacroPtrs_.clear();
  movableMacroPtrs_.reserve(numMovableMacro_);

  movablePtrs_.clear();
  movablePtrs_.reserve(numMovable_);

  fixedPtrs_.clear();
  fixedPtrs_.reserve(numFixed_);

  int fixedID   = 0;
  int movableID = 0;

  for(auto& cell : cellInsts_)
  {
    cellPtrs_.push_back(&cell);
    if(!cell.isFixed())
    {
      cell.setID(movableID++);
      movablePtrs_.push_back(&cell);

      if( cell.isMacro() )
        movableMacroPtrs_.push_back(&cell);
    }
    else
    {
      cell.setID(fixedID++);
      fixedPtrs_.push_back(&cell);
    }
    for(auto & pin : cell.pins())
      pin->setCell(&cell);
  }
}

OverlapBins
SkyPlaceDB::findBin(const Cell* cell)
{
  float lx = cell->lx();
  float ux = cell->ux();
  
  int minX = std::floor((lx - die_.lx()) / binX_);
  int maxX = std::ceil((ux - die_.lx()) / binX_);

  minX = std::max(minX, 0);
  maxX = std::min(numBinX_, maxX);

  std::pair<int, int> minMaxX = std::make_pair(minX, maxX);

  float ly = cell->ly();
  float uy = cell->uy();
  
  int minY = std::floor((ly - die_.ly()) / binY_);
  int maxY = std::ceil((uy - die_.ly()) / binY_);

  minY = std::max(minY, 0);
  maxY = std::min(numBinY_, maxY);

  std::pair<int, int> minMaxY = std::make_pair(minY, maxY);

  return std::make_pair(minMaxX, minMaxY);
}

OverlapBins
SkyPlaceDB::findBinWithDensitySize(const Cell* cell)
{
  float lx = cell->dLx();
  float ux = cell->dUx();
  
  int minX = std::floor((lx - die_.lx()) / binX_);
  int maxX = std::ceil((ux - die_.lx()) / binX_);

  minX = std::max(minX, 0);
  maxX = std::min(numBinX_, maxX);

  std::pair<int, int> minMaxX = std::make_pair(minX, maxX);

  float ly = cell->dLy();
  float uy = cell->dUy();

  int minY = std::floor((ly - die_.ly()) / binY_);
  int maxY = std::ceil((uy - die_.ly()) / binY_);

  minY = std::max(minY, 0);
  maxY = std::min(numBinY_, maxY);

  std::pair<int, int> minMaxY = std::make_pair(minY, maxY);

  return std::make_pair(minMaxX, minMaxY);
}

bool
SkyPlaceDB::isOverlap(const Cell* cell, const Bin* bin)
{
  bool checkX = false;
  bool checkY = false;

  if((cell->lx() >= bin->lx()) && (cell->lx() <= bin->ux()))
    checkX = true;
  if((cell->ux() >= bin->lx()) && (cell->ux() <= bin->ux()))
    checkX = true;
  if((cell->ly() >= bin->ly()) && (cell->ly() <= bin->uy()))
    checkY = true;
  if((cell->uy() >= bin->ly()) && (cell->uy() <= bin->uy()))
    checkY = true;

  return checkX && checkY;
}

void
SkyPlaceDB::moveCellInsideLayout(Cell* cell)
{
  cell->setCenterLocation(getXCoordiInsideLayout(cell), 
                          getYCoordiInsideLayout(cell));
}

float
SkyPlaceDB::getXCoordiInsideLayout(const Cell* cell)
{
  float newCx = cell->cx();

  if(cell->lx() < die_.lx())
    newCx = die_.lx() + cell->dx()/2;
  if(cell->ux() > die_.ux())
    newCx = die_.ux() - cell->dx()/2;

  return newCx;
}

float
SkyPlaceDB::getYCoordiInsideLayout(const Cell* cell)
{
  float newCy = cell->cy();

  if(cell->ly() < die_.ly())
    newCy = die_.ly() + cell->dy()/2;
  if(cell->uy() > die_.uy())
    newCy = die_.uy() - cell->dy()/2;

  return newCy;
}

float
SkyPlaceDB::getXCoordiInsideLayout(const Cell* cell, float cx)
{
  float newCx = cx;

  if(cx - cell->dx()/2 < die_.lx())
    newCx = die_.lx() + cell->dx()/2;
  if(cx + cell->dx()/2 > die_.ux())
    newCx = die_.ux() - cell->dx()/2;
  return newCx;
}

float
SkyPlaceDB::getYCoordiInsideLayout(const Cell* cell, float cy)
{
  float newCy = cy;

  if(cy - cell->dy()/2 < die_.ly())
    newCy = die_.ly() + cell->dy()/2;
  if(cy + cell->dy()/2 > die_.uy())
    newCy = die_.uy() - cell->dy()/2;
  return newCy;
}

float
SkyPlaceDB::getXDensityCoordiInsideLayout(const Cell* cell)
{
  float newCx = cell->cx();

  if(cell->dLx() < die_.lx())
    newCx = die_.lx() + cell->dDx()/2;
  if(cell->dUx() > die_.ux())
    newCx = die_.ux() - cell->dDx()/2;

  return newCx;
}

float
SkyPlaceDB::getYDensityCoordiInsideLayout(const Cell* cell)
{
  float newCy = cell->cy();

  if(cell->dLy() < die_.ly())
    newCy = die_.ly() + cell->dDy()/2;
  if(cell->dUy() > die_.uy())
    newCy = die_.uy() - cell->dDy()/2;

  return newCy;
}

void
SkyPlaceDB::updateDensitySize()
{
  float scaleX = 0, scaleY = 0;
  float densityW = 0, densityH = 0;

  for(auto& cell: cellPtrs_)
  {
    if(cell->dx() > DENSITY_SCALE * binX_)
    {
      scaleX   = 1.0;
      densityW = cell->dx();
    }
    else
    {
      scaleX   = cell->dx() / (DENSITY_SCALE * binX_);
      densityW = DENSITY_SCALE * binX_;
    }

    if(cell->dy() > DENSITY_SCALE * binY_)
    {
      scaleY   = 1.0;
      densityH = cell->dy();
    }
    else
    {
      scaleY   = cell->dy() / (DENSITY_SCALE * binY_);
      densityH = DENSITY_SCALE * binY_;
    }
    cell->setDensitySize(densityW, densityH, scaleX * scaleY);
  }
}

void
SkyPlaceDB::updateMacroDensityWeight(float macroWeight)
{
  if(numMovableMacro_ == 0)
    return;

  float avgMacroArea = sumMovableMacroArea_ 
                     / static_cast<float>(numMovableMacro_);

  float maxMacroArea = 0.0;
  float minMacroArea = FLT_MAX;

  float sumMacroAreaVarianceSquare = 0.0;

  for(auto& macro : movableMacroPtrs_)
  {
    float area = macro->area();
    sumMacroAreaVarianceSquare += (area - avgMacroArea) 
                                * (area - avgMacroArea);
  
    if(area > maxMacroArea)
      maxMacroArea = area;
    if(area < minMacroArea)
      minMacroArea = area;
  }

  float std_dev = std::sqrt( sumMacroAreaVarianceSquare / 
                             static_cast<float>( numMovableMacro_ ) );

  float maxRatio = maxMacroArea / avgMacroArea;
  float minRatio = minMacroArea / avgMacroArea;

  float ratio_std_macro = sumMovableMacroArea_ / sumMovableStdArea_;

  std::cout << "Avg MacroArea   : " << avgMacroArea << std::endl;
  std::cout << "Std_Dev         : " << std_dev << std::endl;
  std::cout << "Max Ratio       : " << maxRatio << std::endl;
  std::cout << "Min Ratio       : " << minRatio << std::endl;
  std::cout << "A_Macro / A_Std : " << ratio_std_macro << std::endl;
  std::cout << "Density Weight  : " << macroWeight << std::endl;
  std::cout << "Max MacroArea   : " << maxMacroArea << std::endl;
  std::cout << "Min MacroArea   : " << minMacroArea << std::endl;

  // if(targetDensity_ < 1.0 && (maxRatio > 1.2 || minRatio < 0.2) )
  for(auto& macro : movableMacroPtrs_)
  {
    //if(macro->dx() > die_.dx() * 0.2 || macro->dy() > die_.dy() * 0.2)
    {
      //printf("large Macro!\n");
      float weight = macro->densityScale();
      macro->setDensityScale(weight * macroWeight); // 1.09??
    }
  }
}

void
SkyPlaceDB::updateFixedOverlapArea()
{
  for(auto& bin : binPtrs_)
    bin->setFixedArea(0);

  for(auto& cell : fixedPtrs_) 
  {
    if(isOutsideDie(cell))
      continue;

    OverlapBins ovBins = findBin(cell);
    int minX  = ovBins.first.first;
    int maxX  = ovBins.first.second;
    int minY  = ovBins.second.first;
    int maxY  = ovBins.second.second;

    for(int i = minX; i < maxX; i++)
    {
      for(int j = minY; j < maxY; j++)
      {
        Bin* bin = binPtrs_[j * numBinX_ + i];
        const float overlapArea = getOverlapArea(bin, cell);
        bin->addFixedArea(overlapArea * bin->targetDensity());
        // FixedArea should be scaled-down with target density
        // (according to the OpenROAD RePlAce comment)
      }
    }
  }
}

void
SkyPlaceDB::printInfo() const 
{
  using namespace std;

  float initHpwl = hpwl_;

  if(lefDefFlag_)
    initHpwl /= static_cast<float>(dbLefDef_->dbUnit());

  cout << endl;
  cout << "*** Summary of SkyPlaceDB ***" << endl;
  cout << "---------------------------------------------" << endl;
  cout << " DESIGN NAME        : " << designName_         << endl;
  cout << " NUM CELL (TOTAL)   : " << numMovable_ + numFixed_ << endl;
  cout << " NUM CELL (MOVABLE) : " << numMovable_         << endl;
  cout << " NUM CELL (FIXED)   : " << numFixed_           << endl;
  cout << " NUM CELL (FILLER)  : " << numFiller_          << endl;
  cout << " NUM MOVABLE MACRO  : " << numMovableMacro_    << endl;
  cout << " NUM NET            : " << numNet()            << endl;
  cout << " NUM PIN            : " << numPin()            << endl;
  cout << " NUM IO             : " << numIO()             << endl;
  cout << " NUM BIN   (IDEAL)  : " << idealNumBin_        << endl;
  cout << " NUM BIN   (TOTAL)  : " << numBinX_ * numBinY_ << endl;
  cout << " NUM BIN_X (USED)   : " << numBinX_            << endl;
  cout << " NUM BIN_Y (USED)   : " << numBinY_            << endl;
  cout << " BIN WIDTH          : " << binX_               << endl;
  cout << " BIN HEIGHT         : " << binY_               << endl;
  cout << " FILLER WIDTH       : " << fillerWidth_        << endl;
  cout << " FILLER HEIGHT      : " << fillerHeight_       << endl;
  cout << " FILLER AREA        : " << avgFillerArea_      << endl;
  cout << " AREA (TOTAL)       : " << sumTotalInstArea_   << endl;
  cout << " AREA (MOVABLE)     : " << sumMovableArea_     << endl;
  cout << " AREA (STD)         : " << sumMovableStdArea_  << endl;
  cout << " AREA (MACRO)       : " << sumMovableMacroArea_<< endl;
  cout << " AREA (FIXED)       : " << sumFixedArea_       << endl;
  cout << " AREA (FILLER)      : " << fillerArea_         << endl;
  cout << " AREA (WHITESPACE)  : " << whiteSpaceArea_     << endl;
  cout << " AREA (CORE)        : " << dieArea_            << endl;
  cout << " TARGET DENSITY     : " << targetDensity_ * 100.0 << "%\n";
  cout << " DENSITY            : " << density_ * 100.0    << "%\n";
  cout << " UTIL               : " << util_    * 100.0    << "%\n";
  cout << " INITIAL HPWL       : " << initHpwl << endl;
  cout << "---------------------------------------------" << endl;
  cout << endl;
}

void
SkyPlaceDB::writeBookShelf(const std::filesystem::path& dir, bool isLg) const
{
  std::string benchName = designName_;
  std::string outputDir = designDir_;

  if(dir != "")
    outputDir = std::string(dir) + "/";

  std::string auxFileName = outputDir + benchName + ".gp.aux";
  std::string plFileName  = outputDir + benchName + ".gp.pl";
  
  std::string command = "mkdir -p " + outputDir;

  std::system(command.c_str());

  // Step #1. Write .aux file 
  std::ofstream aux_output;
  aux_output.open(auxFileName);

  aux_output << "RowBasedPlacement : ";
  aux_output << benchName + ".nodes ";
  aux_output << benchName + ".nets ";
  aux_output << benchName + ".wts ";
  aux_output << benchName + ".gp.pl ";
  aux_output << benchName + ".scl";

  aux_output.close();

  // Step #2. Write .pl file 
  // Print Headline
  std::ofstream pl_output;
  pl_output.open(plFileName);

  pl_output << "UCLA pl 1.0" << std::endl;
  pl_output << "# User: Annonymous" << std::endl;
  pl_output << std::endl;

  for(auto& cell : cellInsts_)
  {
    if(cell.isFiller())
      continue;
    pl_output << cell.bsCellPtr()->name() << " ";
    pl_output << cell.lx() << " ";
    pl_output << cell.ly() << " : N";
    if(cell.isFixed())
      pl_output << " /FIXED";
    pl_output << std::endl;
  }

  pl_output.close();

  printf("Write results to %s.\n", plFileName.c_str());
}

// TODO: Be sure about backslash convention of .def and .v
void modifyComponentName(std::string& compName)
{
  bool busComponent = false;
  size_t bracketPos1 = compName.find("[");
  while(bracketPos1 != std::string::npos)
  {
    compName.replace(bracketPos1, 1, "\\[");
    bracketPos1 = compName.find("[", bracketPos1 + 2);
    busComponent = true;
  }
  
  size_t bracketPos2 = compName.find("]");
  while(bracketPos2 != std::string::npos)
  {
    compName.replace(bracketPos2, 1, "\\]");
    bracketPos2 = compName.find("]", bracketPos2 + 2);
    busComponent = true;
  }

  size_t findDollar = compName.find("$");
  if(findDollar != std::string::npos)
    busComponent = true;

  if(compName[0] == '\\')
    compName = compName.substr(1, compName.size() - 1);
}

void modifyNetName(const dbNet* net, std::string& netName)
{
	// Do not insert backslash for PIN NET (ex. input wire key[127:0])
	for(auto& pin : net->pins())
	{
		if(pin->isExternal())
			return;
	}

  bool busComponent = false;
  size_t bracketPos1 = netName.find("[");
  while(bracketPos1 != std::string::npos)
  {
    netName.replace(bracketPos1, 1, "\\[");
    bracketPos1 = netName.find("[", bracketPos1 + 2);
    busComponent = true;
  }
  
  size_t bracketPos2 = netName.find("]");
  while(bracketPos2 != std::string::npos)
  {
    netName.replace(bracketPos2, 1, "\\]");
    bracketPos2 = netName.find("]", bracketPos2 + 2);
    busComponent = true;
  }

  size_t findDollar = netName.find("$");
  if(findDollar != std::string::npos)
    busComponent = true;

  if(netName[0] == '\\')
    netName = netName.substr(1, netName.size() - 1);
}

void
SkyPlaceDB::writeDef(const std::filesystem::path& dir, bool isLg) const
{
  std::string benchName = designName_;
  std::string outputDir = dbLefDef_->getDir() + "/";

  if(dir != "")
    outputDir = std::string(dir) + "/";

  std::string defFileName = outputDir + benchName;
	defFileName += (isLg == true) ? "_lg.def" : "_gp.def";

  int dbUnit = dbLefDef_->dbUnit();

  // Step #1. Copy Coordinates from SkyPlaceDB to LefDefDB
  for(auto& cell : cellPtrs_)
  {
    if(cell->dbCellPtr() == nullptr)
      continue;

    auto _dbCellPtr = cell->dbCellPtr();

    int lxInt = static_cast<int>( cell->lx() );
    int lyInt = static_cast<int>( cell->ly() );

    _dbCellPtr->setLx( lxInt );
    _dbCellPtr->setLy( lyInt );
  }

  // Step #2. Make Def file
  std::ofstream def_output;
  def_output.open(defFileName);

  def_output << "# Created by Annonymous \n";
  def_output << "# " <<  __DATE__ << " " << __TIME__ << "\n";
  def_output << "\n";
  def_output << "VERSION 5.8 ;\n";
  def_output << "DIVIDERCHAR \"/\" ;\n";
  def_output << "BUSBITCHARS \"[]\" ;\n";
  def_output << "DESIGN " << dbLefDef_->designName() << " ;\n";
  def_output << "UNITS DISTANCE MICRONS " << dbUnit << " ;\n";

  // Step #3. Write DIEAREA
  int dieLx = dbLefDef_->die()->lx();
  int dieLy = dbLefDef_->die()->ly();
  int dieUx = dbLefDef_->die()->ux();
  int dieUy = dbLefDef_->die()->uy();

  def_output << "DIEAREA ";
  def_output << "( " << dieLx << " " << dieLy << " ) ";
  def_output << "( " << dieUx << " " << dieUy << " ) ;\n";

  // Step #4. Write ROW Info
  for(auto& row : dbLefDef_->rows() )
  {
    def_output << "ROW " << row->name() << " " << row->lefSite()->name() << " ";
    def_output << row->origX() << " " << row->origY() << " ";

    auto orient = row->orient();
    if(orient == Orient::N)
      def_output << "N ";
    else if(orient == Orient::FS)
      def_output << "FS ";
    else 
      def_output << "N ";
    // We only support N and FS for dbRow yet

    def_output << "DO "   << row->numSiteX() << " BY " << row->numSiteY() << " ";
    def_output << "STEP " << row->stepX() << " " << row->stepY() << " ;\n";
  }

  // Step #5. Write COMPONENTS Info
  def_output << "\n";
  def_output << "COMPONENTS " << dbLefDef_->cells().size() << " ;\n";

  for(auto& cell : dbLefDef_->cells())
  {
    std::string compName = cell->name();
   
    modifyComponentName(compName);

    def_output << "    - ";
    def_output << compName << " ";
    def_output << cell->lefMacro()->name() << " ";

//    def_output << "+ SOURCE ";
//    auto source = cell->source();
//    if(source == Source::DIST)
//      def_output << "DIST ";
//    else if(source == Source::NETLIST)
//      def_output << "NETLIST ";
//    else if(source == Source::TIMING)
//      def_output << "TIMING ";
//    else if(source == Source::USER)
//      def_output << "USER ";

    def_output << "+";
    if(cell->isFixed())
      def_output << " FIXED ";
    else
      def_output << " PLACED ";

    def_output << "( " << cell->lx() << " " << cell->ly() << " )";

    auto orient = cell->orient();
    if(orient == Orient::N)
      def_output << " N ;\n";
    else if(orient == Orient::S)
      def_output << " S ;\n";
    else if(orient == Orient::FN)
      def_output << " FN ;\n";
    else if(orient == Orient::FS)
      def_output << " FS ;\n";
  }

  def_output << "END COMPONENTS" << std::endl;

  // Step #6. Write external PINS Info
  def_output << "\n";
  def_output << "PINS " << dbLefDef_->ios().size() << " ;\n";

  for(auto& io : dbLefDef_->ios())
  {
    def_output << "- ";
    def_output << io->name() << " + NET " << io->pin()->net()->name() << "\n";
    def_output << "\t" << "+ DIRECTION ";

    auto direction = io->direction();
    if(direction == PinDirection::INPUT)
      def_output << "INPUT\n";
    else if(direction == PinDirection::OUTPUT)
      def_output << "OUTPUT\n";
    else if(direction == PinDirection::INOUT)
      def_output << "INOUT\n";

    def_output << "\t" << "+ FIXED ";
    def_output << "( " << io->origX() << " " << io->origY() << " )";

    auto orient = io->orient();
    if(orient == Orient::N)
      def_output << " N\n";
    else if(orient == Orient::E)
      def_output << " E\n";
    // For external pins, only N and E are supported yet.

    def_output << "\t" << "+ LAYER ";
    def_output << io->layer();
    def_output << " ( " << io->offsetX1() << " " << io->offsetY1() << " )";
    def_output << " ( " << io->offsetX2() << " " << io->offsetY2() << " ) ;\n";
  }

  def_output << "END PINS" << std::endl;

  // Step #7. Write NETS Info
  def_output << "\n";
  def_output << "NETS " << dbLefDef_->nets().size() << " ;\n";

  for(auto& net : dbLefDef_->nets())
  {
    std::string netName = net->name();
    modifyNetName(net, netName);

    def_output << "- ";
    def_output << netName << "\n";

    for(auto& pin : net->pins())
    {
      def_output << "( ";

      if(pin->isExternal())
        def_output << "PIN " << pin->name();
      else
			{
				std::string compNameOfThisPin = pin->cell()->name();
				modifyComponentName(compNameOfThisPin);
        def_output << compNameOfThisPin << " " << pin->lefPin()->name();
			}

      def_output << " ) " << std::endl;
    }

    def_output << ";" << std::endl;
  }

  def_output << "END NETS" << std::endl;

  def_output << std::endl;

  def_output << "END DESIGN" << std::endl;
  def_output.close();

  printf("Write results to %s.\n", defFileName.c_str());
}

void
SkyPlaceDB::printFixedOverlapArea()
{
  std::ofstream output;
  output.open("fixedOverlapArea.txt");

  for(auto& bin : binPtrs_)
    output << bin->fixedArea() << std::endl;

  output.close();
}

void
SkyPlaceDB::printBinDensity()
{
  std::ofstream output;
  output.open("BinDensity.txt");

  for(auto& bin : binPtrs_)
  {
    output << bin->col() << "," << bin->row() << ",";
    output << bin->density() << std::endl;
  }

  output.close();
}

void
SkyPlaceDB::printBinPotential()
{
  std::ofstream output;
  output.open("Potential.txt");

  for(auto& bin : binPtrs_)
  {
    output << bin->col() << "," << bin->row() << ",";
    output << bin->potential() << std::endl;
  }

  output.close();
}

void
SkyPlaceDB::printBinElectroFieldXY()
{
  std::ofstream output;
  output.open("ElectroField.txt");

  for(auto& bin : binPtrs_)
  {
    output << bin->col() << "," << bin->row() << ",";
    output << bin->electroForceX() << ",";
    output << bin->electroForceY() <<  std::endl;
  }

  output.close();
}

void
SkyPlaceDB::debugCell() const
{
  printf("=== Debug Cell ===\n");

  for(auto &cell : cellPtrs_)
    printCellInfo(cell);
}

void
SkyPlaceDB::debugNet() const
{
  printf("=== Debug Net ===\n");

  for(auto &net : netPtrs_)
    printNetInfo(net);
}

void
SkyPlaceDB::printCellInfo(Cell* cell) const
{
  std::cout << "Cell ID: " << cell->id() << std::endl;
  printf("(%f, %f) - (%f, %f)\n", 
      cell->lx(), cell->ly(), cell->ux(), cell->uy() );

  printf("Pin Coordinate\n");
  for(auto &pin: cell->pins())
    printf("ID: %d (%f, %f) Net: %d\n", pin->id(), pin->cx(), pin->cy(), pin->net()->id());
}

void
SkyPlaceDB::printNetInfo(Net* net) const
{
  std::cout << "Net ID: " << net->id() << std::endl;
  printf("(%f, %f) - (%f, %f)\n", 
      net->lx(), net->ly(), net->ux(), net->uy() );

  printf("Pin Coordinate\n");
  for(auto &pin: net->pins())
    printf("ID: %d (%f, %f) Cell: %d\n", pin->id(), pin->cx(), pin->cy(), pin->cell()->id());
}

} // namespace SkyPlace
