#ifndef DB_DATABASE_H
#define DB_DATABASE_H

#include <set>
#include <string>
#include <filesystem>
#include <memory>

#include "dbTypes.h"
#include "dbTech.h"
#include "dbDesign.h"

namespace db
{

class dbLefReader;
class dbDefReader;
class dbVerilogReader;
class dbBookShelfReader;

class dbDatabase
{
  public:

    dbDatabase();

    void readLef          (const char* filename);
    void readDef          (const char* filename);
    void readVerilog      (const char* filename);
    void readBookShelf    (const char* filename);
    void setTopModuleName (const char*  topname);

    std::shared_ptr<dbTech>   getTech()   { return tech_;   }
    std::shared_ptr<dbDesign> getDesign() { return design_; }

  private:

    // Parsing
    std::shared_ptr<dbLefReader>       lefReader_;
    std::shared_ptr<dbDefReader>       defReader_;
    std::shared_ptr<dbVerilogReader>   verilogReader_;
    std::shared_ptr<dbBookShelfReader> bsReader_;
    
    std::string auxFile_;           // File name  of .aux already read
    std::string   vFile_;           // File name  of .v   already read
    std::string defFile_;           // File name  of .def already read
    std::set<std::string> lefList_; // File names of .lef already read

    // Technology (PDK) and Design
    std::shared_ptr<dbTech>   tech_;
    std::shared_ptr<dbTypes>  types_;
    std::shared_ptr<dbDesign> design_;
};

}

#endif
