#ifndef DB_NET
#define DB_NET

#include "dbTypes.h"

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

    // Setters
    void setName(const std::string& name) { name_ = name; }
    void addITerm(dbITerm* iterm) { iterms_.push_back(iterm); }
    void addBTerm(dbBTerm* bterm) { bterms_.push_back(bterm); }
    void setUsage(const NetUsage us) { usage_ = us; }

    // Getters
    const std::string& name() const { return name_; }
    const std::vector<dbITerm*>& getITerms() const { return iterms_; }
    const std::vector<dbBTerm*>& getBTerms() const { return bterms_; }
    const NetUsage usage() const { return usage_; }

  private:

    std::string name_;
    
    std::vector<dbITerm*> iterms_;
    std::vector<dbBTerm*> bterms_;

    NetUsage usage_;

};

}

#endif
