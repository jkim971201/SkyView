#ifndef DB_DESIGN
#define DB_DESIGN

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
#include "dbIO.h"
#include "dbNet.h"
#include "dbTerm.h"

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
    void setDie(const defiBox* box);

    // Getters
    dbInst* getInstByName (const std::string& name);
    dbIO*   getIOByName   (const std::string& name);
    dbNet*  getNetByName  (const std::string& name);

    // TODO: Maybe this is not the best to way to describe a "block".
    int coreLx() const { return coreLx_; }
    int coreLy() const { return coreLy_; }
    int coreUx() const { return coreUx_; }
    int coreUy() const { return coreUy_; }

    // Row
    void addNewRow  (const defiRow* row);

    // Inst
    void addNewInst (const defiComponent* comp, const std::string& name);
    void fillInst   (const defiComponent* comp, dbInst* inst);

    // IO
    void addNewIO   (const defiPin* pin, const std::string& name);

    // Net
    dbNet* getNewNet(const std::string& name);

    // Getters
          dbDie* getDie()       { return &die_;  }
    const dbDie* getDie() const { return &die_;  }

          std::vector<dbRow*> getRows()       { return rows_; }
    const std::vector<dbRow*> getRows() const { return rows_; }

          std::vector<dbInst*> getInsts()       { return insts_; }
    const std::vector<dbInst*> getInsts() const { return insts_; }

          std::vector<dbIO*> getIOs()       { return ios_; }
    const std::vector<dbIO*> getIOs() const { return ios_; }

          std::vector<dbNet*> getNets()       { return nets_; }
    const std::vector<dbNet*> getNets() const { return nets_; }

    const std::string& name() const { return name_; }

  private:

    int coreLx_;
    int coreLy_;
    int coreUx_;
    int coreUy_;

    std::shared_ptr<dbTypes> types_;
    std::shared_ptr<dbTech>  tech_;

    std::string          name_;
    dbDie                die_;
    std::vector<dbRow*>  rows_;
    std::vector<dbInst*> insts_;
    std::vector<dbIO*>   ios_;
    std::vector<dbNet*>  nets_;
    std::vector<dbTerm*> terms_;

    std::unordered_map<std::string, dbInst*> str2dbInst_;
    std::unordered_map<std::string, dbIO*>   str2dbIO_;
    std::unordered_map<std::string, dbNet*>  str2dbNet_;
};

}

#endif
