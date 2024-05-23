#ifndef DB_DATABASE
#define DB_DATABASE

#include <set>
#include <string>
#include <filesystem>
#include <memory>

#include "dbTypes.h"
#include "dbLefReader.h"
#include "dbDefReader.h"
#include "dbTech.h"
#include "dbDesign.h"

namespace db
{

class dbDatabase
{
  public:

    dbDatabase();

    void readLef(const char* filename);
    void readDef(const char* filename);

    std::shared_ptr<dbTech>   getTech()   { return tech_;   }
    std::shared_ptr<dbDesign> getDesign() { return design_; }

  private:

    // Parsing
    std::shared_ptr<dbLefReader> lefReader_;
    std::shared_ptr<dbDefReader> defReader_;
    
    std::string defFile_;           // File name  of .def already read
    std::set<std::string> lefList_; // File names of .lef already read

    // Technology (PDK) and Design
    std::shared_ptr<dbTech>   tech_;
    std::shared_ptr<dbTypes>  types_;
		std::shared_ptr<dbDesign> design_;
};

}

#endif
