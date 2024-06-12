#include <algorithm> // for sort
#include <limits>    // for std::numeric_limits
#include <cassert>   // for assert
#include <thread>    // for std::thread
#include <mutex>     // for std::mutex

#include "Legalizer.h"

namespace SkyPlace 
{

void sortCells(std::vector<AbacusCell*>& cells)
{
  auto cmp = [] (const AbacusCell* cell1, const AbacusCell* cell2)
  { return cell1->lxOrig() < cell2->lxOrig(); };

  /* Sorting Rule : Increasing order of Original X */
  std::stable_sort(cells.begin(), cells.end(), cmp);
}

void sortCells(std::vector<AbacusCell>& cells)
{
  auto cmp = [] (const AbacusCell& cell1, const AbacusCell& cell2)
  { return cell1.lxOrig() < cell2.lxOrig(); };

  /* Sorting Rule : Increasing order of Original X */
  std::stable_sort(cells.begin(), cells.end(), cmp);
}

inline void makeSiteAlign(int& newX, int rowLx, int siteWidth, int margin)
{
  int lenFromLx = newX - rowLx;

  if(margin > 0)
    lenFromLx = (lenFromLx + siteWidth * margin - 1) / siteWidth * siteWidth;
  else if(margin < 0)
    lenFromLx = (lenFromLx + siteWidth * margin + 1) / siteWidth * siteWidth;
  else
    lenFromLx = lenFromLx / siteWidth * siteWidth;

  newX = rowLx + lenFromLx;
}

// AbacusCell //
AbacusCell::AbacusCell(Cell* cell)
{
  dx_ = static_cast<int>(cell->dx());
  dy_ = static_cast<int>(cell->dy());
  lx_ = static_cast<int>(cell->lx());
  ly_ = static_cast<int>(cell->ly());

  lxOrig_ = lx_;
  lyOrig_ = ly_;

  cell_ = cell;
}

AbacusCell::AbacusCell(const AbacusCell* cell)
{
  dx_     = cell->dx();
  dy_     = cell->dy();
  lx_     = cell->lx();
  ly_     = cell->ly();
  lxOrig_ = cell->lxOrig();
  lyOrig_ = cell->lyOrig();

  cell_ = nullptr;
}

void
AbacusCell::exportCoordinates()
{
  float newCx = static_cast<float>(lx_ + dx_ / 2);
  float newCy = static_cast<float>(ly_ + dy_ / 2);
  cell_->setCenterLocation(newCx, newCy);
}

// AbacusCluster //
void 
AbacusCluster::addCell(AbacusCell* cell)
{
  cells_.push_back(cell);
  e_  += 1; // numPin?
  q_  += cell->lxOrig() - dx_;
  dx_ += cell->dx();
}

void 
AbacusCluster::addCluster(const AbacusCluster* cluster)
{
  for(auto cellPtr : cluster->cells())
    cells_.push_back(cellPtr);

  e_  += cluster->e();
  q_  += cluster->q() - cluster->e() * dx_;
  dx_ += cluster->dx();
}

// AbacusRow //
AbacusRow::AbacusRow(const Row* row)
{
  dx_ = static_cast<int>(row->dx());
  dy_ = static_cast<int>(row->dy());
  lx_ = static_cast<int>(row->lx());
  ly_ = static_cast<int>(row->ly());

  sumCellWidth_ = 0;
  siteWidth_ = row->siteWidth();
}

AbacusRow::AbacusRow(int dx, int dy, int lx, int ly, int siteWidth)
  : dx_        (dx),
    dy_        (dy),
    lx_        (lx),
    ly_        (ly),
    siteWidth_ (siteWidth)
{
  sumCellWidth_ = 0;
}

bool 
AbacusRow::addCellToRowTrial(AbacusCell* cell)
{
  if(sumCellWidth_ + cell->dx() > dx_)
    return false;
  else
  {
    cellCopy_ = AbacusCell(cell);
    cellCopy_.setLy(ly_);
    cells_.push_back(&cellCopy_);
    return true;
  }
}

void
AbacusRow::addCellToRow(AbacusCell* cell)
{
  cell->setLy(ly_);
  cells_.push_back(cell); 
  sumCellWidth_ += cell->dx();
}

void 
AbacusRow::removeLastCell() 
{ 
  cells_.pop_back(); 
}

AbacusCluster*
AbacusRow::getLastCluster()
{
  if(clusters_.empty()) 
    return nullptr;
  else                   
    return &clusters_.back();
}

void
AbacusRow::placeRow(bool trial)
{
  clusters_.clear();
  sortCells(cells_);

  for(auto cellPtr : cells_)
  {
    // Line 2 of Algorithm 2
    AbacusCluster* curCluster = getLastCluster();
    
    // Line 3 - 11 of Algorithm 2
    // TODO : Potential bug -> nullptr check for curCluster
    if( (clusters_.empty()) )
    {
      // If no overlap with current cluster,
      // then make new one.
      AbacusCluster newCluster(0, 0, 0, 0); // Line 4-5
      
      int newLx = cellPtr->lxOrig();
      //makeSiteAlign(newLx, lx_, siteWidth_, +1);

      if(newLx + cellPtr->dx() > lx_ + dx_)  
      {
        newLx = lx_ + dx_ - cellPtr->dx();
        //makeSiteAlign(newLx, lx_, siteWidth_, +0);
      }
      else if(newLx < lx_)
      {
        newLx = lx_;
        //makeSiteAlign(newLx, lx_, siteWidth_, +1);
      }

      newCluster.setLx(newLx);              // Line 6
      newCluster.addCell(cellPtr);          // Line 7
      clusters_.push_back(newCluster);
    }
    else if(curCluster->ux() < cellPtr->lxOrig())
    {
      int newLx = cellPtr->lxOrig();

      if(newLx + cellPtr->dx() > lx_ + dx_)  
        newLx = lx_ + dx_ - cellPtr->dx();
      else if(newLx < lx_)
        newLx = lx_;

      if(curCluster->ux() < newLx)
      {
        // If no overlap with current cluster,
        // then make new one.
        AbacusCluster newCluster(0, 0, 0, 0); // Line 4-5
        newCluster.setLx(newLx);              // Line 6
        newCluster.addCell(cellPtr);          // Line 7
        clusters_.push_back(newCluster);
      }
      else
      {
        curCluster->addCell(cellPtr);            // Line 8
        collapseCluster(curCluster, siteWidth_); // Line 9
      }
    }
    else
    {
      // If a cell overlaps with curCluster,
      // merge the cell into the curCluster.
      assert(curCluster != nullptr);
      curCluster->addCell(cellPtr);            // Line 8
      collapseCluster(curCluster, siteWidth_); // Line 9
    }
  }

  // Line 15 - 21 of Algorithm 2
  for(auto& cluster : clusters_)
  {
    int curX = cluster.lx();
    makeSiteAlign(curX, lx_, siteWidth_, +0);
    
    for(auto& cellPtr : cluster.cells())
    {
      if(!trial) 
        cellPtr->setLx(curX);
      else if(trial && cellPtr == &cellCopy_)
        cellPtr->setLx(curX);

      curX += cellPtr->dx();
    }
  }
}

void 
AbacusRow::collapseCluster(AbacusCluster* cluster, int siteWidth)
{
  int xMin = this->lx(); // lx of BBox of this row
  int xMax = this->ux(); // ux of BBox of this row
  int clusterDx = cluster->dx();
  int newLx // newClusterLx
    = cluster->q() / cluster->e(); // 33

  //makeSiteAlign(newLx, lx_, siteWidth, +0);

  // printf("dx of this row : %d %% %d = %d\n", this->dx(), siteWidth, this->dx() % siteWidth);
  // assert(this->dx() % siteWidth == 0);
  // assert(clusterDx % siteWidth == 0);

  if(newLx + clusterDx > xMax)
  {
    newLx = xMax - clusterDx; // 35
   // makeSiteAlign(newLx, lx_, siteWidth, +0);
  }
  else if(newLx < xMin)                                           
  {
    newLx = xMin; // 34
    //makeSiteAlign(newLx, lx_, siteWidth, +1);
  }

  cluster->setLx(newLx); 

  if(clusters_.size() > 1)
  {
    AbacusCluster* prevCluster = &clusters_[clusters_.size() - 2];

    // If current cluster overlaps with previous cluster,
    // then merge the current one to previous one.
    // And remove overlap while minimizing displacement.
    if(prevCluster->ux() > newLx)
    {
      prevCluster->addCluster(cluster);
      clusters_.pop_back();
      collapseCluster(prevCluster, siteWidth);
    }
  }
}

int
AbacusRow::getCostForThisRow() const
{
  return cellCopy_.getDisp();
}

void
AbacusRow::sanityCheck() const
{
  for(auto& cellPtr : cells_)
  {
    if(cellPtr == &cellCopy_)
      continue;
    if(cellPtr->lx() > lx_ + dx_ || cellPtr->lx() < lx_)
      assert(0);
    if(cellPtr->ly() != ly_)
      assert(0);
  }
}

void
AbacusRow::printRow() const
{
  printf("Lx - Ux This Row : %d - %d (%d)\n", lx_, lx_ + dx_, ly_);

  int numCluster = 0;
  int prevUx = 0;
  int curLx = 0;
  for(const auto& cluster : clusters_)
  {
    printf("Cluster[%03d] LxUx : %d - %d\n", numCluster, cluster.lx(), cluster.ux());
    for(const auto& cell : cluster.cells())
    {
      if(cell == &cellCopy_)
        continue;
      curLx = cell->lx();
      printf("%d - %d\n", cell->lx(), cell->ux());
      assert(curLx >= prevUx);
      prevUx = cell->ux();
    }
    numCluster++;
  }
  printf("\n");
}

// Legalizer //
void
Legalizer::importFromDB()
{
  cells_.clear();
  for(auto& rowVec : rows_)
    rowVec.clear();
  rows_.clear();

  for(auto cellPtr : db_->movableCells())
  {
    if(cellPtr->isFiller() || cellPtr->isMacro())
      continue;
    
    cells_.push_back( AbacusCell(cellPtr) );
  } 

  // TODO : Make this can handle non-uniform row height
  rowLxMin_  = (db_->rows()[0]->lx());
  rowUxMax_  = (db_->rows()[0]->ux());

  rowLyMin_  = (db_->rows()[0]->ly());
  rowLyMax_  = (db_->rows().back()->ly());

  rowUyMin_  = (db_->rows()[0]->uy());
  rowUyMax_  = (db_->rows().back()->uy());

  rowHeight_ = (db_->rows()[0]->dy());
  
  std::vector<std::vector<AbacusObstacle>> obstacles(db_->rows().size());

  for(auto cellPtr : db_->fixedCells())
  {
    if(cellPtr->isIO())
      continue;

    // printf("fixedCell : %s\n", cellPtr->dbCellPtr()->name().c_str());

    const int fixedCellLx = static_cast<int>(cellPtr->lx());
    const int fixedCellLy = static_cast<int>(cellPtr->ly());
    const int fixedCellDx = static_cast<int>(cellPtr->dx());
    const int fixedCellDy = static_cast<int>(cellPtr->dy());
    const int fixedCellUx = static_cast<int>(cellPtr->ux());
    const int fixedCellUy = static_cast<int>(cellPtr->uy());
    
    if(fixedCellLx < rowLxMin_ || fixedCellUx > rowUxMax_ || 
       fixedCellLy < rowLyMin_ || fixedCellUy > rowUyMax_)
    {
      continue;
    }

    int rowIdxMin = (fixedCellLy - rowLyMin_    ) / rowHeight_;
    int rowIdxMax = (fixedCellUy - rowLyMin_ - 1) / rowHeight_;
   
    for(int y = rowIdxMin; y < rowIdxMax + 1; y++)
      obstacles[y].push_back( AbacusObstacle(fixedCellLx, fixedCellDx) );
  }

  for(auto& obsVector : obstacles)
  {
    std::stable_sort(obsVector.begin(), obsVector.end(),
                     [&] (const AbacusObstacle& o1, const AbacusObstacle& o2)
                     { return o1.lx() < o2.lx(); } );
  }

  rows_.resize(db_->rows().size());
  for(auto rowPtr : db_->rows())
  {
    const int rowLx = static_cast<int>(rowPtr->lx());
    const int rowLy = static_cast<int>(rowPtr->ly());
    const int rowUx = static_cast<int>(rowPtr->ux());
    const int rowUy = static_cast<int>(rowPtr->uy());
    const int rowDy = static_cast<int>(rowPtr->dy());

    const int siteWidth = rowPtr->siteWidth();

    const int rowIdx = (rowLy - rowLyMin_) / rowHeight_;

    const auto& obsVector = obstacles[rowIdx];
          auto& rowVector = rows_[rowIdx];

    const int numObs = obsVector.size();

    for(int i = 0; i < numObs + 1; i++)
    {
      int newLx;
      int newUx; 

      if(numObs == 0)
      {
        newLx = rowLx;
        newUx = rowUx;
      }
      else
      {
        newLx = (i ==      0) ? rowLx : obsVector[i - 1].ux();
        newUx = (i == numObs) ? rowUx : obsVector[i    ].lx();
      }

      if(i != 0)
        makeSiteAlign(newLx, rowLx, siteWidth, +1);
      if(i != numObs)
        makeSiteAlign(newUx, rowLx, siteWidth, +0);

      assert( (newUx - newLx) % siteWidth == 0);

      if(newLx == newUx)
        continue;
      else
        rowVector.push_back(AbacusRow(newUx - newLx, rowDy, newLx, rowLy, siteWidth));
    }
  }

// For debugging
//  for(int i = 0; i < db_->rows().size(); i++)
//  {
//    printf("Obstacles lx of row [%3d] ", i);
//    for(auto& obs : obstacles[i])
//      printf("%d-%d ", obs.lx(), obs.ux());
//    printf("\n");
//  }
//
//  for(int i = 0; i < rows_.size(); i++)
//  {
//    printf("Row [%3d] Ly : %d Obs : ", i, rows_[i][0].ly());
//    for(int j = 0; j < rows_[i].size(); j++)
//      printf(" %d-%d ", rows_[i][j].lx(), rows_[i][j].ux());
//    printf("\n");
//  }
}

void
Legalizer::exportToDB()
{
  for(auto& cell : cells_)
    cell.exportCoordinates();
}

void
Legalizer::legalize()
{
  printf("[Legalizer] Start Legalization (numThread : %d)\n", numThread_);

  auto t1 = std::chrono::high_resolution_clock::now();

  importFromDB();

  runAbacus();

  exportToDB();

  hpwlBefore_ = static_cast<int>(db_->getHPWL() / db_->dbUnit());
  db_->updateHpwl();
  hpwlAfter_  = static_cast<int>(db_->getHPWL() / db_->dbUnit());

  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> runtime = t2 - t1;
  lgTime_ = runtime.count();

  printStats();
  printf("[Legalizer] Finish Legalization\n");
}

std::vector<AbacusRow*>
Legalizer::getCandidateRows(const AbacusCell& cell) 
{
  std::vector<AbacusRow*> candidates;

  int startIdx = (cell.ly() - rowLyMin_) / rowHeight_;
  //printf("startIdx : %d\n", startIdx);
  
  for(int i = -3; i < 4; i++)
  {
    int curIdx = startIdx + i;
    //printf("curIdx : %d cellLy : %d rowLy: %d numRow : %d\n", 
    //        curIdx, cell.ly(), curIdx * rowHeight_ + rowLyMin_, rows_.size());

    if(curIdx >= 0 && curIdx < rows_.size())
    {
      auto& rowVector = rows_[curIdx];
      
      for(auto& abacusRow : rowVector)
        candidates.push_back(&abacusRow);
    }
  }

  return candidates;
}

void
Legalizer::runAbacus()
{
  // Line 1 of Algorithm 1
  sortCells(cells_); 

  constexpr int inf = std::numeric_limits<int>::max();

  for(auto& cell : cells_)
  {
    // Line 3 of Algorithm 1
    AbacusRow* bestRow = nullptr;  
    int bestCost = inf;

    std::vector<AbacusRow*> candidateRows  
      = getCandidateRows(cell);

    int numCandidate = candidateRows.size();

    /* i == id of candidate row */
    auto tryCandidateRow = [&] (int i) 
    {
      AbacusRow* row = candidateRows[i];

      // Line 5 of Algorithm 1
      bool hasRoom = row->addCellToRowTrial(&cell); 
      
      if(hasRoom)
      {
        // Line 6 of Algorithm 1
        row->placeRow(true); /* trial */

        // Line 7 of Algorithm 1
        int curCost = row->getCostForThisRow();

        std::mutex cost_mutex;

        // Line 8 of Algorithm 1
        cost_mutex.lock();
        if(curCost < bestCost) // Find min displacement
        {
          bestCost = curCost;
          bestRow  = row;
        }
        cost_mutex.unlock();

        // Line 9 of Algorithm 1
        row->removeLastCell();
      }
    };

    runMultiThread(numCandidate, numThread_, tryCandidateRow);

    assert(bestCost != inf);

    if(bestRow != nullptr)
    {
      // Line 11 of Algorithm 1
      bestRow->addCellToRow(&cell);

      // Line 12 of Algorithm 1
      bestRow->placeRow(false); /* real placeRow */
    }
    else
    {
      assert(0);
      // TODO: Search for all rows
    }
  }

  //for(const auto& abacusRowVector : rows_)
  //{
  //  for(const auto& abacusRow : abacusRowVector) 
  //  {
  //    abacusRow.printRow();
  //    abacusRow.sanityCheck();
  //  }
  //}
}

void
Legalizer::runMultiThread(int numJobs, 
                          int numThreadInput, 
                          const std::function<void(int)>& closure)
{
  // We don't have to assign too many threads if there are only few jobs.
  int numThread = std::min(numJobs, numThreadInput);
  
  if(numThread <= 1) 
  {
    for(int i = 0; i < numJobs; ++i) 
      closure(i);
  } 
  else 
  {
    int globalJobIdx = 0;
    std::mutex mtx;
    auto thread_func = [&](int threadIdx) 
    {
      int jobIdx;
      while(true) 
      {
        // We can avoid racing problem by using "mutex"
        mtx.lock();
        jobIdx = globalJobIdx++;
        mtx.unlock();
        if(jobIdx >= numJobs) 
          break;
        closure(jobIdx);
      }
    };

    std::thread threads[numThread];
    for(int i = 0; i < numThread; i++) 
      threads[i] = std::thread(thread_func, i);

    // Real Execution
    for(int i = 0; i < numThread; i++) 
      threads[i].join();
  }
}

void
Legalizer::printStats() const
{
  int64_t maxDisp = 0;
  int64_t sumDisp = 0;

  for(auto& cell : cells_)
  {
    int64_t curDisp = static_cast<int64_t>(cell.getDisp());
    sumDisp += curDisp;
    
    if(curDisp > maxDisp) 
      maxDisp = curDisp;
  }

  int64_t avgDisp = sumDisp / static_cast<int64_t>(cells_.size());

  int lgTimeRound = static_cast<int>(lgTime_);

  printf("\n");
  printf("Legalization Time : %12d s\n", lgTimeRound);
  printf("Avg Displacement  : %12ld \n", avgDisp);
  printf("Max Displacement  : %12ld \n", maxDisp);
  printf("Sum Displacement  : %12ld \n", sumDisp);
  printf("--------------------------------\n");
  printf("HPWL Before       : %12d u\n", hpwlBefore_);
  printf("HPWL After        : %12d u\n", hpwlAfter_);
  printf("\n");
}

}; // namespace SkyPlace
