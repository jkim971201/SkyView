#pragma once

#include <string>

#include "dbTypes.h"
#include "dbPin.h"

namespace db
{

class dbPin;
class dbSite;

class dbMacro
{

  public:

    dbMacro();

    void print() const;
    void addPin(dbPin pin);

    // Setters
    void setName(const char* name)       { name_ = std::string(name); }
    void setMacroClass(MacroClass cl)    { macroClass_ = cl;     }
    void setSite (dbSite* site)          { site_   = site;       }
    void setSizeX(int sizeX)             { sizeX_  = sizeX;      }
    void setSizeY(int sizeY)             { sizeY_  = sizeY;      }
    void setOrigX(int origX)             { origX_  = origX;      }
    void setOrigY(int origY)             { origY_  = origY;      }
    void setSymmetryX   (bool sym)       { symX_   = sym;        }
    void setSymmetryY   (bool sym)       { symY_   = sym;        }
    void setSymmetryR90 (bool sym)       { symR90_ = sym;        }

    // Getters
    MacroClass   macroClass() const { return macroClass_; }
    dbSite*            site() const { return site_;       }
    const std::string& name() const { return name_;       }

    int sizeX() const { return sizeX_; }
    int sizeY() const { return sizeY_; }
    int origX() const { return origX_; }
    int origY() const { return origY_; }

    bool isSymmetryX()   const { return symX_;   }
    bool isSymmetryY()   const { return symY_;   }
    bool isSymmetryR90() const { return symR90_; }

    const std::vector<dbPin>& pins() const { return pins_; }

    const dbPin* getPin(const std::string& pinName) const;

          std::unordered_map<std::string, dbPin*>& pinMap()       { return pinMap_; }
    const std::unordered_map<std::string, dbPin*>& pinMap() const { return pinMap_; }

  private:

    // LEF Syntax
    std::string  name_;
    MacroClass   macroClass_;
    dbSite*      site_;

    std::vector<dbPin> pins_;

    std::unordered_map<std::string, dbPin*> pinMap_;

    int sizeX_;
    int sizeY_;
    
    int origX_;
    int origY_;

    bool symX_;
    bool symY_;
    bool symR90_;
};

}
