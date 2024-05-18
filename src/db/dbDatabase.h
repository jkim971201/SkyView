#pragma once

#include <set>
#include <string>
#include <filesystem>
#include <memory>

#include "dbTypes.h"
#include "dbLefReader.h"
#include "dbTech.h"

namespace db
{

class dbDatabase
{
  public:

    dbDatabase();

    void readLef(const std::filesystem::path& file);

  private:

    // Parsing
    std::shared_ptr<dbLefReader> lefReader_;
    
    // Technology (PDK) and Design
    std::shared_ptr<dbTech>  tech_;
    std::shared_ptr<dbTypes> types_;

    std::set<std::string> lefList_;
};

}
