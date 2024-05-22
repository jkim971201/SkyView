#pragma once

#include <string>
#include <vector>

#include "dbTypes.h"
#include "dbLayer.h"

namespace db
{

// This class is for describing the pin shape of LefPin
struct dbRect
{
  int lx;
  int ly;
  int ux;
  int uy;

  dbRect(int lx, int ly, int ux, int uy)
    : lx(lx), ly(ly), ux(ux), uy(uy) {}
};

class dbMacro;
class dbLayer;

class dbPin
{
  public: 

    dbPin();

    void print() const;

    // Setters
    void setMacro        (dbMacro*     lefMacro) { dbMacro_  = lefMacro;    }
    void setLayer        (dbLayer*     lefLayer) { dbLayer_  = lefLayer;    }
    void setPinUsage     (PinUsage     pinUsage) { pinUsage_ = pinUsage;    }
    void setPinDirection (PinDirection pinDir  ) { pinDir_   = pinDir;      }
    void setPinShape     (PinShape     pinShape) { pinShape  = pinShape_;   }
    void addRect         (dbRect       rect    ) { dbRect_.push_back(rect); }

    // Getters
    int lx() const { return lx_;             }
    int ly() const { return ly_;             }
    int ux() const { return ux_;             }
    int uy() const { return uy_;             }
    int cx() const { return (ux_ + lx_) / 2; } // this works as offsetX
    int cy() const { return (uy_ + ly_) / 2; } // this works as offsetY

    dbMacro*          macro() const { return dbMacro_;  }
    dbLayer*          layer() const { return dbLayer_;  }
    const std::string& name() const { return pinName_;  }
    PinUsage          usage() const { return pinUsage_; }
    PinDirection  direction() const { return pinDir_;   }
    PinShape          shape() const { return pinShape_; }

    const std::vector<dbRect>& rects() const { return dbRect_; }

  private:

    dbMacro* dbMacro_;
    dbLayer* dbLayer_;

    std::string  pinName_;
    PinUsage     pinUsage_;
    PinDirection pinDir_;
    PinShape     pinShape_;

    std::vector<dbRect> dbRect_;

    int lx_;
    int ly_;
    int ux_;
    int uy_;
};

}
