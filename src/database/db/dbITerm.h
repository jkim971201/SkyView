#ifndef DB_ITERM
#define DB_ITERM

#include "dbNet.h"

#include <string>

namespace db
{

class dbNet;
class dbInst;
class dbMTerm;

class dbITerm
{
  public:

    dbITerm();
    dbITerm(const std::string& name, dbInst* inst, dbMTerm* mterm);

    void print() const;

    // Setters
    void setNet   (dbNet*     net) { net_    = net;    }
    void setInst  (dbInst*   inst) { inst_   = inst;   }
    void setMTerm (dbMTerm* mterm) { mterm_  = mterm;  }

    // Getters
    const std::string& name()     const { return name_;  }
    const dbNet*       getNet()   const { return net_;   }
    const dbInst*      getInst()  const { return inst_;  }
    const dbMTerm*     getMTerm() const { return mterm_; }

  private:

    // Name of dbITerm : 
    // "InstanceName" + "DividerCharacter" + "MTermName"
    std::string name_;

    dbNet*   net_;
    dbInst*  inst_;
    dbMTerm* mterm_;
};

}

#endif
