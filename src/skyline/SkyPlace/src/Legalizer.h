#pragma once

#include "PlacerDB.h"
#include <functional>

namespace SkyPlace 
{

class AbacusCell;
class AbacusRow;
class AbacusCluster;

class AbacusCell
{
  public:

    AbacusCell() {};
    AbacusCell(Cell* cell);
    AbacusCell(const AbacusCell* cell); 
    // This constructor will be used when AbacusRow make a copy of AbacusCell

    void setLx(int lx) { lx_ = lx; }
    void setLy(int ly) { ly_ = ly; }

    int dx()     const { return dx_;       }
    int dy()     const { return dx_;       }
    int lx()     const { return lx_;       }
    int ly()     const { return ly_;       }
    int ux()     const { return lx_ + dx_; }
    int uy()     const { return ly_ + dy_; }
    int lxOrig() const { return lxOrig_;   }
    int lyOrig() const { return lyOrig_;   }

    int getDisp() const { return std::abs(lx_ - lxOrig_) + std::abs(ly_ - lyOrig_); }

    const Cell* cell() const { return cell_; }

    void exportCoordinates();

  private:

    int dx_; // dx == width
    int dy_; // dy == height
    int lx_;
    int ly_;
    int lxOrig_;
    int lyOrig_;

    Cell* cell_;
};

class AbacusCluster
{
  public:

    AbacusCluster(int lx, int dx, int q, int e)
      : lx_  (lx), 
        dx_  (dx), 
         q_  ( q), 
         e_  ( e) 
    {}

          std::vector<AbacusCell*>& cells()       { return cells_; }
    const std::vector<AbacusCell*>& cells() const { return cells_; }

    void setLx(int lx) { lx_ = lx; }
    void setDx(int dx) { dx_ = dx; }

    // For Abacus Algorithm
    void addCell(AbacusCell* cell);
    void addCluster(const AbacusCluster* cluster);

    int lx() const { return lx_;       }
    int ux() const { return lx_ + dx_; }
    int dx() const { return dx_;       }
    int  q() const { return  q_;       }
    int  e() const { return  e_;       }

  private:

    std::vector<AbacusCell*> cells_;
    int lx_;
    int dx_;
    int  q_;
    int  e_;
};

class AbacusRow
{
  public:

    AbacusRow() {}
    AbacusRow(const Row* row);
    AbacusRow(int dx, int dy, int lx, int ly, int siteWidth);

    int lx() const { return lx_; }
    int ly() const { return ly_; }
    int dx() const { return dx_; }
    int dy() const { return dy_; }

    int ux() const { return lx_ + dx_; }
    int uy() const { return ly_ + dy_; }

    std::vector<AbacusCell*>& cells() { return cells_; }

    bool addCellToRowTrial(AbacusCell* cell);
    void addCellToRow     (AbacusCell* cell);
    void removeLastCell();
    void collapseCluster(AbacusCluster* cluster, int siteWidth);
    void placeRow(bool trial);

    int getCostForThisRow() const;

    void sanityCheck() const;
    void printRow() const;

  private:
   
    AbacusCluster* getLastCluster();

    int lx_;
    int ly_;
    int dx_;
    int dy_;

    int sumCellWidth_; // This will used to detect if this row is full
    int siteWidth_;

    // To parallelize Abacus, 
    // we have to store one copy of AbacusCell inside each AbacusRow.
    // Therefore, if a cell is just inserted temporarily
    // AbacusRow will insert a pointer of its copy, not the pointer of the cell.
    AbacusCell cellCopy_;

    std::vector<AbacusCell*>   cells_;
    std::vector<AbacusCluster> clusters_;
};

class AbacusObstacle
{
  public: 
    
    AbacusObstacle(int lx, int dx) : lx_(lx), dx_(dx) {}
 
    int lx() const { return lx_;       }
    int dx() const { return dx_;       }
    int ux() const { return lx_ + dx_; }

  private:

    int lx_;
    int dx_;
};

class Legalizer 
{
  public:

    Legalizer(std::shared_ptr<PlacerDB> db) : db_(db) {}

    void legalize();

    const std::vector<std::vector<AbacusRow>>& rows() const { return rows_; }

  private:

    int numThread_ = 1;

    std::shared_ptr<PlacerDB> db_;

    std::vector<AbacusCell> cells_;
    std::vector<std::vector<AbacusRow>> rows_;

    int rowHeight_;
    int rowLxMin_;
    int rowUxMax_;
    int rowLyMin_;
    int rowLyMax_;
    int rowUyMax_;
    int rowUyMin_;

    std::vector<AbacusRow*> getCandidateRows(const AbacusCell& cell);

    void importFromDB();
    void exportToDB();
    void runAbacus();

    void runMultiThread(int numJobs, 
                        int numThreadInput, 
                        const std::function<void(int)>& closure);

    // Stats
    int hpwlBefore_;
    int hpwlAfter_;
    double lgTime_;

    void printStats() const;
};

}; // namespace 
