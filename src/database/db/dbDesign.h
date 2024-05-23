#ifndef DB_DESIGN
#define DB_DESIGN

#include <vector>
#include <memory>
#include <string>

#include "def/defrReader.hpp"
#include "def/defiAlias.hpp"

#include "dbTypes.h"
#include "dbTech.h"
#include "dbDie.h"
#include "dbRow.h"

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

    void addNewRow(const defiRow* ro);

		// Getters
    const dbDie* getDie() const { return &die_; }

  private:

    std::shared_ptr<dbTypes> types_;
    std::shared_ptr<dbTech>  tech_;

    std::string          name_;
    dbDie                die_;
    std::vector<dbRow>   rows_;
    //std::vector<dbIO>    ios_;
    //std::vector<dbInst>  insts_;
    //std::vector<dbNet>   nets_;
    //std::vector<dbTerms> tersm_;
};

}

#endif
