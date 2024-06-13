#ifndef DB_MTERM_H
#define DB_MTERM_H

#include <string>
#include <vector>

#include "dbTypes.h"
#include "dbLayer.h"
#include "dbMacro.h"
#include "dbRect.h"

namespace db
{

class dbMacro;
class dbLayer;

class dbMTerm
{
  public: 

    dbMTerm();

    void print() const;
    void setBoundary(); // Set Boundary Box

    // Setters
    void setName         (std::string  pinName ) { name_     = pinName;    }
    void setMacro        (dbMacro*     lefMacro) { macro_    = lefMacro;   }
    void setPinUsage     (PinUsage     pinUsage) { pinUsage_ = pinUsage;   }
    void setPinDirection (PinDirection pinDir  ) { pinDir_   = pinDir;     }
    void setPinShape     (PinShape     pinShape) { pinShape  = pinShape_;  }
    void addRect         (dbRect&      rect    ) { rects_.push_back(rect); }
    void addRect         (dbRect       rect    ) { rects_.push_back(rect); } 
    // overload of addRect

    // Getters
    // lx ly ux uy are coordinates of LEF PIN BBox
    int lx() const { return lx_; }
    int ly() const { return ly_; }
    int ux() const { return ux_; }
    int uy() const { return uy_; }
    int cx() const { return (ux_ + lx_) / 2; } // this works as offsetX
    int cy() const { return (uy_ + ly_) / 2; } // this works as offsetY

    dbMacro*          macro() const { return macro_;    }
    const std::string& name() const { return name_;     }
    PinUsage          usage() const { return pinUsage_; }
    PinDirection  direction() const { return pinDir_;   }
    PinShape          shape() const { return pinShape_; }

    const std::vector<dbRect>& rects() const { return rects_; }

  private:

    dbMacro* macro_;

    std::string  name_;
    PinUsage     pinUsage_;
    PinDirection pinDir_;
    PinShape     pinShape_;

    std::vector<dbRect> rects_;

    int lx_;
    int ly_;
    int ux_;
    int uy_;
};

}

#endif
