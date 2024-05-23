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

namespace db
{

class dbDesign
{
  public:

    dbDesign(const std::shared_ptr<dbTypes> types,
             const std::shared_ptr<dbTech>  tech);
    ~dbDesign();

    dbInst* getInstByName(const std::string& name);

    // Setters
    void setName(const char* name) { name_ = std::string(name); }
    void setDbu(int dbu);
    void setDie(const defiBox* box);

    void addNewRow  (const defiRow* ro);
    void addNewInst (const defiComponent* comp, const std::string& name);
    void fillInst   (const defiComponent* comp, dbInst* inst);

    // Getters
          dbDie* getDie()       { return &die_;  }
    const dbDie* getDie() const { return &die_;  }

          std::vector<dbRow*> getRows()       { return rows_; }
    const std::vector<dbRow*> getRows() const { return rows_; }

          std::vector<dbInst*> getInsts()       { return insts_; }
    const std::vector<dbInst*> getInsts() const { return insts_; }

    const std::string& name() const { return name_; }

  private:

    std::shared_ptr<dbTypes> types_;
    std::shared_ptr<dbTech>  tech_;

    std::string          name_;
    dbDie                die_;
    std::vector<dbRow*>  rows_;
    std::vector<dbInst*> insts_;
    //std::vector<dbIO*>    ios_;
    //std::vector<dbNet*>   nets_;
    //std::vector<dbTerm*> tersm_;

    std::unordered_map<std::string, dbInst*> str2dbInst_;
};

}

#endif
