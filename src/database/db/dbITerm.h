#ifndef DB_ITERM
#define DB_ITERM

#include "dbNet.h"
#include "dbInst.h"
#include "dbMTerm.h"

namespace db
{

class dbNet;
class dbInst;
class dbMTerm;

class dbITerm
{
  public:

    dbITerm();

    void print() const;

    // Setters
		void setNet   (dbNet*     net) { net_    = net;    }
		void setInst  (dbInst*   inst) { inst_   = inst;   }
		void setMTerm (dbMTerm* mterm) { mterm_  = mterm;  }

		// Getters
    const dbNet*   net() const { return net_;   }
    const dbInst* inst() const { return inst_;  }
    const dbMTerm* pin() const { return mterm_; }

  private:

		dbNet*   net_;
		dbInst*  inst_;
		dbMTerm* mterm_;
};

}

#endif
