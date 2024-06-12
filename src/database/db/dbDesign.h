#ifndef DB_DESIGN_H
#define DB_DESIGN_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "def/defrReader.hpp"
#include "def/defiAlias.hpp"

#include "dbTypes.h"
#include "dbTech.h"
#include "dbDie.h"
#include "dbRow.h"
#include "dbInst.h"
#include "dbNet.h"
#include "dbBTerm.h"
#include "dbITerm.h"

namespace db
{

class dbDesign
{
  public:

    dbDesign(const std::shared_ptr<dbTypes> types,
             const std::shared_ptr<dbTech>  tech);
    ~dbDesign();

    // Setters
    void setName(const char* name) { name_ = std::string(name); }
    void setDbu(int dbu);
    void setDivider(const char div);
    void setDie(const defiBox* box);

    // Getters
    dbInst*  getInstByName  (const std::string& name);
    dbNet*   getNetByName   (const std::string& name);
    dbBTerm* getBTermByName (const std::string& name);

    const char divider() const { return divider_; }

    // TODO: Maybe this is not the best to way to describe a "core region".
    int coreLx() const { return coreLx_; }
    int coreLy() const { return coreLy_; }
    int coreUx() const { return coreUx_; }
    int coreUy() const { return coreUy_; }

    void setCoreLx(int lx) { coreLx_ = lx; }
    void setCoreLy(int ly) { coreLy_ = ly; }
    void setCoreUx(int ux) { coreUx_ = ux; }
    void setCoreUy(int uy) { coreUy_ = uy; }

    // Row
    void addNewRow  (const defiRow* row);

    // Inst
    void addNewInst (const defiComponent* comp, const std::string& name);
    void fillInst   (const defiComponent* comp, dbInst* inst);

    // BTerm (IO)
    void addNewIO   (const defiPin* pin, const std::string& name);

    // Net
    dbNet* getNewNet(const std::string& name);
    void   fillNet  (const defiNet* defNet, dbNet* net);

    // Getters
          dbDie* getDie()       { return &die_;  }
    const dbDie* getDie() const { return &die_;  }

          std::vector<dbRow*>& getRows()       { return rows_; }
    const std::vector<dbRow*>& getRows() const { return rows_; }

          std::vector<dbInst*>& getInsts()       { return insts_; }
    const std::vector<dbInst*>& getInsts() const { return insts_; }

          std::vector<dbBTerm*>& getBTerms()       { return bterms_; }
    const std::vector<dbBTerm*>& getBTerms() const { return bterms_; }

          std::vector<dbNet*>& getNets()       { return nets_; }
    const std::vector<dbNet*>& getNets() const { return nets_; }

    // Returns the design name
    const std::string& name() const { return name_; } 

  private:

    int coreLx_;
    int coreLy_;
    int coreUx_;
    int coreUy_;

    std::shared_ptr<dbTech>  tech_;
    std::shared_ptr<dbTypes> types_;

    char divider_;

    std::string           name_;
    dbDie                 die_;
    std::vector<dbRow*>   rows_;
    std::vector<dbInst*>  insts_;
    std::vector<dbNet*>   nets_;
    std::vector<dbBTerm*> bterms_;
    std::vector<dbITerm*> iterms_;

    std::unordered_map<std::string, dbInst*>  str2dbInst_;
    std::unordered_map<std::string, dbNet*>   str2dbNet_;
    std::unordered_map<std::string, dbBTerm*> str2dbBTerm_;

    const std::string makeITermName(const std::string& instName, const std::string& mTermName) const;
};

}

#endif
