#ifndef DB_NET
#define DB_NET

#include "dbTypes.h"

#include "dbITerm.h"
#include "dbBTerm.h"

#include <vector>
#include <string>

namespace db
{

class dbITerm;
class dbBTerm;

class dbNet
{
  public:

    dbNet();

    void print() const;

    // Setters
    void setName   (const std::string& name) { name_   = name; }
    void setUse    (const NetUse use)      { use_    = use;  }
    void setSource (const Source src)      { source_ = src;  }

    void addITerm  (dbITerm* iterm) { iterms_.push_back(iterm); }
    void addBTerm  (dbBTerm* bterm) { bterms_.push_back(bterm); }

    // Getters
    const std::string& name() const { return name_; }
    const std::vector<dbITerm*>& getITerms() const { return iterms_; }
    const std::vector<dbBTerm*>& getBTerms() const { return bterms_; }

    NetUse use()    const { return use_;    }
    Source source() const { return source_; }

  private:

    std::string name_;
    
    std::vector<dbITerm*> iterms_;
    std::vector<dbBTerm*> bterms_;

    NetUse use_;
    Source source_;

};

}

#endif
